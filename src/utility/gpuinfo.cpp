#include "bugle/utility/gpuinfo.h"

#ifdef GLEW_STATIC
#include <gl/glew.h>
#include <glfw/glfw3.h>
#endif 

namespace bugle {


GpuInfo GpuInfo::current() 
{
    GpuInfo info {};
#ifdef GL_VERSION
    const GLubyte* renderer = glGetString( GL_RENDERER );
    const GLubyte* version = glGetString( GL_VERSION );
    info.renderer = reinterpret_cast<const char*>( renderer );
    info.version = reinterpret_cast<const char*>( version );

    glGetIntegerv( GL_MAX_PATCH_VERTICES, (GLint*)&info.maxPatchVertices );
    glGetIntegerv( GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, (GLint*)&info.maxTextureImageUnits );
    glGetIntegerv( GL_MAX_TEXTURE_SIZE, (GLint*)&info.maxTextureSize );
    glGetIntegerv( GL_MAX_ARRAY_TEXTURE_LAYERS, (GLint*)&info.maxArrayTextureLayers );
    glGetIntegerv( GL_MAX_3D_TEXTURE_SIZE, (GLint*)&info.max3dTextureSize );

    if ( glewIsSupported( "GL_NVX_gpu_memory_info" ) ) 
    {
        GLint totalKb, availKb;
        glGetIntegerv( GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &totalKb );
        glGetIntegerv( GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &availKb );
        info.ramTotalMb = totalKb / 1024;
        info.ramAvailableMb = availKb / 1024;
    } 
    else if ( glewIsSupported( "GL_ATI_meminfo" ) ) 
    {
        GLint vboFreeMemory[ 4 ];
        glGetIntegerv( GL_TEXTURE_FREE_MEMORY_ATI, vboFreeMemory );
        info.ramTotalMb = vboFreeMemory[ 0 ] / 1024;
        info.ramAvailableMb = vboFreeMemory[ 1 ] / 1024;
    }
#else 
    info.renderer = "n/a";
    info.version = " n/a";
    info.maxPatchVertices = -1;
    info.maxTextureImageUnits = -1;
    info.maxTextureSize = -1;
    info.maxArrayTextureLayers = -1;
    info.max3dTextureSize = -1;
    info.ramTotalMb = -1;
    info.ramAvailableMb = -1;
#endif;
    return info;
}


}   //  ::bugle