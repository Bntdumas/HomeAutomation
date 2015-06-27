#ifndef HOUSEDATASTRUCTURE_H
#define HOUSEDATASTRUCTURE_H

#include <QObject>
#include <QList>

#include "houseData_global.h"
#include "devices.h"

#include "utils.h"

/**
 * @brief This class holds the current state of the house
 */

class HOUSEDATASHARED_EXPORT houseDataStructure: public QObject
{
    Q_OBJECT
public:
    explicit houseDataStructure(QObject *parent = Q_NULLPTR) : QObject(parent) {}
    ~houseDataStructure() {}

    /**
     * @brief Adds an empty room to the house
     * @return true if device succesfully added.
     */
    bool addRoom(const QString &roomName);

    /**
     * @brief Remove a room from the house
     * @return true if the room was succesfully removed.
     * @warning Each device for this room will be deleted too
     */
    bool removeRoom(const QString &roomName);

    /**
     * @brief Adds a device to the room "room"
     * @return true if device succesfully added.
     * @note the device name must be unique per room
     */
    bool addDevice(const QString &roomName, const QString &deviceName, devices::DeviceDirection direction, devices::DeviceType type, devices::DeviceSubType subType,
                   double value, int chipID, int esp8266Pin);

    /**
     * @brief Remove a device from a room
     * @return true if device succesfully was removed.
     */
    bool removeDevice(const QString &roomName, const QString &deviceName);

    /**
     * @brief look for the passed device in the list,
     *        if found, the stored ID for this device is returned
     *  @note if the device is not found, -1 is returned.
     */
    int chipIDForDevice(const QString &roomName, const QString &deviceName);


    /**
     * @brief get the index of the room/device in the structure lists
     */
    int roomIndex(const QString &name);
    int deviceIndex(const QString &roomName, const QString &name);

    /**
     * @brief test if the object exists in memory.
     */
    bool roomExists(const QString &roomName);
    bool deviceExists(const QString &roomName, const QString &deviceName);

    bool gpioInUse(int chipID, int pin);

    /**
      * Methods for unit testing
      */
    int roomCount() { return m_rooms.count();}


Q_SIGNALS:
    /**
     * @brief Debug messages
     */
    void message(const QString &text, const utils::messageType type);

private:

    /**
     * @brief Represents a device connected to the house.
     * It is an element that have a single function (a lamp or a sensor for example)
     */
    struct Device {
        QString name;
        devices::DeviceDirection direction;
        devices::DeviceType type;
        devices::DeviceSubType subType;
        float value;
        int chipID;
        int esp8266Pin;

        Device(const QString &name, devices::DeviceDirection direction, devices::DeviceType type, devices::DeviceSubType subType,
               float value, int chipID, int esp8266Pin):
            name(name)
          ,direction(direction)
          ,type(type)
          ,subType(subType)
          ,value(value)
          ,chipID(chipID)
          ,esp8266Pin(esp8266Pin){}

        bool isValid() const {
            return !name.isEmpty();
        }
    };

    typedef QList<Device *> deviceList;

    /**
     * @brief operator overload == and != to compare devices
     */

    /*inline bool operator==(const Device &left, const Device &right) {
        bool sameName = left.roomName == right.roomName;
        bool sameDeviceCount = left.devices.count() == right.devices.count();
        bool sameDevices = true;
        if (sameDeviceCount) {
            for (int i = 0; i< left.devices.count(); ++i) {
                if (left.devices.at(i) != right.devices.at(i)) {
                    sameDevices = false;
                }
            }
        } else {
            sameDevices = false;
        }
        return sameName && sameDeviceCount && sameDevices;
    }
    inline bool operator!=(const Device &left, const Device &right) {
        return !(left == right);
    }*/


    /**
     * @brief The Room struct represents a room.
     * It contains the room readable name and all it's devices
     */
    struct Room {
        QString name;
        deviceList devices;

        Room(const QString &name) : name(name) {}

        /**
         * @brief determines if the object is valid
         */
        bool isValid() const {
            if (name.isEmpty()) {
                return false;
            }
            Q_FOREACH(const Device *device, devices) {
                if (!device->isValid()) {
                    return false;
                }
            }
            return true;
        }
    };

    /**
     * @brief operator overload == and != to compare rooms
     */
    /*inline bool operator==(const Room &left, const Room &right) {
        bool sameName = left.roomName == right.roomName;
        bool sameDeviceCount = left.devices.count() == right.devices.count();
        bool sameDevices = true;
        if (sameDeviceCount) {
            for (int i = 0; i< left.devices.count(); ++i) {
                if (left.devices.at(i) != right.devices.at(i)) {
                    sameDevices = false;
                }
            }
        } else {
            sameDevices = false;
        }
        return sameName && sameDeviceCount && sameDevices;
    }
    inline bool operator!=(const Room &left, const Room &right) {
        return !(left == right);
    }*/


    /**
     * @brief get the pointer of the room/device in the structure lists
     */
    Room *roomPointer(const QString &name);
    Device *devicePointer(const QString &roomName, const QString &deviceName);

    /**
      * Storage
      */
    typedef QList<Room *> roomList;
    roomList m_rooms;
};

Q_DECLARE_METATYPE(devices::DeviceDirection)
Q_DECLARE_METATYPE(devices::DeviceType)
Q_DECLARE_METATYPE(devices::DeviceSubType)

#endif // HOUSEDATASTRUCTURE_H

