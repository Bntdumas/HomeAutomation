#include <QString>
#include <QtTest>
#include <QCoreApplication>

#include "houseDataStructure.h"
#include "debugCenter.h"

class dataStructureTest : public QObject
{
    Q_OBJECT

public:
    dataStructureTest() {};

private Q_SLOTS:
    void testAddRemoveRoom_data();
    void testAddRemoveRoom();

    void testAddRemoveDevice_data();
    void testAddRemoveDevice();

    void testDeviceDoublons();

private:

};

void dataStructureTest::testAddRemoveRoom_data()
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
    debugCenter dbgCenter;
    dbgCenter.addWatcher<houseDataStructure>(&houseData);

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
    QTest::addColumn<devices::DeviceDirection>("direction");
    QTest::addColumn<devices::DeviceType>("type");
    QTest::addColumn<devices::DeviceSubType>("subType");
    QTest::addColumn<double>("value");
    QTest::addColumn<int>("chipID");
    QTest::addColumn<int>("esp8266Pin");
    QTest::addColumn<bool>("ExpectedSuccess");

    QTest::newRow("Valid Input")   << QStringLiteral("Valid Input") << devices::Input
                                   <<  devices::Sensor << devices::Temperature
                                    << 2.5 << 123456 << 5 << true;

    QTest::newRow("Valid Output")   << QStringLiteral("Valid Output") << devices::Output
                                    <<  devices::Lamp << devices::LED
                                     << 1.0 << 123456 << 4 << true;

    QTest::newRow("invalid Input (wrong type for direction)")   << QStringLiteral("invalid Input (wrong type for direction)") << devices::Input
                                                                <<  devices::Lamp << devices::LED
                                                                 << 22.5 << 123456 << 5 << false;

    QTest::newRow("invalid Output (wrong type for direction)")   << QStringLiteral("invalid Output (wrong type for direction)") << devices::Output
                                                                 <<  devices::Sensor << devices::Temperature
                                                                  << 1.0 << 123456 << 4 << false;

    QTest::newRow("invalid Input (wrong subtype for type)")   << QStringLiteral("invalid Input (wrong subtype for type)") << devices::Input
                                                              <<  devices::User << devices::Temperature
                                                               << 22.5 << 123456 << 5 << false;

    QTest::newRow("invalid Output (wrong subtype for type)")   << QStringLiteral("invalid Output (wrong subtype for type") << devices::Output
                                                               <<  devices::Plug << devices::Temperature
                                                                << 1.0 << 123456 << 4 << false;

    QTest::newRow("invalid chip ID")   << QStringLiteral("invalid chip ID") << devices::Output
                                       <<  devices::Lamp << devices::LED
                                        << 1.0 << -1 << 4 << false;

    QTest::newRow("invalid name")   << QStringLiteral("") << devices::Output
                                    <<  devices::Lamp << devices::LED
                                     << 1.0 << 123456 << 4 << false;
}

void dataStructureTest::testAddRemoveDevice()
{
    QFETCH(QString, deviceName);
    QFETCH(devices::DeviceDirection, direction);
    QFETCH(devices::DeviceType, type);
    QFETCH(devices::DeviceSubType, subType);
    QFETCH(double, value);
    QFETCH(int, chipID);
    QFETCH(int, esp8266Pin);
    QFETCH(bool, ExpectedSuccess);

    const QString roomName = QStringLiteral("bedroom");

    houseDataStructure houseData;
    debugCenter dbgCenter;
    dbgCenter.addWatcher<houseDataStructure>(&houseData);
    QVERIFY(houseData.addRoom(roomName));

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

void dataStructureTest::testDeviceDoublons()
{

    const QString roomName = QStringLiteral("bedroom");
    houseDataStructure houseData;
    debugCenter dbgCenter;
    dbgCenter.addWatcher<houseDataStructure>(&houseData);

    QVERIFY(houseData.addRoom(roomName));

    // same name
    QVERIFY(houseData.addDevice(roomName, QStringLiteral("device"), devices::Output, devices::Lamp,
                                          devices::LED, 1.0, 12354, 5));
    QVERIFY(!houseData.addDevice(roomName, QStringLiteral("device"), devices::Input, devices::Sensor,
                                          devices::Temperature, 1.0, 12354, 4));


    // same GPIO pin on same chip
    QVERIFY(houseData.addDevice(roomName, QStringLiteral("deviceA"), devices::Output, devices::Lamp,
                                          devices::LED, 1.0, 12354, 2));
    QVERIFY(houseData.addDevice(roomName, QStringLiteral("deviceC"), devices::Output, devices::Lamp,
                                          devices::LED, 1.0, 12354, 3));
    QVERIFY(houseData.addDevice(roomName, QStringLiteral("deviceB"), devices::Output, devices::Lamp,
                                          devices::LED, 1.0, 12355, 2));
    QVERIFY(!houseData.addDevice(roomName, QStringLiteral("deviceD"), devices::Output, devices::Lamp,
                                          devices::LED, 1.0, 12354, 2));


}


QTEST_MAIN(dataStructureTest)

#include "tst_dataStructure.moc"
