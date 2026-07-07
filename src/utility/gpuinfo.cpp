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
    static GpuInfo info {};
    static bool dirty = true;

    if ( dirty )
    {
        wgpu::Instance instance = wgpu::createInstance( {} );
        wgpu::Adapter adapter = instance.requestAdapter( {} );

        wgpu::AdapterInfo adapterInfo;
        wgpu::Limits limits;
        wgpu::SupportedFeatures features;
        adapter.getInfo( &adapterInfo );
        adapter.getLimits( &limits );
        adapter.getFeatures( &features );

        info = GpuInfo {
            .adapter = {
                //  copies, not string_views — the wgpu strings die with adapterInfo
                .adapterType = adapterTypeName( adapterInfo.adapterType ),
                .backendType = backendTypeName( adapterInfo.backendType ),
                .description = std::string( adapterInfo.description.data, adapterInfo.description.length ),
                .device = std::string( adapterInfo.device.data, adapterInfo.device.length ),
                .deviceID = adapterInfo.deviceID,
                .vendor = std::string( adapterInfo.vendor.data, adapterInfo.vendor.length ),
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

        adapter.release();
        instance.release();

        dirty = false;
    }

    return info;
}


}   //  ::bugle
