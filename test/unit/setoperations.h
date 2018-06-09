#ifndef SET_OPERATIONS_H
#define SET_OPERATIONS_H

#include "messagecenter.h"
#include <QDebug>


class SetOperations
{
    void test()
    {
        QVariantMap m1 = {{ "1", "a" }, { "2", QVariant() }};
        QVariantMap m2 = {{ "1", "a" }};
        QVariantMap m3;
        QVariantMap m4 = {{ "1", "a" }, { "2", QVariant() }, { "d", 3 }};

        qDebug() << MessageCenter::isSubset( m1, m2 );  //  true
        qDebug() << MessageCenter::isSubset( m1, m3 );  //  true
        qDebug() << MessageCenter::isSubset( m2, m3 );  //  true
        qDebug() << MessageCenter::isSubset( m2, m1 );  //  false
        qDebug() << MessageCenter::isSubset( m3, m1 );  //  false
        qDebug() << MessageCenter::isSubset( m3, m2 );  //  false
        qDebug() << MessageCenter::anySubset( m1, { m2, m4 } );  //  true
        qDebug() << MessageCenter::anySubset( m2, { m1, m4 } );  //  false
    }
};


#endif  //  SET_OPERATIONS_H
