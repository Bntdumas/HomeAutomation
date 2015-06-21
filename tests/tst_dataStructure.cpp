#include <QString>
#include <QtTest>
#include <QCoreApplication>

#include "houseDataStructure.h"

class dataStructureTest : public QObject
{
    Q_OBJECT

public:
    dataStructureTest() {};

private Q_SLOTS:
    void testAddRoom_data();
    void testAddRemoveRoom();

    void testAddRemoveDevice_data();
    void testAddRemoveDevice();

private:

};

/*
    bool addRoom(const QString &roomName);
    bool removeRoom(const QString &roomName);
    bool addDevice(const QString &roomName, const QString &deviceName, deviceDirection direction, deviceType type, deviceSubType subType,
                   float value, int chipID, int esp8266Pin);

    bool removeDevice(const QString &roomName, const QString &deviceName);
    int chipIDForDevice(const QString &roomName, const QString &deviceName);

    int roomIndex(const QString &name);
    int deviceIndex(const QString &roomName, const QString &name);

    bool roomExists(const QString &roomName);
    bool deviceExists(const QString &roomName, const QString &deviceName);

 * */

void dataStructureTest::testAddRoom_data()
{
    QTest::addColumn<QString>("string");
    QTest::addColumn<bool>("ExpectedSuccess");

    QTest::newRow("valid room")   << QStringLiteral("bedroom") << true;
    QTest::newRow("invalid room") << QStringLiteral("") << false;
}

void dataStructureTest::testAddRemoveRoom()
{
    QFETCH(QString, string);
    QFETCH(bool, ExpectedSuccess);

    houseDataStructure houseData;

    bool addRoom =  houseData.addRoom(string);
    bool roomExists =  houseData.roomExists(string);
    bool roomIndex =  houseData.roomIndex(string) == (ExpectedSuccess ? 0:-1);
    bool roomCount =  houseData.roomCount() == (ExpectedSuccess ? 1:0);
    bool removeRoom =  houseData.removeRoom(string);


    QVERIFY(ExpectedSuccess ? addRoom:!addRoom);
    QVERIFY(ExpectedSuccess ? roomExists:!roomExists);
    QVERIFY(roomIndex);
    QVERIFY(roomCount);
    QVERIFY(ExpectedSuccess ? removeRoom:!removeRoom);

    QVERIFY(!houseData.roomExists(string));
    QCOMPARE(houseData.roomIndex(string), -1);
    QCOMPARE(!houseData.roomCount(), 1);
}

void dataStructureTest::testAddRemoveDevice_data()
{
    QTest::addColumn<QString>("deviceName");
    QTest::addColumn<houseDataStructure::deviceDirection>("direction");
    QTest::addColumn<houseDataStructure::deviceType>("type");
    QTest::addColumn<houseDataStructure::deviceSubType>("subType");
    QTest::addColumn<float>("value");
    QTest::addColumn<int>("chipID");
    QTest::addColumn<int>("esp8266Pin");
    QTest::addColumn<bool>("ExpectedSuccess");

    QTest::newRow("valid device")   << QStringLiteral("lamp") << true;
    QTest::newRow("invalid device") << QStringLiteral("") << false;
}

void dataStructureTest::testAddRemoveDevice()
{
    QFETCH(QString, deviceName);
    QFETCH(houseDataStructure::deviceDirection, direction);
    QFETCH(houseDataStructure::deviceType, type);
    QFETCH(houseDataStructure::deviceSubType, subType);
    QFETCH(float, value);
    QFETCH(int, chipID);
    QFETCH(int, esp8266Pin);
    QFETCH(bool, ExpectedSuccess);

    const QString roomName = QStringLiteral("bedroom");

    houseDataStructure houseData;
    QVERIFY(houseData.addRoom(roomName));
/*(const QString &roomName, const QString &deviceName,
    houseDataStructure::deviceDirection direction, houseDataStructure::deviceType type, houseDataStructure::deviceSubType subType,
    float value, int chipID, int esp8266Pin)*/

    bool addDevice =  houseData.addDevice(roomName, deviceName, direction, type, subType, value, chipID, esp8266Pin);
    bool deviceExists =  houseData.deviceExists(roomName, deviceName);
    bool deviceIndex =  houseData.deviceIndex(roomName, deviceName) == (ExpectedSuccess ? 0:-1);
    bool removedevice =  houseData.removeDevice(roomName, deviceName);


    QVERIFY(ExpectedSuccess ? addDevice:!addDevice);
    QVERIFY(ExpectedSuccess ? deviceExists:!deviceExists);
    QVERIFY(deviceIndex);
    QVERIFY(ExpectedSuccess ? removedevice:!removedevice);

    QVERIFY(!houseData.deviceExists(roomName, deviceName));
    QCOMPARE(houseData.deviceIndex(roomName, deviceName), -1);
}


QTEST_MAIN(dataStructureTest)

#include "tst_dataStructure.moc"
