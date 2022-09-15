#pragma once

#include <array>
#include <QString>
#include <QByteArray>
#include <QDataStream>

template <int N>
struct fixstr: public std::array<char, N>
{
    typedef std::array<char, N> base_t;

    template<typename ...T>
    fixstr(T&&...o)
        : base_t {{std::forward<T>(o)...}}
    { }

    operator QString() const
    {
        return QByteArray(this->data(), N);
    }

    fixstr &operator=(const QByteArray &ba)
    {
        qstrncpy(this->data(), ba.constData(), N);
        return *this;
    }

    fixstr &operator=(const QString &str)
    {
        return *this = str.toLatin1();
    }

    friend QDataStream &operator<<(QDataStream &stream, const fixstr &str)
    {
        stream.writeRawData(str.data(), N);
        return stream;
    }

    friend QDataStream &operator>>(QDataStream &stream, fixstr &str)
    {
        stream.readRawData(str.data(), N);
        return stream;
    }
};
