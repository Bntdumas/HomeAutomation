#include "houseDataStructure.h"

bool houseDataStructure::addRoom(const QString &roomName)
{
    if (roomName.isEmpty()) {
        Q_EMIT message(tr("houseDataStructure: The room must have a name."), utils::SoftwareError);
        return false;
    }

    if (roomExists(roomName)) {
        Q_EMIT message(tr("houseDataStructure: The room to add '%1' already exists").arg(roomName), utils::SoftwareError);
        return false;
    }

    Room *newRoom = new Room(roomName);
    m_rooms.append(newRoom);
    Q_EMIT message(tr("houseDataStructure: The room '%1' was added to the house structure").arg(roomName), utils::Success);
}


bool houseDataStructure::removeRoom(const QString &roomName)
{
    if (!roomExists(roomName)) {
        Q_EMIT message(tr("houseDataStructure: The room to remove '%1' does not exists").arg(roomName), utils::SoftwareError);
        return false;
    }

    const int idx = roomIndex(roomName);

    if (idx == -1) {
        Q_EMIT message(tr("houseDataStructure: The room '%1' could not be retreived from the list of rooms").arg(roomName), utils::SoftwareError);
        return false;
    }

    delete m_rooms.takeAt(idx);
    Q_EMIT message(tr("houseDataStructure: The room '%1' was removed from the house structure").arg(roomName), utils::Success);
}

bool houseDataStructure::addDevice(const QString &roomName, const QString &deviceName,
                                   devices::DeviceDirection direction, devices::DeviceType type, devices::DeviceSubType subType,
                                   double value, int chipID, int esp8266Pin)
{
    // already have a device with this name in this room
    if (deviceExists(roomName, deviceName)) {
        Q_EMIT message(tr("houseDataStructure: The device '%1' already exists in room '%2'").arg(deviceName, roomName), utils::SoftwareError);
        return false;
    }

    // chip id valid?
    if (chipID <= 1000) {
        Q_EMIT message(tr("houseDataStructure: The chip ID '%1' is invalid").arg(chipID), utils::SoftwareError);
        return false;
    }

    if (gpioInUse(chipID, esp8266Pin)) {
        Q_EMIT message(tr("houseDataStructure: The pin '%1' is already in use in the chip  n'%2'").arg(esp8266Pin, chipID), utils::SoftwareError);
        return false;
    }

    // test types compatibility
    if (!devices::typeCompatible(direction, type)) {
        Q_EMIT message(tr("houseDataStructure: The type '%1' is not valid in regard to the pin direction '%2'").arg(type, direction), utils::SoftwareError);
        return false;
    }

    if (!devices::subTypeCompatible(type, subType)) {
        Q_EMIT message(tr("houseDataStructure: The sub type '%1' is not valid in regard to the type '%2'").arg(subType, type), utils::SoftwareError);
        return false;
    }


    // retrieve room
    if (!roomExists(roomName)) {
        Q_EMIT message(tr("houseDataStructure: The room to remove '%1' does not exists").arg(roomName), utils::SoftwareError);
        return false;
    }

    int idx = roomIndex(roomName);

    if (idx == -1) {
        Q_EMIT message(tr("houseDataStructure: The room '%1' could not be retreived from the list of rooms").arg(roomName), utils::SoftwareError);
        return false;
    }

    Room *currentRoom = m_rooms.at(idx);

    //add device to the room
    Device *dev = new Device(deviceName, direction, type, subType, value, chipID, esp8266Pin);
    if (!dev->isValid()) {
        Q_EMIT message(tr("houseDataStructure: The device '%1' is not valid").arg(deviceName), utils::SoftwareError);
        return false;
    }
    currentRoom->devices.append(dev);
    return true;
}

bool houseDataStructure::removeDevice(const QString &roomName, const QString &deviceName)
{
    const int idx = deviceIndex(roomName, deviceName);

    if (idx == -1) {
        Q_EMIT message(tr("houseDataStructure: The device '%1' could not be retreived from the room '%2'").arg(deviceName).arg(roomName), utils::SoftwareError);
        return false;
    }

    Room *room = roomPointer(roomName);
    const int deviceIdx = deviceIndex(roomName, deviceName);
    delete room->devices.takeAt(deviceIdx);

    Q_EMIT message(tr("houseDataStructure: The device '%1' was removed from the house structure").arg(deviceName), utils::Success);
}

int houseDataStructure::chipIDForDevice(const QString &roomName, const QString &deviceName)
{
    const Device *device = devicePointer(roomName, deviceName);
    return device->chipID;
}

int houseDataStructure::roomIndex(const QString &name)
{
    for (int i = 0; i < m_rooms.count(); ++i) {
        const Room *currentRoom = m_rooms.at(i);
        if (currentRoom->name == name) {
            return i;
        }
    }
    return -1;
}

int houseDataStructure::deviceIndex(const QString &roomName, const QString &name)
{
    if (!roomExists(roomName)) {
        Q_EMIT message(tr("houseDataStructure: The room to remove '%1' does not exists").arg(roomName), utils::SoftwareError);
        return false;
    }

    int idx = roomIndex(roomName);

    if (idx == -1) {
        Q_EMIT message(tr("houseDataStructure: The room '%1' could not be retreived from the list of rooms").arg(roomName), utils::SoftwareError);
        return false;
    }

    const Room *currentRoom = m_rooms.at(idx);

    for (int i = 0; i < currentRoom->devices.count(); ++i) {
        const Device *currentDevice = currentRoom->devices.at(i);
        if (currentDevice->name == name) {
            return i;
        }
    }
    return -1;
}

houseDataStructure::Room *houseDataStructure::roomPointer(const QString &name)
{
    const int idx = roomIndex(name);
    return m_rooms.at(idx);
}

houseDataStructure::Device *houseDataStructure::devicePointer(const QString &roomName, const QString &deviceName)
{
    const Room *room = roomPointer(deviceName);
    const int deviceIdx = deviceIndex(roomName, deviceName);
    return room->devices.at(deviceIdx);
}

bool houseDataStructure::roomExists(const QString &roomName)
{
    return roomIndex(roomName) != -1;
}

bool houseDataStructure::deviceExists(const QString &roomName, const QString &deviceName)
{
    return deviceIndex(roomName, deviceName) != -1;
}

bool houseDataStructure::gpioInUse(int chipID, int pin)
{
    Q_FOREACH(const Room *currentRoom, m_rooms) {
        Q_FOREACH(const Device *currentDevice, currentRoom->devices) {
            if (currentDevice->chipID == chipID && currentDevice->esp8266Pin == pin ) {
                return true;
            }
        }
    }
    return false;
}
