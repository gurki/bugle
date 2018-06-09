#include "messagelist.h"
#include <QDebug>


////////////////////////////////////////////////////////////////////////////////
MessageList::MessageList( const int maxRingSize, QObject* parent ) :
    maxRingSize_( maxRingSize ),
    QAbstractListModel( parent )
{
    formatter_ = new MessageFormatter( this );

    if ( maxRingSize_ > 0 ) {
        messages_.resize( maxRingSize_ );
    }
}


////////////////////////////////////////////////////////////////////////////////
void MessageList::setFormatter( MessageFormatter* formatter )
{
    if ( formatter == formatter_ ) {
        return;
    }

    formatter_ = formatter;
}


////////////////////////////////////////////////////////////////////////////////
int MessageList::rowCount( const QModelIndex& parent ) const {
    Q_UNUSED( parent );
    return maxRingSize_ > 0 ? ringSize_ : messages_.size();
}


////////////////////////////////////////////////////////////////////////////////
QVariant MessageList::data( const QModelIndex& index, int role ) const
{
    const int row = index.row();

    if ( row < 0 || row >= rowCount() || rowCount() == 0 ) {
        return QVariant();
    }

    const int id = (
        maxRingSize_ > 0 ?
        ( ringIndex_ + row ) % maxRingSize_ :   //  read ring as LIFO
        row                                     //  read vector as LIFO
    );

    const Message& msg = messages_.at( id );

    switch ( role ) {
        case PrettySummaryRole: return formatter_->format( msg );
        case RichTagsRole: return msg.tags();
        case TimestampRole: return msg.timestamp();
    }

    return QVariant();
}


////////////////////////////////////////////////////////////////////////////////
QHash<int, QByteArray> MessageList::roleNames() const
{
    QHash<int, QByteArray> names;
    names[ PrettySummaryRole ] = "prettySummary";
    names[ RichTagsRole ] = "tags";
    names[ TimestampRole ] = "timestamp";

    return names;
}


////////////////////////////////////////////////////////////////////////////////
void MessageList::notify( const Message& message )
{
    if ( maxRingSize_ <= 0 )
    {
        //  push back
        beginInsertRows( QModelIndex(), rowCount(), rowCount() );
        messages_.push_back( message );
        endInsertRows();

        emit sizeChanged();
        emit hasBeenNotified();
        return;
    }

    if ( ringSize_ == maxRingSize_ )
    {
        //  remove ring front
        beginRemoveRows( QModelIndex(), 0, 0 );
        endRemoveRows();

        //  rotate and set ring back
        const int id = maxRingSize_ - 1;
        beginInsertRows( QModelIndex(), id, id );
        messages_[ ringIndex_ ] = message;
        ringIndex_ = ( ringIndex_ + 1 ) % maxRingSize_;
        endInsertRows();

        emit hasBeenNotified();
        emit fullRingBufferChanged();
        return;
    }

    //  push ring back
    const int id = ( ringIndex_ + ringSize_ ) % maxRingSize_;

    beginInsertRows( QModelIndex(), ringSize_, ringSize_ );
    messages_[ id ] = message;
    endInsertRows();

    ringSize_++;

    emit sizeChanged();
    emit hasBeenNotified();
}


////////////////////////////////////////////////////////////////////////////////
nlohmann::json to_json();


////////////////////////////////////////////////////////////////////////////////
void from_json( const nlohmann::json& json );
