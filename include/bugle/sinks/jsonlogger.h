// #pragma once

// #include "observer.h"
// #include <QFile>


// class JsonLogger : public QObject, public Recipient
// {
//     Q_OBJECT

//     public:

//         explicit JsonLogger( QObject* parent = nullptr );

//         bool open( const QString& filename );
//         void createDefaultFile();

//     public slots:

//         virtual void receive( const Letter& message );

//     private:

//         QFile file_;
// };
