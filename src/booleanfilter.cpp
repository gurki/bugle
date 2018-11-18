#include "messagecenter/booleanfilter.h"


////////////////////////////////////////////////////////////////////////////////
BooleanFilter::BooleanFilter( const std::string& plaintext ) {
    set( plaintext );
}


////////////////////////////////////////////////////////////////////////////////
bool BooleanFilter::set( const std::string& plaintext )
{
    // //  convert linebreaks to conjunctions

    // plaintext.replace( '\n', ',' );

    // if ( plaintext == plaintext_ ) {
    //     return true;
    // }

    // //  parse line
    // const FilterDisjunction disjunction = parseLine( plaintext );

    // //  invalid line
    // if ( disjunction.isEmpty() && ! plaintext.isEmpty() ) {
    //     return false;
    // }

    // //  set
    // if ( normalForm_ == disjunction ) {
    //     return true;
    // }

    // plaintext_ = plaintext;
    // normalForm_ = disjunction;

    return true;
}


////////////////////////////////////////////////////////////////////////////////
bool BooleanFilter::unite( const std::string& line )
{
    // if ( line.isEmpty() ) {
    //     return true;
    // }

    // const FilterDisjunction disjunction = parseLine( line );

    // if ( disjunction.isEmpty() && ! line.isEmpty() ) {
    //     return false;
    // }

    // plaintext_ += "," + line;
    // normalForm_.append( disjunction );

    return true;
}


////////////////////////////////////////////////////////////////////////////////
FilterDisjunction BooleanFilter::parseLine( const std::string& line )
{
    // //  split into conjuncts
    // const auto disjuncts = line.split( ',' );

    FilterDisjunction disjunction;

    // for ( const auto& disjunct : disjuncts )
    // {
    //     FilterConjunction conjunction;

    //     if ( disjunct.isEmpty() ) {
    //         continue;
    //     }

    //     //  split expression into and-items
    //     const auto conjuncts = disjunct.split( QRegExp("\\s+"), QString::SkipEmptyParts );

    //     for ( const auto& conjunct : conjuncts )
    //     {
    //         const bool negate = conjunct[0] == '!';
    //         const std::string variable = negate ? conjunct.mid( 1 ) : conjunct;

    //         FilterItem item;
    //         const bool valid = item.parse( variable );

    //         if ( valid ) {
    //             conjunction[ item ] = ! negate;
    //         }
    //     }

    //     //  add valid option
    //     if ( ! conjunction.isEmpty() ) {
    //         disjunction.push_back( conjunction );
    //     }
    // }

    return disjunction;
}


////////////////////////////////////////////////////////////////////////////////
bool BooleanFilter::passes( const nlohmann::json& set ) const {
    return containsSubset( normalForm_, set );
}


////////////////////////////////////////////////////////////////////////////////
bool BooleanFilter::containsSubset(
    const FilterDisjunction& disjunct,
    const nlohmann::json& set )
{
    if ( disjunct.empty() ) {
        return true;
    }

    for ( const auto& conjunct : disjunct ) {
        if ( isSubset( conjunct, set ) ) {
            return true;
        }
    }

    return false;
}


////////////////////////////////////////////////////////////////////////////////
bool BooleanFilter::isSubset(
    const FilterConjunction& conjunct,
    const nlohmann::json& set )
{
    for ( const auto& item : conjunct ) {
        if ( ! passes( item, set ) ) {
            return false;
        }
    }

    return true;
}


////////////////////////////////////////////////////////////////////////////////
bool BooleanFilter::passes(
    const FilterItem& item,
    const nlohmann::json& set )
{
    // const std::string& key = item.key();
    // bool passes = set.contains( key );  //  default for simple tag

    // //  rich tag option
    // if ( item.hasValue() )
    // {
    //     if ( ! passes ) {
    //         return false;
    //     }

    //     const nlohmann::json& value = set.value( key );
    //     passes = item.compare( value );
    // }

    // return ( passes == negate );
    return false;
}
