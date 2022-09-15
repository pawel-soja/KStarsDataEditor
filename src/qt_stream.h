#pragma once

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
