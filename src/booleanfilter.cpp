#include "booleanfilter.h"

#include <QDebug>
#include <QDateTime>


////////////////////////////////////////////////////////////////////////////////
BooleanFilter::BooleanFilter( QObject* parent ) :
    QObject( parent )
{}


////////////////////////////////////////////////////////////////////////////////
bool BooleanFilter::set( QString plaintext )
{
    //  convert linebreaks to conjunctions

    plaintext.replace( '\n', ',' );

    if ( plaintext == plaintext_ ) {
        return true;
    }

    //  parse line
    const FilterDisjunction disjunction = parseLine( plaintext );

    //  invalid line
    if ( disjunction.isEmpty() && ! plaintext.isEmpty() ) {
        return false;
    }

    //  set
    if ( normalForm_ == disjunction ) {
        return true;
    }

    plaintext_ = plaintext;
    normalForm_ = disjunction;
    emit changed();

    return true;
}


////////////////////////////////////////////////////////////////////////////////
bool BooleanFilter::unite( QString line )
{
    if ( line.isEmpty() ) {
        return true;
    }

    const FilterDisjunction disjunction = parseLine( line );

    if ( disjunction.isEmpty() && ! line.isEmpty() ) {
        return false;
    }

    plaintext_ += "," + line;
    normalForm_.append( disjunction );
    emit changed();

    return true;
}


////////////////////////////////////////////////////////////////////////////////
FilterDisjunction BooleanFilter::parseLine( const QString& line )
{
    //  split into conjuncts
    const auto disjuncts = line.split( ',' );

    FilterDisjunction disjunction;

    for ( const auto& disjunct : disjuncts )
    {
        FilterConjunction conjunction;

        if ( disjunct.isEmpty() ) {
            continue;
        }

        //  split expression into and-items
        const auto conjuncts = disjunct.split( QRegExp("\\s+"), QString::SkipEmptyParts );

        for ( const auto& conjunct : conjuncts )
        {
            const bool negate = conjunct[0] == '!';
            const QString variable = negate ? conjunct.mid( 1 ) : conjunct;

            FilterItem item;
            const bool valid = item.parse( variable );

            if ( valid ) {
                conjunction[ item ] = ! negate;
            }
        }

        //  add valid option
        if ( ! conjunction.isEmpty() ) {
            disjunction.push_back( conjunction );
        }
    }

    return disjunction;
}


////////////////////////////////////////////////////////////////////////////////
bool BooleanFilter::passes( const QVariantMap& set ) const {
    return containsSubset( normalForm_, set );
}


////////////////////////////////////////////////////////////////////////////////
bool BooleanFilter::containsSubset(
    const FilterDisjunction& disjunct,
    const QVariantMap& set )
{
    if ( disjunct.isEmpty() ) {
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
    const QVariantMap& set )
{
    QMapIterator<FilterItem, bool> iter( conjunct );

    while ( iter.hasNext() )
    {
        const auto& item = iter.next();
        const FilterItem& filter = item.key();
        const bool negate = item.value();

        if ( ! passes( set, filter, negate ) ) {
            return false;
        }
    }

    return true;
}


////////////////////////////////////////////////////////////////////////////////
bool BooleanFilter::passes(
    const QVariantMap& set,
    const FilterItem& item,
    const bool negate )
{
    const QString& key = item.key();
    bool passes = set.contains( key );  //  default for simple tag

    //  rich tag option
    if ( item.hasValue() )
    {
        if ( ! passes ) {
            return false;
        }

        const QVariant& value = set.value( key );
        passes = item.compare( value );
    }

    return ( passes == negate );
}
