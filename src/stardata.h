#pragma once

#include <QDataStream>
#include <QTextStream>

struct StarData
{
public:
    QString longName() const;
    QString bayerName() const;

    bool isNamedStar() const;
    // Multiplicity = stardata->flags & 0x02;
    // Variability  = stardata->flags & 0x04;
    double ra() const;
    double dec() const;

    double mag() const;

    double pmRa() const;
    double pmDec() const;

    double parallax() const;

    QString specType() const;

    QString raAsString() const;
    QString decAsString() const;

public:
    void setLongName(const QString &name);
    void setBayerName(const QString &name);

    bool setRa(double value);
    bool setRa(const QString &value);

    bool setDec(double value);
    bool setDec(const QString &value);

public:
    void dump() const;

public:
    static QString toString(double value, const QVector<QChar> &symbols = {0x00b0, '\'', '"'});
    static double fromString(const QString &value, const QVector<QChar> &symbols = {0x00b0, '\'', '"'}, bool *ok = nullptr);

public:
    friend QDataStream &operator>>(QDataStream &stream, StarData &d);
    friend QDataStream &operator<<(QDataStream &stream, const StarData &d);
    friend QTextStream &operator<<(QTextStream &stream, const StarData &d);

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
        quint8 spec_type[2];
        quint8 flags { 0 };
        quint8 unused { 0 };

        struct StarName
        {
            quint8 bayerName[8];
            quint8 longName[32];

            friend QDataStream &operator>>(QDataStream &stream, StarName &d);
            friend QDataStream &operator<<(QDataStream &stream, const StarName &d);
            friend QTextStream &operator<<(QTextStream &stream, const StarName &d);
        } starName;
    };

public:
    Data::StarName &rawName();
    Data::StarName rawName() const;

public: // TODO
    Data data;
};
