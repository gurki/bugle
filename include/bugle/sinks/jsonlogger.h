// #pragma once

// #include "observer.h"
// #include <QFile>


// class JsonLogger : public QObject, public Observer
// {
//     Q_OBJECT

//     public:

//         explicit JsonLogger( QObject* parent = nullptr );

//         bool open( const QString& filename );
//         void createDefaultFile();

//     public slots:

//         virtual void notify( const Letter& message );

//     private:

//         QFile file_;
// };
