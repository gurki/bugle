#pragma once

#include <QVariant>
#include <QMap>
#include <functional>


class FilterItem
{
    public:

        FilterItem();

        bool parse( QString plaintext );
        bool compare( const QVariant& target ) const;

        bool isValid() const { return valid_; }
        bool isEmpty() const { return key_.isEmpty(); }
        bool hasValue() const;
        uint hash() const;
        const QString& key() const { return key_; }
        const QVariant& value() const { return value_; }

    private:

        typedef std::function< bool( const QVariant&, const QVariant& ) > Comparator;

        static Comparator comparatorForString( const QString& string );

        bool valid_ = false;
        QString key_;
        QVariant value_;
        Comparator comparator_;
};


inline uint qHash( const FilterItem& item ) {
    return item.hash();
}

inline bool qMapLessThanKey( const FilterItem& key1, const FilterItem& key2 )  {
    return qHash( key1 ) < qHash( key2 );
}
