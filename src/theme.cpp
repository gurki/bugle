#include "messagecenter/theme.h"


namespace mc {


////////////////////////////////////////////////////////////////////////////////
Theme::Theme()
{}


////////////////////////////////////////////////////////////////////////////////
const IndexPair& Theme::indices( const std::string& key ) const
{
    if ( colors_.find( key ) == colors_.end() ) {
        return primaryIds();
    }

    return colors_.at( key );
}


}   //  mc::
