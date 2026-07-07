#pragma once

#include <string_view>
#include <ranges>
#include <algorithm>
#include <cctype>
#include <vector>

namespace bugle {


enum class VariableType {
    Invalid,
    Timestamp,
    Message,
    Tag,
    File,
    Line,
    Function,
    Attribute,
    Value
};


struct Literal {
    std::string_view type;
    std::string_view variable;
    std::string_view op;        //  optional comparison operator, e.g. ">" in "attribute:value>100"
    std::string_view value;     //  optional comparison operand
    bool negate;
};


using Conjunction = std::vector<Literal>;
using Disjunction = std::vector<Conjunction>;


////////////////////////////////////////////////////////////////////////////////
static constexpr auto toStringView = []( const auto& expression ) constexpr {
    return std::string_view( expression );
};


////////////////////////////////////////////////////////////////////////////////
static constexpr auto notIsSpace = []( const auto& c ) {
    return ! std::isspace( static_cast<unsigned char>( c ) );
};


////////////////////////////////////////////////////////////////////////////////
static constexpr auto notIsEmpty = []( std::string_view expression ) constexpr {
    return std::ranges::any_of( expression, notIsSpace );
};


////////////////////////////////////////////////////////////////////////////////
constexpr std::string_view trim( std::string_view expression )
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
constexpr VariableType typeFromString( std::string_view type )
{
    if ( type == "timestamp" ) return VariableType::Timestamp;
    if ( type == "message" ) return VariableType::Message;
    if ( type == "tag" ) return VariableType::Tag;
    if ( type == "file" ) return VariableType::File;
    if ( type == "function" ) return VariableType::Function;
    if ( type == "attribute" ) return VariableType::Attribute;
    if ( type == "value" ) return VariableType::Value;

    return VariableType::Invalid;
}

////////////////////////////////////////////////////////////////////////////////
constexpr Literal parseLiteral( std::string_view expression )
{
    const size_t id = expression.find_first_of( ':' );

    if ( id == std::string_view::npos ) {
        return {};  //  empty type parses as VariableType::Invalid
    }

    std::string_view type = expression.substr( 0, id );
    std::string_view variable = expression.substr( id + 1 );

    bool negate = false;

    if ( type.starts_with( '!' ) ) {
        negate = true;
        type = type.substr( 1 );
    }

    //  optional comparison, e.g. "value>100", "count>=2", "name=core", "state!=idle"
    std::string_view op {};
    std::string_view value {};
    const size_t opId = variable.find_first_of( "<>=!" );

    if ( opId != std::string_view::npos )
    {
        const bool wide = ( opId + 1 < variable.size() ) && ( variable[ opId + 1 ] == '=' );
        const size_t opSize = wide ? 2 : 1;

        op = variable.substr( opId, opSize );
        value = variable.substr( opId + opSize );
        variable = variable.substr( 0, opId );
    }

    return { type, variable, op, value, negate };
}


////////////////////////////////////////////////////////////////////////////////
constexpr auto parseConjunction( std::string_view expression ) {
    auto literals = expression | std::views::split( ' ' );
    return literals |
        std::views::transform( toStringView ) |
        std::views::filter( notIsEmpty ) |
        std::views::transform( parseLiteral );
}


////////////////////////////////////////////////////////////////////////////////
constexpr auto parseDisjunction( std::string_view expression ) {
    auto conjunctions = expression | std::views::split( '\n' );
    return conjunctions |
        std::views::transform( toStringView ) |
        std::views::transform( trim ) |
        std::views::filter( notIsEmpty ) |
        std::views::transform( parseConjunction );
}


}   //  ::bugle