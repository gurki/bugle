#pragma once

#include "messagelist.h"
#include "booleanfilter.h"

#include <QSet>
#include <QSortFilterProxyModel>


class MessageListProxy : public QSortFilterProxyModel
{
    Q_OBJECT

    Q_PROPERTY( QString tagFilter READ tagFilterString WRITE setTagFilterString NOTIFY filterChanged )

    public:

        MessageListProxy( QObject* parent = nullptr );

        virtual void invalidateFilter();

        void setTagFilterString( QString plaintext );

        const QString& tagFilterString() const { return tagFilter_.plaintext(); }

    signals:

        void filterChanged() const;

    protected:

        bool filterAcceptsRow(
            int sourceRow,
            const QModelIndex& sourceParent
        ) const override;

        bool lessThan(
            const QModelIndex& left,
            const QModelIndex& right
        ) const override;

    private:

        BooleanFilter tagFilter_;
};
