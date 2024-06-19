#include "bugle/filter/tagfilter.h"
#include "bugle/core/letter.h"

#include <print>

namespace bugle {


////////////////////////////////////////////////////////////////////////////////
TagFilter::TagFilter(
    const std::vector<std::string>& include,
    const std::vector<std::string>& exclude )
{
    for ( const auto& tag : include ) {
        tags[ tag ] = true;
    }

    for ( const auto& tag : exclude ) {
        tags[ tag ] = false;
    }
}


////////////////////////////////////////////////////////////////////////////////
bool TagFilter::matches( const Letter& letter ) const
{
    for ( const auto& [ tag, include ] : tags ) {
        if ( letter.tags.contains( tag ) != include ) {
            return false;
        }
    }

    return true;
}


}   //  ::bugle