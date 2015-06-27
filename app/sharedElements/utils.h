#ifndef UTILS_H
#define UTILS_H
#include <QObject>
#include <QMetaEnum>

/**
 * @brief Contains some utilitaries functions
 */
class utils: public QObject {

    Q_OBJECT

public:
    enum messageType {
        Info = 0,
        Success,
        Warning,
        SoftwareError,
        NetworkError
    };
    Q_ENUMS(messageType)
    static QString stringFromEnum(messageType type)
    {
        QMetaEnum currentMetaEnum = metaEnum(QStringLiteral("messageType"));
        return QString::fromLatin1(currentMetaEnum.valueToKey(type));
    }

    static messageType enumFromString(const QString &type)
    {
        QMetaEnum currentMetaEnum = metaEnum(QStringLiteral("messageType"));
        const QByteArray byteArray = type.toUtf8();
        const char *textPtr = byteArray.constData();
        int value = currentMetaEnum.keyToValue(textPtr);
        return static_cast<messageType>(value);
    }

private:
    static QMetaEnum metaEnum(const QString &enumName) {
        const QMetaObject &mo = utils::staticMetaObject;

        const QByteArray byteArray = enumName.toUtf8();
        const char *textPtr = byteArray.constData();
        int index = mo.indexOfEnumerator(textPtr);

        return mo.enumerator(index);
    }
};
Q_DECLARE_METATYPE(utils::messageType)

#endif // UTILS_H

