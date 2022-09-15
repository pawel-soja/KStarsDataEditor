#pragma once

#include <QTextStream>
#include <QDataStream>

template <size_t N>
inline QDataStream &operator>>(QDataStream &stream, quint8 (&d)[N])
{
    stream.readRawData((char*)d, N);
    return stream;
}

template <size_t N>
inline QDataStream &operator<<(QDataStream &stream, const quint8 (&d)[N])
{
    stream.writeRawData((const char*)d, N);
    return stream;
}

#include <QDebug>

template <size_t N>
inline QTextStream &operator>>(QTextStream &stream, quint8 (&d)[N])
{
    //stream.readRawData((char*)d, N);
    qFatal("");
    return stream;
}

template <size_t N>
inline QTextStream &operator<<(QTextStream &stream, const quint8 (&d)[N])
{
    stream << QByteArray((const char *)d, N);
    return stream;
}

inline QTextStream &endlw(QTextStream &s)
{
    int w = s.fieldWidth();
    return s << qSetFieldWidth(0) << "\n" << qSetFieldWidth(w);
}

inline QTextStream &TAG(QTextStream &s)
{
    int w = s.fieldWidth();
    return s << qSetFieldWidth(2) << "#" << qSetFieldWidth(w);
}
