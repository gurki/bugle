// #pragma once

// #include "observer.h"
// #include "formatter.h"

// #include <QFile>
// #include <QTextStream>


// class HtmlLogger : public QObject, public Recipient
// {
//     Q_OBJECT

//     public:

//         HtmlLogger( QObject* parent = nullptr );
//         ~HtmlLogger();

//         void createDefaultFile( const bool open = false );
//         void createFile( const QString& name, const bool open = false );

//         void setFormatter( Formatter* formatter );

//     public slots:

//         virtual void receive( const Letter& message );

//     private:

//         void writeHeader();
//         void writeSystemInfo();
//         void writeSessionInfo();
//         void writeTitle();
//         void writeDiv( const QString& key, const QString& value );

//         QFile fout_;
//         QTextStream stream_;
//         Formatter* formatter_ = nullptr;
// };
