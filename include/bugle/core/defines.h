#pragma once

#define PO bugle::PostOffice::instance()

#ifdef BUGLE_ENABLE
    #define ENV( title ) bugle::Envelope __envelope( bugle::PostOffice::instance() )
    #define ENV_N( title ) bugle::Envelope __envelope( bugle::PostOffice::instance(), title )
    #define ENV_T( tags ) bugle::Envelope __envelope( bugle::PostOffice::instance(), {}, tags )
    #define ENV_NT( title, tags ) bugle::Envelope __envelope( bugle::PostOffice::instance(), title, tags )
    #define ENV_C() __envelope.close()
#else
    #define ENV()
    #define ENV_N( title )
    #define ENV_T( tags )
    #define ENV_NT( title, tags )
    #define ENV_C()
#endif
