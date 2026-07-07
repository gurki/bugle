#include "bugle/core/filter.h"
#include "bugle/filter/lines.h"
#include "bugle/filter/address.h"
#include "bugle/filter/route.h"
#include "bugle/filter/parse.h"

namespace bugle {


////////////////////////////////////////////////////////////////////////////////
static std::optional<ValueFilter::comp_t> comparatorFromString( std::string_view op )
{
    using json = nlohmann::json;

    if ( op == "=" || op == "==" ) return std::equal_to<>{};
    if ( op == "!=" ) return std::not_equal_to<>{};
    if ( op == ">" ) return []( const json& a, const json& b ){ return a > b; };
    if ( op == ">=" ) return []( const json& a, const json& b ){ return a >= b; };
    if ( op == "<" ) return []( const json& a, const json& b ){ return a < b; };
    if ( op == "<=" ) return []( const json& a, const json& b ){ return a <= b; };

    return {};
}


////////////////////////////////////////////////////////////////////////////////
static std::optional<Filter> buildValueFilter( const Literal& literal )
{
    const auto compare = comparatorFromString( literal.op );

    if ( ! compare ) {
        return {};
    }

    //  operands parse as json where possible ("100" -> 100, "true" -> true),
    //  and fall back to plain strings otherwise ("core" -> "core")
    nlohmann::json value = nlohmann::json::parse( literal.value, nullptr, false );

    if ( value.is_discarded() ) {
        value = std::string( literal.value );
    }

    return ValueFilter( std::string( literal.variable ), value, compare.value() );
}


////////////////////////////////////////////////////////////////////////////////
static std::optional<Line> buildLine( const Literal& literal )
{
    Filter filter;
    const std::string variable = std::string( literal.variable );
    const VariableType type = typeFromString( literal.type );

    switch ( type )
    {
        case VariableType::Tag: filter = TagFilter( variable ); break;
        case VariableType::Message: filter = MessageFilter( variable ); break;
        case VariableType::File: filter = FileFilter( variable ); break;
        case VariableType::Function: filter = FunctionFilter( variable ); break;

        case VariableType::Attribute:
        case VariableType::Value:
        {
            if ( literal.op.empty() ) {
                filter = AttributeFilter( variable );
                break;
            }

            const auto maybeFilter = buildValueFilter( literal );

            if ( ! maybeFilter ) {
                return {};
            }

            filter = maybeFilter.value();
            break;
        }

        case VariableType::Timestamp:
        case VariableType::Line:
        case VariableType::Invalid:
            return {};
    }

    if ( ! filter.matches ) {
        return {};
    }

    return Line{ filter, literal.negate };
}


////////////////////////////////////////////////////////////////////////////////
static std::optional<Address> buildAddress( auto conjunction )
{
    std::vector<Line> lines;

    for ( const Literal& literal : conjunction )
    {
        const auto maybeLine = buildLine( literal );

        if ( ! maybeLine ) {
            continue;
        }

        lines.push_back( maybeLine.value() );
    }

    if ( lines.empty() ) {
        return {};
    }

    return Address{ std::move( lines ) };
}


////////////////////////////////////////////////////////////////////////////////
static std::optional<Route> buildRoute( auto disjunction )
{
    std::vector<Filter> addresses;

    for ( auto conjunction : disjunction )
    {
        const auto maybeAddress = buildAddress( conjunction );

        if ( ! maybeAddress ) {
            continue;
        }

        addresses.push_back( maybeAddress.value() );
    }

    if ( addresses.empty() ) {
        return {};
    }

    return Route( std::move( addresses ) );
}


////////////////////////////////////////////////////////////////////////////////
Filter Filter::fromString( std::string_view expression ) {
    auto disjunction = parseDisjunction( expression );
    const auto maybeRoute = buildRoute( disjunction );
    if ( ! maybeRoute ) return {};
    return maybeRoute.value();
}


}   //  ::bugle