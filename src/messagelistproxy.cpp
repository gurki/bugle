#include "messagelistproxy.h"
#include <QStringList>
#include <QDebug>


////////////////////////////////////////////////////////////////////////////////
MessageListProxy::MessageListProxy( QObject* parent ) :
    QSortFilterProxyModel( parent )
{
    setFilterRole( MessageList::RichTagsRole );
    setSortRole( MessageList::TimestampRole );

    QObject::connect(
        &tagFilter_, &BooleanFilter::changed,
        this, &MessageListProxy::invalidateFilter
    );
}


////////////////////////////////////////////////////////////////////////////////
void MessageListProxy::setTagFilterString( QString plaintext ) {
    tagFilter_.set( plaintext );
}


////////////////////////////////////////////////////////////////////////////////
void MessageListProxy::invalidateFilter() {
    QSortFilterProxyModel::invalidateFilter();
}


////////////////////////////////////////////////////////////////////////////////
bool MessageListProxy::filterAcceptsRow(
    int sourceRow,
    const QModelIndex& sourceParent ) const
{
    if ( tagFilter_.isEmpty() ) {
        return true;
    }

    const QModelIndex index = sourceModel()->index( sourceRow, 0, sourceParent );
    const QVariantMap& tags = sourceModel()->data( index, filterRole() ).toMap();

    return tagFilter_.passes( tags );
}


////////////////////////////////////////////////////////////////////////////////
bool MessageListProxy::lessThan(
    const QModelIndex& left,
    const QModelIndex& right ) const
{
    return left < right;
}
