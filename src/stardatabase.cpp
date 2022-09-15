#include "stardatabase.h"
#include "qt_stream.h"
#include <QFile>

QDataStream &operator>>(QDataStream &stream, StarDatabase::Properties &d)
{
    return stream >> d.faintmag >> d.htm_level >> d.t_MSpT;
}

QDataStream &operator<<(QDataStream &stream, const StarDatabase::Properties &d)
{
    return stream << d.faintmag << d.htm_level << d.t_MSpT;
}

QTextStream &operator<<(QTextStream &stream, const StarDatabase::Properties &d)
{
    return stream << endlw << "#" << d.faintmag << d.htm_level << d.t_MSpT << endlw;
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
        StarData star;
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

bool StarDatabase::saveToCsv(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly))
    {
        return false;
    }

    QTextStream stream(&file);

    stream << qSetFieldWidth(16) << Qt::left;
    stream << namedStars << properties;

    stream << "# RA" << "Dec" << "dRA" << "dDec" << "parallax" << "HD" << "mag" << "bv_index" << "spec_type" << "flags" << "name" << endlw;
    for (const auto &it: starDatas)
    {
        stream << it;
    }

    return true;
}

void StarDatabase::dump()
{
    //
}
