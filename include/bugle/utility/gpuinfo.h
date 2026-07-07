#pragma once

#include <nlohmann/json.hpp>
#include <vector>

namespace bugle {


struct GpuInfo
{
    std::string _title = "Gpu";
    std::string _icon = "🎨";
    std::vector<std::string> _order = { "adapter", "textures", "vertices", "sizes", "compute" };

    struct Adapter {
        std::string _icon = "👩‍🎨";
        std::string adapterType;
        std::string backendType;
        std::string description;
        std::string device;
        uint32_t deviceID;
        std::string vendor;
        uint32_t vendorID;
    } adapter;

    struct Textures {
        std::string _icon = "🖼️";
        uint32_t maxTextureDimension1D;
        uint32_t maxTextureDimension2D;
        uint32_t maxTextureDimension3D;
        uint32_t maxTextureArrayLayers;
    } textures;

    struct Vertices {
        std::string _icon = "🧱";
        uint32_t maxVertexBuffers;
        uint32_t maxVertexAttributes;
        uint32_t maxVertexBufferArrayStride;
    } vertices;

    struct Sizes {
        std::string _icon = "📏";
        float maxUniformBufferBindingSizeKiB;
        float maxStorageBufferBindingSizeGiB;
        float maxComputeWorkgroupStorageSizeKiB;
    } sizes;

    struct Compute {
        std::string _icon = "🧮";
        uint32_t maxComputeInvocationsPerWorkgroup;
        uint32_t maxComputeWorkgroupSizeX;
        uint32_t maxComputeWorkgroupSizeY;
        uint32_t maxComputeWorkgroupSizeZ;
        uint32_t maxComputeWorkgroupsPerDimension;
    } compute;

    static GpuInfo current();
};


NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE( GpuInfo::Adapter,
    _icon,
    adapterType,
    backendType,
    description,
    device,
    deviceID,
    vendor,
    vendorID
);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE( GpuInfo::Textures,
    _icon,
    maxTextureDimension1D,
    maxTextureDimension2D,
    maxTextureDimension3D,
    maxTextureArrayLayers
);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE( GpuInfo::Vertices,
    _icon,
    maxVertexBuffers,
    maxVertexAttributes,
    maxVertexBufferArrayStride
);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE( GpuInfo::Sizes,
    _icon,
    maxUniformBufferBindingSizeKiB,
    maxStorageBufferBindingSizeGiB,
    maxComputeWorkgroupStorageSizeKiB
);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE( GpuInfo::Compute,
    _icon,
    maxComputeInvocationsPerWorkgroup,
    maxComputeWorkgroupSizeX,
    maxComputeWorkgroupSizeY,
    maxComputeWorkgroupSizeZ,
    maxComputeWorkgroupsPerDimension
);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE( GpuInfo,
    _title,
    _icon,
    _order,
    adapter,
    textures,
    vertices,
    sizes,
    compute
);


}   //  ::bugle
