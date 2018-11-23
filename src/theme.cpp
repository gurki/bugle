#include "messagecenter/theme.h"
#include <iostream>
#include <string>


namespace mc {


////////////////////////////////////////////////////////////////////////////////
void Theme::clear() {
    colors_.clear();
}


////////////////////////////////////////////////////////////////////////////////
void Theme::setPrimary( const uint8_t primaryId, const uint8_t variantId ) {
    primary_ = { primaryId, variantId };
}


////////////////////////////////////////////////////////////////////////////////
void Theme::setSecondary( const uint8_t secondaryId, const uint8_t variantId ) {
    secondary_ = { secondaryId, variantId };
}

////////////////////////////////////////////////////////////////////////////////
void Theme::setColor( const std::string& key, const uint8_t colorId ) {
    colors_[ key ] = { colorId, colorId };
}


////////////////////////////////////////////////////////////////////////////////
void Theme::setColor( 
    const std::string& key, 
    const uint8_t colorId, 
    const uint8_t variantId ) 
{
    colors_[ key ] = { colorId, variantId };
}


////////////////////////////////////////////////////////////////////////////////
const IndexPair Theme::indices( const std::string& key ) const
{
    if ( colors_.find( key ) == colors_.end() ) {
        return { primary().second, secondary().first };
    }

    return colors_.at( key );
}


////////////////////////////////////////////////////////////////////////////////
const IndexPair Theme::operator[] ( const std::string& key ) const {
    return indices( key );
}


}   //  mc::
