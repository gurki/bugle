#pragma once

#include <QVariantMap>
#include <QVector>


class SystemInfo
{
    public:

        static const QMap<QString, QString>& buildInfo();
        static const QMap<QString, QString>& sessionInfo();
        static QString applicationName();
        static QString languageInfo();
        static QString compilerInfo();
        static const QVector<QString>& orderedBuildKeys();
        static const QVector<QString>& orderedSessionKeys();


    private:

        SystemInfo() {}
        ~SystemInfo() {}
};
