#pragma once

#include <QDataStream>

#include "starfile.h"
#include "stardata.h"

class StarDatabase
{
public:
    struct Properties
    {
        qint16 faintmag;
        quint8 htm_level;
        quint16 t_MSpT;

        friend QDataStream &operator>>(QDataStream &stream, Properties &d);
        friend QDataStream &operator<<(QDataStream &stream, const Properties &d);
    };

public:
    void setNamedStarsPath(const QString &path);
    void setStarsNamesPath(const QString &path);

public:
    void clear();

    bool load();
    bool save();

public:
    QString namedStarsPath;
    QString starsNamesPath;

    StarFile namedStars;
    StarFile starsNames;
    Properties properties;

    QList<StarData> starDatas;
};
