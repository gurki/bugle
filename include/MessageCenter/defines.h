#pragma once

//  register module; convenience for checking mc availability

#define MODULE_MESSAGE_CENTER

//  __va__opt__ support; supported by clang, workaround implemented for msvc

#define PP_THIRD_ARG(a,b,c,...) c
#define VA_OPT_SUPPORTED_I(...) PP_THIRD_ARG(__VA_OPT__(,),true,false,)
#define VA_OPT_SUPPORTED VA_OPT_SUPPORTED_I(?)

//  message meta info

#define MC_INFO_VALUES __FILE__, __func__, __LINE__
#define MC_INFO_NAMES file, func, line
#define MC_INFO_DECLARE const char* file, const char* func, const int line
#define MC_INFO_DECLARE_DEFAULT const char* file = nullptr, const char* func = nullptr, const int line = -1

//  conveniance mc global instance getter

#define MC ( mc::MessageCenter::instance() )
#ifndef MC_DISABLE_LOWERCASE_DEFINES
    #define mci MC
#endif

//  post calls for automatic meta info insertion

#ifdef MC_DISABLE_POST
    #define MC_POST( text )
#else 
    #if VA_OPT_SUPPORTED
        #define MC_POST( text, ... ) ( MC.post( text, MC_INFO_VALUES __VA_OPT__(, __VA_ARGS__) ))
    #else
        #define MC_POST( text, ... ) ( MC.post( text, MC_INFO_VALUES, __VA_ARGS__ ) )
    #endif
#endif

//  convenience post shorthands

#define MCP MC_POST
#ifndef MC_DISABLE_LOWERCASE_DEFINES
    #define mcp MC_POST
    #define mc_post MC_POST 
#endif