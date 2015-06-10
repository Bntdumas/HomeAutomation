#include "houseDataStructure.h"

bool houseDataStructure::addRoom(const QString &roomName)
{
    if (roomExists(roomName)) {
        Q_EMIT message(tr("houseDataStructure: The room to add \"%1\" already exists").arg(roomName), utils::SoftwareError);
        return false;
    }
    Room *newRoom = new Room(roomName);
    m_rooms.append(newRoom);
    Q_EMIT message(tr("houseDataStructure: The room \"%1\" was added to the house structure").arg(roomName), utils::Success);
}

bool houseDataStructure::removeRoom(const QString &roomName)
{
    if (!roomExists(roomName)) {
        Q_EMIT message(tr("houseDataStructure: The room to remove \"%1\" does not exists").arg(roomName), utils::SoftwareError);
        return false;
    }

    const int idx = roomIndex(roomName);

    if (idx == -1) {
        Q_EMIT message(tr("houseDataStructure: The room \"%1\" could not be retreived from the list of rooms").arg(roomName), utils::SoftwareError);
        return false;
    }

    delete m_rooms.takeAt(idx);
    Q_EMIT message(tr("houseDataStructure: The room \"%1\" was removed from the house structure").arg(roomName), utils::Success);
}

bool houseDataStructure::addDevice(const QString &roomName, const QString &deviceName,
                                   houseDataStructure::deviceDirection direction, houseDataStructure::deviceType type, houseDataStructure::deviceSubType subType,
                                   float value, int chipID, int esp8266Pin)
{
    // retrieve room
    if (!roomExists(roomName)) {
        Q_EMIT message(tr("houseDataStructure: The room to remove \"%1\" does not exists").arg(roomName), utils::SoftwareError);
        return false;
    }

    int idx = roomIndex(roomName);

    if (idx == -1) {
        Q_EMIT message(tr("houseDataStructure: The room \"%1\" could not be retreived from the list of rooms").arg(roomName), utils::SoftwareError);
        return false;
    }

    Room *currentRoom = m_rooms.at(idx);

    //add device to the room
    Device *dev = new Device(deviceName, direction, type, subType, value, chipID, esp8266Pin);
    if (!dev->isValid()) {
        Q_EMIT message(tr("houseDataStructure: The device \"%1\" is not valid").arg(deviceName), utils::SoftwareError);
        return false;
    }
    currentRoom->devices.append(dev);
}

bool houseDataStructure::removeDevice(const QString &roomName, const QString &deviceName)
{
    const int idx = deviceIndex(roomName, deviceName);

    if (idx == -1) {
        Q_EMIT message(tr("houseDataStructure: The device \"%1\" could not be retreived from the room \"%2\"").arg(deviceName).arg(roomName), utils::SoftwareError);
        return false;
    }

    Room *room = roomPointer(roomName);
    const int deviceIdx = deviceIndex(roomName, deviceName);
    delete room->devices.takeAt(deviceIdx);

    Q_EMIT message(tr("houseDataStructure: The device \"%1\" was removed from the house structure").arg(deviceName), utils::Success);
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
        Q_EMIT message(tr("houseDataStructure: The room to remove \"%1\" does not exists").arg(roomName), utils::SoftwareError);
        return false;
    }

    int idx = roomIndex(roomName);

    if (idx == -1) {
        Q_EMIT message(tr("houseDataStructure: The room \"%1\" could not be retreived from the list of rooms").arg(roomName), utils::SoftwareError);
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
