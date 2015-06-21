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
    void testAddRemoveRoom_data();
    void testAddRemoveRoom();

    void testAddRemoveDevice_data();
    void testAddRemoveDevice();

private:

};

/*
    bool addRoom(const QString &roomName);
    bool removeRoom(const QString &roomName);
    bool addDevice(const QString &roomName, const QString &deviceName, DeviceDirection direction, DeviceType type, DeviceSubType subType,
                   float value, int chipID, int esp8266Pin);

    bool removeDevice(const QString &roomName, const QString &deviceName);
    int chipIDForDevice(const QString &roomName, const QString &deviceName);

    int roomIndex(const QString &name);
    int deviceIndex(const QString &roomName, const QString &name);

    bool roomExists(const QString &roomName);
    bool deviceExists(const QString &roomName, const QString &deviceName);

 * */

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
    QTest::addColumn<houseDataStructure::DeviceDirection>("direction");
    QTest::addColumn<houseDataStructure::DeviceType>("type");
    QTest::addColumn<houseDataStructure::DeviceSubType>("subType");
    QTest::addColumn<double>("value");
    QTest::addColumn<int>("chipID");
    QTest::addColumn<int>("esp8266Pin");
    QTest::addColumn<bool>("ExpectedSuccess");

    /*
     * To add in houseDataStructure
     * - test if chip id valid
     * - test if direction, types and subtypes are compatibles
     * - test if gpio for pin n is already used
     * - value can be left blank
     */

    QTest::newRow("Valid Input")   << QStringLiteral("Temperature exterior") << houseDataStructure::Input
                                   <<  houseDataStructure::Sensor << houseDataStructure::Temperature
                                    << 2.5 << 123456 << 5 << true;

    QTest::newRow("Valid Output")   << QStringLiteral("Living room lamp") << houseDataStructure::Output
                                    <<  houseDataStructure::Lamp << houseDataStructure::LED
                                     << 1.0 << 123456 << 4 << true;

    QTest::newRow("invalid Input (wrong type for direction)")   << QStringLiteral("Temperature exterior") << houseDataStructure::Input
                                                                <<  houseDataStructure::Lamp << houseDataStructure::LED
                                                                 << 22.5 << 123456 << 5 << true;

    QTest::newRow("invalid Output (wrong type for direction)")   << QStringLiteral("Living room lamp") << houseDataStructure::Output
                                                                 <<  houseDataStructure::Sensor << houseDataStructure::Temperature
                                                                  << 1.0 << 123456 << 4 << true;

    QTest::newRow("invalid Input (wrong subtype for type)")   << QStringLiteral("Temperature exterior") << houseDataStructure::Input
                                                              <<  houseDataStructure::User << houseDataStructure::Temperature
                                                               << 22.5 << 123456 << 5 << false;

    QTest::newRow("invalid Output (wrong subtype for type)")   << QStringLiteral("Living room lamp") << houseDataStructure::Output
                                                               <<  houseDataStructure::Plug << houseDataStructure::Temperature
                                                                << 1.0 << 123456 << 4 << false;

    QTest::newRow("invalid chip ID")   << QStringLiteral("Living room lamp") << houseDataStructure::Output
                                       <<  houseDataStructure::Lamp << houseDataStructure::LED
                                        << 1.0 << -1 << 4 << false;

    QTest::newRow("invalid name")   << QStringLiteral("") << houseDataStructure::Output
                                    <<  houseDataStructure::Lamp << houseDataStructure::LED
                                     << 1.0 << 123456 << 4 << false;
}

void dataStructureTest::testAddRemoveDevice()
{
    QFETCH(QString, deviceName);
    QFETCH(houseDataStructure::DeviceDirection, direction);
    QFETCH(houseDataStructure::DeviceType, type);
    QFETCH(houseDataStructure::DeviceSubType, subType);
    QFETCH(double, value);
    QFETCH(int, chipID);
    QFETCH(int, esp8266Pin);
    QFETCH(bool, ExpectedSuccess);

    const QString roomName = QStringLiteral("bedroom");

    houseDataStructure houseData;
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


QTEST_MAIN(dataStructureTest)

#include "tst_dataStructure.moc"