#pragma once

#include "fixstr.h"
#include <QDataStream>

struct StarData
{
public:
    QString longName() const;
    QString bayerName() const;

    double ra() const;
    double dec() const;

    double mag() const;
    double bvIndex() const;

    double pmRa() const;
    double pmDec() const;

    double parallax() const;

    QString specType() const;
    quint8 flags() const;

public:
    QString raAsString() const;
    QString decAsString() const;

    bool isNamedStar() const;
    bool isMultiplicity() const;
    bool isVariability() const;

public:
    bool setLongName(const QString &name);
    bool setBayerName(const QString &name);

    bool setRa(double value);
    bool setRa(const QString &value);

    bool setDec(double value);
    bool setDec(const QString &value);

    bool setMag(double value);
    bool setBvIndex(double value);

    bool setPmRa(double value);
    bool setPmDec(double value);

    bool setParallax(double value);
    bool setSpecType(const QByteArray &value);

    bool setFlags(quint8 value);

public:
    double raScale() const;
    double decScale() const;
    double magScale() const;
    double bvIndexScale() const;
    double pmRaScale() const;
    double pmDecScale() const;
    double parallaxScale() const;

public:
    static QString toString(double value, const QVector<QChar> &symbols = {0x00b0, '\'', '"'});
    static double fromString(const QString &value, const QVector<QChar> &symbols = {0x00b0, '\'', '"'}, bool *ok = nullptr);

public:
    friend QDataStream &operator>>(QDataStream &stream, StarData &d);
    friend QDataStream &operator<<(QDataStream &stream, const StarData &d);

public:
    struct Data
    {
        qint32 RA { 0 };  /**< Raw signed 32-bit RA value. Needs to be multiplied by the scale (1e6) */
        qint32 Dec { 0 }; /**< Raw signed 32-bit DE value. Needs to be multiplied by the scale (1e6) */
        qint32 dRA { 0 };
        qint32 dDec { 0 };
        qint32 parallax { 0 };
        qint32 HD { 0 };  /**< unsigned 32-bit Henry Draper Index. No scaling is required. */
        qint16 mag { 0 }; /**< signed 16-bit raw magnitude. Needs to be divided by the scale (1e2) */
        qint16 bv_index { 0 };
        fixstr<2> spec_type;
        quint8 flags { 0 };
        quint8 unused { 0 };

        struct StarName
        {
            fixstr<8> bayerName;
            fixstr<32> longName;

            friend QDataStream &operator>>(QDataStream &stream, StarName &d);
            friend QDataStream &operator<<(QDataStream &stream, const StarName &d);
        } starName;
    };

public:
    Data::StarName &rawName();
    Data::StarName rawName() const;

protected:
    Data data;
};

