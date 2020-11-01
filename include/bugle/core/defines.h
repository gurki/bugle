#pragma once

//  register module; convenience for checking mc availability

#define MODULE_BUGLE

//  __va__opt__ support; supported by clang, workaround implemented for msvc

#define BUGLE_PP_THIRD_ARG(a,b,c,...) c
#define BUGLE_VA_OPT_SUPPORTED_I(...) BUGLE_PP_THIRD_ARG(__VA_OPT__(,),true,false,)
#define BUGLE_VA_OPT_SUPPORTED BUGLE_VA_OPT_SUPPORTED_I(?)

//  message meta info

#define BUGLE_INFO_VALUES __FILE__, __func__, __LINE__
#define BUGLE_INFO_NAMES file, func, line
#define BUGLE_INFO_DECLARE const char* file, const char* func, const int line
#define BUGLE_INFO_DECLARE_DEFAULT const char* file = nullptr, const char* func = nullptr, const int line = -1

//  conveniance bgl global instance getter

#define BUGLE_INSTANCE ( bugle::MessageCenter::instance() )

#ifdef BUGLE_LOWERCASE_DEFINES
    #define bgli BUGLE_INSTANCE
#endif

// //  post calls for automatic meta info insertion

// #ifdef BUGLE_ENABLE
//     #if BUGLE_VA_OPT_SUPPORTED
//         #define BUGLE_POST( text, ... ) ( BUGLE_INSTANCE.post( text, BUGLE_INFO_VALUES __VA_OPT__(, __VA_ARGS__) ))
//     #else
//         #define BUGLE_POST( text, ... ) ( BUGLE_INSTANCE.post( text, BUGLE_INFO_VALUES, __VA_ARGS__ ) )
//     #endif
// #else
//     #define BUGLE_POST( text, ... )
// #endif

// #define BGLP BUGLE_POST

// #ifdef BUGLE_LOWERCASE_DEFINES
//     #define bglp BUGLE_POST
// #endif

//  scope

#ifdef BUGLE_ENABLE
    #define BUGLE_SCOPE(...) bugle::Scope _bugle_scope( { __VA_ARGS__ }, BUGLE_INFO_VALUES )
#else
    #define BUGLE_SCOPE(...)
#endif

#ifdef BUGLE_LOWERCASE_DEFINES
    #define bgls BUGLE_SCOPE
#endif

//  fmt with variadic arguments

#ifdef BUGLE_ENABLE
    #define BUGLE_POST( message, ... ) ( BUGLE_INSTANCE.post( BUGLE_INFO_VALUES, message, { __VA_ARGS__ } ) )
    #define BUGLE_POST_FORMAT( tags, message, ... ) ( BUGLE_INSTANCE.post( BUGLE_INFO_VALUES, message, tags, __VA_ARGS__ ) )
    #define BUGLE_POST_FORMAT_NOTAG( message, ... ) ( BUGLE_INSTANCE.post( BUGLE_INFO_VALUES, message, {}, __VA_ARGS__ ) )
#else
    #define BUGLE_POST( message, ... )
    #define BUGLE_POST_FORMAT( tags, message, ... )
    #define BUGLE_POST_FORMAT_NOTAG( message, ... )
#endif

#ifdef BUGLE_LOWERCASE_DEFINES
    #define bglp BUGLE_POST
    #define bglf BUGLE_POST_FORMAT
    #define bgln BUGLE_POST_FORMAT_NOTAG
#endif