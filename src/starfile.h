#pragma once

#include "fixstr.h"

#include <QByteArray>
#include <QDataStream>

class StarFile
{
public:
    struct Header
    {
        fixstr<124> description = {0};
        bool byteSwap = true;
        quint8 version = 0;

    public:
        friend QDataStream &operator>>(QDataStream &stream, Header &d);
        friend QDataStream &operator<<(QDataStream &stream, const Header &d);
    };

    class DataElement
    {
    public:
        QString name() const;
        qint8 size() const;
        quint8 type() const;
        qint32 scale() const;

    public:
        void setName(const QString &name);
        void setSize(qint8 value);
        void setType(quint8 value);
        void setScale(qint32 scale);

    public:
        friend QDataStream &operator>>(QDataStream &stream, DataElement &d);
        friend QDataStream &operator>>(QDataStream &stream, QVector<DataElement> &d);

        friend QDataStream &operator<<(QDataStream &stream, const DataElement &d);
        friend QDataStream &operator<<(QDataStream &stream, const QVector<DataElement> &d);

    protected:
        fixstr<10> mName = {0}; /**< Field name (eg. RA) */
        qint8  mSize  { 0 };    /**< Field size in bytes (eg. 4) */
        quint8 mType  { 0 };
        qint32 mScale { 0 };    /**< Field scale. The final field value = raw_value * scale */
    };

    class IndexEntry
    {
    public:
        quint32 id() const;
        quint32 offset() const;
        quint32 count() const;

    public:
        void setId(quint32 value);
        void setOffset(quint32 value);
        void setCount(quint32 value);

    public:
        friend QDataStream &operator>>(QDataStream &stream, IndexEntry &d);
        friend QDataStream &operator>>(QDataStream &stream, QVector<IndexEntry> &d);

        friend QDataStream &operator<<(QDataStream &stream, const IndexEntry &d);
        friend QDataStream &operator<<(QDataStream &stream, const QVector<IndexEntry> &d);

    protected:
        quint32 mId = 0;
        quint32 mOffset = 0;
        quint32 mCount = 0;
    };

public:
    QString description() const;
    int version() const;

public:
    void setDescription(const QString &description);
    void setVersion(int version);

public:
    friend QDataStream &operator>>(QDataStream &stream, StarFile &starFile);
    friend QDataStream &operator<<(QDataStream &stream, const StarFile &starFile);

public:
    Header mHeader;

    QVector<DataElement> mDataElements;
    QVector<IndexEntry> mIndexEntries;

    // File offsets
    quint32 mDataOffset;
};
