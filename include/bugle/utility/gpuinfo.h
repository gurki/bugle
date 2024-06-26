#pragma once

#include <nlohmann/json.hpp>

namespace bugle {


struct GpuInfo 
{
    std::string renderer;
    std::string version;
    int maxPatchVertices;
    int maxTextureImageUnits;
    int maxTextureSize;
    int maxArrayTextureLayers;
    int max3dTextureSize;
    int ramTotalMb;
    int ramAvailableMb;

    static GpuInfo current();
};


NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE( GpuInfo, 
    renderer, 
    version, 
    maxPatchVertices, 
    maxTextureImageUnits, 
    maxTextureSize, 
    maxArrayTextureLayers,
    max3dTextureSize,
    ramTotalMb,
    ramAvailableMb
);


}   //  ::bugle
