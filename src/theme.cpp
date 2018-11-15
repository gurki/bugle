#include "messagecenter/theme.h"


namespace mc {


////////////////////////////////////////////////////////////////////////////////
Theme::Theme() :
    colors_({
        { "info", { 39, 45 } },
        { "success", { 40, 46 } },
        { "warning", { 220, 226 } },
        { "error", { 196, 202 } },
        { "debug", { 159, 195 } }
    })
{}


////////////////////////////////////////////////////////////////////////////////
const IndexPair& Theme::colors( const std::string& key ) const
{
    if ( colors_.find( key ) == colors_.end() ) {
        return textColors();
    }

    return colors_.at( key );
}


}   //  mc::
