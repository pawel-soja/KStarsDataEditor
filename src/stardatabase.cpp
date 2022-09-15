#include "stardatabase.h"
#include <QFile>
#include <QDebug>

QDataStream &operator>>(QDataStream &stream, StarDatabase::Properties &d)
{
    return stream >> d.faintmag >> d.htm_level >> d.t_MSpT;
}

QDataStream &operator<<(QDataStream &stream, const StarDatabase::Properties &d)
{
    return stream << d.faintmag << d.htm_level << d.t_MSpT;
}

void StarDatabase::setNamedStarsPath(const QString &path)
{
    namedStarsPath = path;
}

void StarDatabase::setStarsNamesPath(const QString &path)
{
    starsNamesPath = path;
}

void StarDatabase::clear()
{
    //*this = StarDatabase();
    starDatas.clear();
}

bool StarDatabase::load()
{
    QFile namedStarsFile(namedStarsPath);
    QFile starsNamesFile(starsNamesPath);

    if (!namedStarsFile.open(QFile::ReadOnly))
    {
        qWarning() << "Could not open data file" << namedStarsPath;
        return false;
    }

    if (!starsNamesFile.open(QFile::ReadOnly))
    {
        qWarning() << "Could not open data file" << starsNamesPath;
        return false;
    }

    QDataStream namedStarsStream(&namedStarsFile);
    QDataStream starsNamesStream(&starsNamesFile);

    namedStarsStream >> namedStars >> properties;
    starsNamesStream >> starsNames;

    while(!namedStarsStream.atEnd())
    {
        StarData star(this);
        namedStarsStream >> star;
        if (star.isNamedStar())
        {
            starsNamesStream >> star.rawName();
        }

        starDatas.append(star);
    }

    return true;
}

bool StarDatabase::save()
{
    QFile namedStarsFile(namedStarsPath);
    QFile starsNamesFile(starsNamesPath);

    if (!namedStarsFile.open(QFile::WriteOnly))
    {
        qWarning() << "Could not open data file" << namedStarsPath;
        return false;
    }

    if (!starsNamesFile.open(QFile::WriteOnly))
    {
        qWarning() << "Could not open data file" << starsNamesPath;
        return false;
    }

    QDataStream namedStarsStream(&namedStarsFile);
    QDataStream starsNamesStream(&starsNamesFile);

    namedStarsStream << namedStars << properties;
    starsNamesStream << starsNames;

    for (const auto &it: starDatas)
    {
        namedStarsStream << it;
        starsNamesStream << it.rawName();
    }

    return true;
}
