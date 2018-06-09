#include "filteritem.h"
#include "messageutility.h"

#include <QDebug>
#include <QRegularExpression>
#include <QDateTime>


////////////////////////////////////////////////////////////////////////////////
FilterItem::FilterItem()
{}


////////////////////////////////////////////////////////////////////////////////
bool FilterItem::compare( const QVariant& target ) const {
    return comparator_( target, value_ );
}


////////////////////////////////////////////////////////////////////////////////
bool FilterItem::hasValue() const {
    return ! value_.isNull() && value_.isValid();
}


////////////////////////////////////////////////////////////////////////////////
bool FilterItem::parse( QString plaintext )
{
    static const QString validOperators = "(>=|<=|!=|=|>|<)";
    static const QString validSymbols = "\\w|\\.|-|_|:";
    static const QString validNames = "((?:" + validSymbols + ")+)";
    static const QString validValues = "(?:(?:" + validOperators + validNames + ")?)";
    static const QRegularExpression re( validNames + validValues );

    const QRegularExpressionMatch match = re.match( plaintext );

    if ( ! match.hasMatch() ) {
        valid_ = false;
        return false;
    }

    //  valid match => non-empty key, optional ( non-empty operator and non-empty value )

    valid_ = true;
    key_ = match.captured( 1 );

    const QString valueStr = match.captured( 3 );
    const QString op = match.captured( 2 );

    //  early return for key-only
    if ( op.isEmpty() ) {
        return true;
    }

    value_ = mc::parseValue( valueStr );
    comparator_ = comparatorForString( op );

    return true;
}


////////////////////////////////////////////////////////////////////////////////
uint FilterItem::hash() const
{
    return (
        qHash( key_ ) ^
        qHash( value_.toString() ) ^
        *( reinterpret_cast<const uint8_t*>( &comparator_ ) )
    );
}


////////////////////////////////////////////////////////////////////////////////
FilterItem::Comparator FilterItem::comparatorForString( const QString& string )
{
    if ( string == ">=" ) {
        return std::greater_equal<QVariant>();
    } else if ( string == "<=" ) {
        return std::less_equal<QVariant>();
    } else if ( string == "!=" ) {
        return std::not_equal_to<QVariant>();
    } else if ( string == ">" ) {
        return std::greater<QVariant>();
    } else if ( string == "<" ) {
        return std::less<QVariant>();
    } else if ( string == "=" ) {
        return std::equal_to<QVariant>();
    } else {
        qCritical() << "this should never be reached";
        return Comparator();
    }
}
