#ifndef DEVICES_H
#define DEVICES_H
#include <QObject>
#include <QMetaEnum>
#include <QString>
#include <QByteArray>

/**
 * @brief The Devices class contains the available devices in the house.
 * TODO generate this file from an XML file
 */

class Devices: public QObject {

    Q_OBJECT

public:
    /**
     * @brief These enums contains the differents types of devices available in the house.
     *  When modifying these, you need to make sure to update @subTypeCompatible() and @typeCompatible() with the new data in order to separate the elements into sections.
     */
    enum DeviceDirection {
        Input = 0
        ,Output
        ,Disabled
    };
    Q_ENUMS(DeviceDirection)
    static QString stringFromEnumDirection(DeviceDirection direction)
    {
        QMetaEnum currentMetaEnum = metaEnum(QStringLiteral("DeviceDirection"));
        return QString::fromLatin1(currentMetaEnum.valueToKey(direction));
    }

    static DeviceDirection enumFromStringDirection(const QString &direction)
    {
        QMetaEnum currentMetaEnum = metaEnum(QStringLiteral("DeviceDirection"));
        const QByteArray byteArray = direction.toUtf8();
        const char *textPtr = byteArray.constData();
        int value = currentMetaEnum.keyToValue(textPtr);
        return static_cast<DeviceDirection>(value);
    }

    enum DeviceType {
        Lamp = 0
        ,Switch
        ,Plug
        ,Sensor
        ,User
        ,TypeOther
    };
    Q_ENUMS(DeviceType)
    static QString stringFromEnumType(DeviceType type)
    {
        QMetaEnum currentMetaEnum = metaEnum(QStringLiteral("DeviceType"));
        return QString::fromLatin1(currentMetaEnum.valueToKey(type));
    }

    static DeviceType enumFromStringType(const QString &type)
    {
        QMetaEnum currentMetaEnum = metaEnum(QStringLiteral("DeviceType"));
        const QByteArray byteArray = type.toUtf8();
        const char *textPtr = byteArray.constData();
        int value = currentMetaEnum.keyToValue(textPtr);
        return static_cast<DeviceType>(value);
    }

    enum DeviceSubType {
        None = 0
        ,Ambiance
        ,SpotLight
        ,RoomLight
        ,LED
        ,Computer
        ,Screen
        ,NormalPlug
        ,Temperature
        ,Light
        ,Humidity
        ,Button
        ,Potentiometer
        ,SubTypeOther
    };
    Q_ENUMS(DeviceSubType)

    static QString stringFromEnumSubType(DeviceSubType subType)
    {
        QMetaEnum currentMetaEnum = metaEnum(QStringLiteral("DeviceSubType"));
        return QString::fromLatin1(currentMetaEnum.valueToKey(subType));
    }

    static DeviceSubType enumFromStringSubType(const QString &subType)
    {
        QMetaEnum currentMetaEnum = metaEnum(QStringLiteral("DeviceSubType"));
        const QByteArray byteArray = subType.toUtf8();
        const char *textPtr = byteArray.constData();
        int value = currentMetaEnum.keyToValue(textPtr);
        return static_cast<DeviceSubType>(value);
    }

    /**
     * @brief methods to test if the passed enums are compatibles
     * eg a lamp or a plug cannot be an input, and a lamp cannot be a temperature sensor
     */
    static bool subTypeCompatible(DeviceType type, DeviceSubType subType)
    {
        switch (type) {
        case Lamp:
            return (subType == Ambiance ||
                    subType == SpotLight ||
                    subType == RoomLight ||
                    subType == LED);
        case Switch:
            return (subType == Computer);
        case Plug:
            return (subType == Screen ||
                    subType == NormalPlug);
        case Sensor:
            return (subType == Temperature ||
                    subType == Light ||
                    subType == Humidity);
        case User:
            return (subType == Button ||
                    subType == Potentiometer);
        case TypeOther:
            return (subType == SubTypeOther);
        default:
            return false;
        }
    }

    static bool typeCompatible(DeviceDirection direction, DeviceType type)
    {
        switch (direction) {
        case Input:
            return (type == User ||
                    type == Sensor ||
                    type == TypeOther);
        case Output:
            return (type == Lamp ||
                    type == Switch ||
                    type == Plug ||
                    type == TypeOther);
        case Disabled:
            return true;
        default:
            return false;
        }
    }

private:
    static QMetaEnum metaEnum(const QString &enumName) {
        const QMetaObject &mo = Devices::staticMetaObject;

        const QByteArray byteArray = enumName.toUtf8();
        const char *textPtr = byteArray.constData();
        int index = mo.indexOfEnumerator(textPtr);

        return mo.enumerator(index);
    }

};

#endif // DEVICES_H

