#include "stardata.h"
#include "stardatabase.h"

StarData::StarData(StarDatabase *db)
    : mStarDatabase(db)
{ }

QString StarData::longName() const
{
    return data.starName.longName;
}

QString StarData::bayerName() const
{
    return data.starName.bayerName;
}

StarData::Data::StarName StarData::rawName() const
{
    return data.starName;
}

StarData::Data::StarName &StarData::rawName()
{
    return data.starName;
}

bool StarData::isNamedStar() const
{
    return data.flags & 0x01;
}

bool StarData::isMultiplicity() const
{
    return data.flags & 0x02;
}

bool StarData::isVariability() const
{
    return data.flags & 0x04;
}

double StarData::ra() const
{
    return data.RA / raScale();
}

double StarData::dec() const
{
    return data.Dec / decScale();
}

double StarData::mag() const
{
    return data.mag / magScale();
}

double StarData::bvIndex() const
{
    return data.bv_index / bvIndexScale();
}

double StarData::pmRa() const
{
    return data.dRA / pmRaScale();
}

double StarData::pmDec() const
{
    return data.dDec / pmDecScale();
}

double StarData::parallax() const
{
    return data.parallax / parallaxScale();
}

QString StarData::specType() const
{
    return data.spec_type;
}

quint8 StarData::flags() const
{
    return data.flags;
}

QString StarData::toString(double value, const QVector<QChar> &symbols)
{
    QStringList result;

    int intValue = 0;
    for (const auto &it: symbols)
    {
        intValue = (int)value;
        result.append(QString::number(intValue) + it);

        value -= intValue;
        value *= 60;
    }

    result.last() = QString::number(intValue + value / 60.0) + symbols.last();

    return result.join(' ');
}

#include <QRegularExpression>


double StarData::fromString(const QString &value, const QVector<QChar> &symbols, bool *ok)
{
    double result = 0;

    QStringList exp;
    for (const auto &it: symbols) exp.append(QString("([+-]?(?:[0-9]*[.])?[0-9]+)") + it);

    const auto match = QRegularExpression("^" + exp.join(" ") + "$").match(value);

    if (ok)
    {
        *ok = match.hasMatch();
    }

    if (!match.hasMatch())
    {
        return 0;
    }

    for (int i=0, m = 1; i<exp.size(); ++i)
    {
        result += match.captured(i+1).toDouble() / m;
        m *= 60;
    }

    return result;
}

QString StarData::raAsString() const
{
    return toString(ra(), {'h', 'm', 's'});
}

QString StarData::decAsString() const
{
    return toString(dec());
}

bool StarData::setLongName(const QString &name)
{
    data.starName.longName = name;
    return true;
}

bool StarData::setBayerName(const QString &name)
{
    data.starName.bayerName = name;
    return true;
}

bool StarData::setRa(double value)
{
    data.RA = value * raScale();
    return true;
}

bool StarData::setRa(const QString &value)
{
    bool ok;
    auto v = fromString(value, {'h', 'm', 's'}, &ok);
    if (ok)
    {
        setRa(v);
    }
    return ok;
}

bool StarData::setDec(double value)
{
    data.Dec = value * decScale();
    return true;
}

bool StarData::setDec(const QString &value)
{
    bool ok;
    auto v = fromString(value, {0x00b0, '\'', '"'}, &ok);
    if (ok)
    {
        setDec(v);
    }
    return ok;
}

bool StarData::setMag(double value)
{
    data.mag = value * magScale();
    return true;
}

bool StarData::setBvIndex(double value)
{
    data.bv_index = value * bvIndexScale();
    return true;
}

bool StarData::setPmRa(double value)
{
    data.dRA = value * pmRaScale();
    return true;
}

bool StarData::setPmDec(double value)
{
    data.dDec = value * pmDecScale();
    return true;
}

bool StarData::setParallax(double value)
{
    data.parallax = value * parallaxScale();
    return true;
}

bool StarData::setSpecType(const QByteArray &value)
{
    data.spec_type = value;
    return true;
}

bool StarData::setFlags(quint8 value)
{
    data.flags = value;
    return true;
}

double StarData::raScale() const
{
    StarFile::DataElement &element = mStarDatabase->namedStars.element("RA");
    if (element.scale() == 0)
    {
        qWarning("Invalid RA scale, set to 1e6");
        element.setScale(1e6);
    }
    return element.scale();
}

double StarData::decScale() const
{
    StarFile::DataElement &element = mStarDatabase->namedStars.element("Dec");
    if (element.scale() == 0)
    {
        qWarning("Invalid Dec scale, set to 1e5");
        element.setScale(1e5);
    }
    return element.scale();
}

double StarData::magScale() const
{
    StarFile::DataElement &element = mStarDatabase->namedStars.element("mag");
    if (element.scale() == 0)
    {
        qWarning("Invalid mag scale, set to 100");
        element.setScale(100);
    }
    return element.scale();
}

double StarData::bvIndexScale() const
{
    StarFile::DataElement &element = mStarDatabase->namedStars.element("bv_index");
    if (element.scale() == 0)
    {
        qWarning("Invalid bv_index scale, set to 100");
        element.setScale(100);
    }
    return element.scale();
}

double StarData::pmRaScale() const
{
    StarFile::DataElement &element = mStarDatabase->namedStars.element("dRA");
    if (element.scale() == 0)
    {
        qWarning("Invalid dRA scale, set to 10");
        element.setScale(10);
    }
    return element.scale();
}

double StarData::pmDecScale() const
{
    StarFile::DataElement &element = mStarDatabase->namedStars.element("dDec");
    if (element.scale() == 0)
    {
        qWarning("Invalid dDec scale, set to 10");
        element.setScale(10);
    }
    return element.scale();
}

double StarData::parallaxScale() const
{
    StarFile::DataElement &element = mStarDatabase->namedStars.element("parallax");
    if (element.scale() == 0)
    {
        qWarning("Invalid parallax scale, set to 10");
        element.setScale(10);
    }
    return element.scale();
}

QDataStream &operator>>(QDataStream &stream, StarData &d)
{
    stream >> d.data.RA >> d.data.Dec
            >> d.data.dRA >> d.data.dDec
            >> d.data.parallax >> d.data.HD
            >> d.data.mag >> d.data.bv_index
            >> d.data.spec_type >> d.data.flags >> d.data.unused;
    return stream;
}

QDataStream &operator<<(QDataStream &stream, const StarData &d)
{
    stream << d.data.RA << d.data.Dec
           << d.data.dRA << d.data.dDec
           << d.data.parallax << d.data.HD
           << d.data.mag << d.data.bv_index
           << d.data.spec_type << d.data.flags << d.data.unused;
    return stream;
}

QDataStream &operator>>(QDataStream &stream, StarData::Data::StarName &d)
{
    return stream >> d.bayerName >> d.longName;
}

QDataStream &operator<<(QDataStream &stream, const StarData::Data::StarName &d)
{
    return stream << d.bayerName << d.longName;
}
