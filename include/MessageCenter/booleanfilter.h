// #pragma once

// #include "filteritem.h"

// #include <QVector>
// #include <QVariantMap>


// //  some distant takeaways in the first google result [1]
// //  [1] http://booleanblackbelt.com/2008/12/basic-boolean-search-operators-and-query-modifiers-explained/


// typedef QMap<FilterItem, bool> FilterConjunction;
// typedef QVector<FilterConjunction> FilterDisjunction;


// ////////////////////////////////////////////////////////////////////////////////
// //  disjunctive normal form of
// class BooleanFilter : public QObject
// {
//     Q_OBJECT

//     public:

//         BooleanFilter( QObject* parent = nullptr );

//         bool set( QString plaintext );
//         bool unite( QString line );
//         bool passes( const QVariantMap& set ) const;

//         const int size() const { return normalForm_.size(); }
//         bool isEmpty() const { return normalForm_.isEmpty(); }
//         const QString& plaintext() const { return plaintext_; }

//         static bool passes(
//             const QVariantMap& set,
//             const FilterItem& item,
//             const bool negate
//         );
//         static bool isSubset(
//             const FilterConjunction& conjunct,
//             const QVariantMap& set
//         );
//         static bool containsSubset(
//             const FilterDisjunction& disjunct,
//             const QVariantMap& set
//         );

//     signals:

//         void changed() const;

//     private:

//         FilterDisjunction parseLine( const QString& plaintext );

//         FilterDisjunction normalForm_;    //  or -> and -> (tag, not)
//         QString plaintext_;
// };
