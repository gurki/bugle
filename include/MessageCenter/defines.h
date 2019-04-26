#pragma once

//  register module; convenience for checking mc availability

#define MODULE_MESSAGE_CENTER

//  __va__opt__ support; supported by clang, workaround implemented for msvc

#define MC_PP_THIRD_ARG(a,b,c,...) c
#define MC_VA_OPT_SUPPORTED_I(...) MC_PP_THIRD_ARG(__VA_OPT__(,),true,false,)
#define MC_VA_OPT_SUPPORTED MC_VA_OPT_SUPPORTED_I(?)

//  message meta info

#define MC_INFO_VALUES __FILE__, __func__, __LINE__
#define MC_INFO_NAMES file, func, line
#define MC_INFO_DECLARE const char* file, const char* func, const int line
#define MC_INFO_DECLARE_DEFAULT const char* file = nullptr, const char* func = nullptr, const int line = -1

//  conveniance mc global instance getter

#define MC ( mc::MessageCenter::instance() )

#ifdef MC_LOWERCASE_DEFINES
    #define mci MC
#endif

//  post calls for automatic meta info insertion

#ifdef MC_ENABLE
    #if MC_VA_OPT_SUPPORTED
        #define MC_POST( text, ... ) ( MC.post( text, MC_INFO_VALUES __VA_OPT__(, __VA_ARGS__) ))
    #else
        #define MC_POST( text, ... ) ( MC.post( text, MC_INFO_VALUES, __VA_ARGS__ ) )
    #endif
#else 
    #define MC_POST( text, ... )
#endif

//  post shorthands

#define MCP MC_POST

#ifdef MC_LOWERCASE_DEFINES
    #define mcp MC_POST
#endif

//  scope 

#ifdef MC_ENABLE
    #define MC_SCOPE(...) mc::Scope _mc_scope( { __VA_ARGS__ }, MC_INFO_VALUES )
#else 
    #define MC_SCOPE(...)
#endif 

//  scope shorthands

#define MCS MC_SCOPE

#ifdef MC_LOWERCASE_DEFINES
    #define mcs MC_SCOPE
#endif