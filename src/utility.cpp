#include "messagecenter/utility.h"


namespace mc {


//////////////////////////////////////////////////////////////////////////////////
//QVariant parseValue( const QString& value ) {
//    return parseVariant( QVariant( value ) );
//}


//////////////////////////////////////////////////////////////////////////////////
//QVariant parseVariant( const QVariant& variant )
//{
//    const auto types = {
//        QVariant::Int,
//        QVariant::Double,
//        QVariant::Date,
//        QVariant::Time,
//        QVariant::String,
//        variant.type()
//    };

//    for ( const auto type : types )
//    {
//        //  NOTE(tgurdan): need to create temporary copy, because
//        //    a) canConvert() does not capture all cases correctly, as it
//        //       only checks the captured type() for compatibility, not
//        //       the content
//        //    b) convert() set the variable to default constructed type()
//        //       if conversion fails. this would prevent subsequent
//        //       conversions from working
//        QVariant temp = variant;

//        if ( temp.convert( type ) ) {
//            return temp;
//        }
//    }

//    return QVariant();
//}


}   //  mc::
