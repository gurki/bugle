#pragma once

#define PO bugle::PostOffice::instance()

#ifdef BUGLE_ENABLE
    #define POST( letter, tags, attributes ) bugle::PostOffice::instance().post( BUGLE_INFO_VALUES, letter, tags, attributes )
    #define ENVELOPE( title ) bugle::Envelope __envelope( bugle::PostOffice::instance(), title )
    #define ENVELOPE_CLOSE() __envelope.close()
#else
    #define POST( letter, tags, attributes )
    #define ENVELOPE( title )
    #define ENVELOPE_CLOSE()
#endif
