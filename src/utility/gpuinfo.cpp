#include "bugle/utility/gpuinfo.h"

#define WEBGPU_CPP_IMPLEMENTATION
#include <webgpu/webgpu.hpp>

namespace bugle {


////////////////////////////////////////////////////////////////////////////////
static std::string backendTypeName( wgpu::BackendType backendType )
{
    switch ( backendType ) {
        case wgpu::BackendType::Undefined: return "Undefined";
        case wgpu::BackendType::Null: return "Null";
        case wgpu::BackendType::WebGPU: return "WebGPU";
        case wgpu::BackendType::D3D11: return "D3D11";
        case wgpu::BackendType::D3D12: return "D3D12";
        case wgpu::BackendType::Metal: return "Metal";
        case wgpu::BackendType::Vulkan: return "Vulkan";
        case wgpu::BackendType::OpenGL: return "OpenGL";
        case wgpu::BackendType::OpenGLES: return "OpenGLES";
        case wgpu::BackendType::Force32: return "Force32";
        default: return "Unknown";
    }
}


////////////////////////////////////////////////////////////////////////////////
static std::string adapterTypeName( wgpu::AdapterType adapterType )
{
    switch ( adapterType ) {
        case wgpu::AdapterType::DiscreteGPU: return "Discrete GPU";
        case wgpu::AdapterType::IntegratedGPU: return "Integrated GPU";
        case wgpu::AdapterType::CPU: return "CPU";
        case wgpu::AdapterType::Unknown: return "Unknown";
        case wgpu::AdapterType::Force32: return "Virtual GPU";
        default: return "Invalid Adapter Type";
    }
}


////////////////////////////////////////////////////////////////////////////////
GpuInfo GpuInfo::current()
{
    static const GpuInfo info = [] {
        wgpu::Instance instance = wgpu::createInstance( {} );
        if ( ! instance ) {
            return GpuInfo {};
        }
        wgpu::Adapter adapter = instance.requestAdapter( {} );
        if ( ! adapter ) {
            instance.release();
            return GpuInfo {};
        }

        wgpu::AdapterInfo adapterInfo {};
        wgpu::Limits limits {};
        struct Cleanup {
            wgpu::Instance instance;
            wgpu::Adapter adapter;
            wgpu::AdapterInfo& info;
            ~Cleanup() {
                info.freeMembers();
                adapter.release();
                instance.release();
            }
        } cleanup { instance, adapter, adapterInfo };
        if ( adapter.getInfo( &adapterInfo ) != wgpu::Status::Success ||
             adapter.getLimits( &limits ) != wgpu::Status::Success ) {
            return GpuInfo {};
        }
        const auto copyString = []( const auto& view ) {
            return view.data ? std::string( view.data, view.length ) : std::string {};
        };

        const GpuInfo result = GpuInfo {
            .adapter = {
                //  copies, not string_views — the wgpu strings die with adapterInfo
                .adapterType = adapterTypeName( adapterInfo.adapterType ),
                .backendType = backendTypeName( adapterInfo.backendType ),
                .description = copyString( adapterInfo.description ),
                .device = copyString( adapterInfo.device ),
                .deviceID = adapterInfo.deviceID,
                .vendor = copyString( adapterInfo.vendor ),
                .vendorID = adapterInfo.vendorID
            },
            .textures = {
                .maxTextureDimension1D = limits.maxTextureDimension1D,
                .maxTextureDimension2D = limits.maxTextureDimension2D,
                .maxTextureDimension3D = limits.maxTextureDimension3D,
                .maxTextureArrayLayers = limits.maxTextureArrayLayers
            },
            .vertices = {
                .maxVertexBuffers = limits.maxVertexBuffers,
                .maxVertexAttributes = limits.maxVertexAttributes,
                .maxVertexBufferArrayStride = limits.maxVertexBufferArrayStride,
            },
            .sizes = {
                .maxUniformBufferBindingSizeKiB = limits.maxUniformBufferBindingSize / 1024.0f,
                .maxStorageBufferBindingSizeGiB = limits.maxStorageBufferBindingSize / ( 1024.0f * 1024.0f * 1024.0f ),
                .maxComputeWorkgroupStorageSizeKiB = limits.maxComputeWorkgroupStorageSize / 1024.0f,
            },
            .compute = {
                .maxComputeInvocationsPerWorkgroup = limits.maxComputeInvocationsPerWorkgroup,
                .maxComputeWorkgroupSizeX = limits.maxComputeWorkgroupSizeX,
                .maxComputeWorkgroupSizeY = limits.maxComputeWorkgroupSizeY,
                .maxComputeWorkgroupSizeZ = limits.maxComputeWorkgroupSizeZ,
                .maxComputeWorkgroupsPerDimension = limits.maxComputeWorkgroupsPerDimension,
            }
        };

        return result;
    }();

    return info;
}


}   //  ::bugle
