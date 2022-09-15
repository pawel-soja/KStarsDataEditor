#include "starfile.h"
#include "qt_stream.h"

QDataStream &operator>>(QDataStream &stream, StarFile::Header &d)
{
    // Read the first 124 bytes of the binary file which contains a general text about the binary data.
    // e.g. "KStars Star Data v1.0. To be read using the 32-bit StarData structure only"
    //d.description = stream.device()->read(124);
    stream >> d.description;

    // Find out endianess from reading "KS" 0x4B53 in the binary file which was encoded on a little endian machine
    // Therefore, in the binary file it is written as 53 4B (little endian as least significant byte is stored first),
    // and when read on a little endian machine then it results in 0x4B53 (least significant byte is stored first in memory),
    // whereas a big endian machine would read it as 0x534B (most significant byte is stored first in memory).
    quint16 KS = 0;
    stream >> KS;

    d.byteSwap = (KS != 0x4B53);
    stream.setByteOrder(d.byteSwap ? QDataStream::LittleEndian : QDataStream::BigEndian);

    return stream >> d.version;
}

QDataStream &operator<<(QDataStream &stream, const StarFile::Header &d)
{
    stream.setByteOrder(d.byteSwap ? QDataStream::LittleEndian : QDataStream::BigEndian);
    return stream << d.description << quint16(0x4B53) << d.version;
}

QString StarFile::DataElement::name() const
{
    return QByteArray((const char*)mName, sizeof mName);
}

qint8 StarFile::DataElement::size() const
{
    return mSize;
}

quint8 StarFile::DataElement::type() const
{
    return mType;
}

qint32 StarFile::DataElement::scale() const
{
    return mScale;
}

void StarFile::DataElement::setName(const QString &name)
{
    qstrncpy((char *)mName, name.toLatin1().constData(), sizeof mName);
}

void StarFile::DataElement::setSize(qint8 value)
{
    mSize = value;
}

void StarFile::DataElement::setType(quint8 value)
{
    mType = value;
}

void StarFile::DataElement::setScale(qint32 scale)
{
    mScale = scale;
}

QDataStream &operator>>(QDataStream &stream, StarFile::DataElement &d)
{
    return stream >> d.mName >> d.mSize >> d.mType >> d.mScale;
}

QDataStream &operator>>(QDataStream &stream, QVector<StarFile::DataElement> &d)
{
    quint16 count;
    stream >> count;
    d.resize(count);
    for (auto &it: d)
    {
        stream >> it;
    }
    return stream;
}

QDataStream &operator<<(QDataStream &stream, const StarFile::DataElement &d)
{
    return stream << d.mName << d.mSize << d.mType << d.mScale;
}

QDataStream &operator<<(QDataStream &stream, const QVector<StarFile::DataElement> &d)
{
    stream << quint16(d.size());
    for (const auto &it: d)
    {
        stream << it;
    }
    return stream;
}

quint32 StarFile::IndexEntry::id() const
{
    return mId;
}

quint32 StarFile::IndexEntry::offset() const
{
    return mOffset;
}

quint32 StarFile::IndexEntry::count() const
{
    return mCount;
}

void StarFile::IndexEntry::setId(quint32 value)
{
    mId = value;
}

void StarFile::IndexEntry::setOffset(quint32 value)
{
    mOffset = value;
}

void StarFile::IndexEntry::setCount(quint32 value)
{
    mCount = value;
}

QDataStream &operator>>(QDataStream &stream, StarFile::IndexEntry &d)
{
    return stream >> d.mId >> d.mOffset >> d.mCount;
}

QDataStream &operator>>(QDataStream &stream, QVector<StarFile::IndexEntry> &d)
{
    quint32 count;
    stream >> count;
    d.resize(count);
    for (auto &it: d)
    {
        stream >> it;
        if (it.id() >= count)
        {
            qDebug() << QString::asprintf("ID %u is greater than the expected number of expected entries (%u)", it.id(), count);
        }
    }
    return stream;
}

QDataStream &operator<<(QDataStream &stream, const StarFile::IndexEntry &d)
{
    return stream << d.mId << d.mOffset << d.mCount;
}

QDataStream &operator<<(QDataStream &stream, const QVector<StarFile::IndexEntry> &d)
{
    stream << quint32(d.size());
    for (const auto &it: d)
    {
        stream << it;
    }
    return stream;
}

QDataStream &operator>>(QDataStream &stream, StarFile &starFile)
{
    return stream >> starFile.mHeader >> starFile.mDataElements >> starFile.mIndexEntries;
}

QDataStream &operator<<(QDataStream &stream, const StarFile &starFile)
{
    return stream << starFile.mHeader << starFile.mDataElements << starFile.mIndexEntries;
}

QString StarFile::description() const
{
    return QByteArray((const char*)mHeader.description, sizeof mHeader.description);
}

int StarFile::version() const
{
    return mHeader.version;
}

void StarFile::setDescription(const QString &description)
{
    qstrncpy((char *)mHeader.description, description.toLatin1().constData(), sizeof mHeader.description);
}

void StarFile::setVersion(int version)
{
    mHeader.version = version;
}
