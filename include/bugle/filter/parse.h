#pragma once

#include <string_view>
#include <ranges>
#include <algorithm>
#include <cctype>

namespace bugle {


struct Literal {
    std::string_view type;
    std::string_view variable;
    bool negate;
};


////////////////////////////////////////////////////////////////////////////////
static constexpr auto toStringView = []( const auto& expression ) constexpr {
    return std::string_view( expression );
};


////////////////////////////////////////////////////////////////////////////////
static constexpr auto notIsSpace = []( const auto& c ) {
    return ! std::isspace( c );
};


////////////////////////////////////////////////////////////////////////////////
static constexpr auto notIsEmpty = []( std::string_view expression ) constexpr {
    return std::ranges::any_of( expression, notIsSpace );
};


////////////////////////////////////////////////////////////////////////////////
static constexpr std::string_view trim( std::string_view expression )
{
    const size_t from = expression.find_first_not_of( ' ' );
    const size_t to = expression.find_last_not_of( ' ' );

    //  only spaces
    if ( from >= expression.size() ) {
        return {};
    }

    return expression.substr( from, to - from + 1 );
}


////////////////////////////////////////////////////////////////////////////////
static constexpr Literal parseLiteral( std::string_view expression )
{
    const size_t id = expression.find_first_of( ':' );
    std::string_view type = expression.substr( 0, id );
    std::string_view variable = expression.substr( id + 1 );

    bool negate = false;

    if ( type.starts_with( '!' ) ) {
        negate = true;
        type = type.substr( 1 );
    }

    return { type, variable, negate };
}


////////////////////////////////////////////////////////////////////////////////
static constexpr auto parseConjunction( std::string_view expression ) {
    auto literals = expression | std::views::split( ' ' );
    return literals |
        std::views::transform( toStringView ) |
        std::views::filter( notIsEmpty ) |
        std::views::transform( parseLiteral );
}


////////////////////////////////////////////////////////////////////////////////
static constexpr auto parseDisjunction( std::string_view expression ) {
    auto conjunctions = expression | std::views::split( '\n' );
    return conjunctions |
        std::views::transform( toStringView ) |
        std::views::transform( trim ) |
        std::views::filter( notIsEmpty ) |
        std::views::transform( parseConjunction );
}


}   //  ::bugle