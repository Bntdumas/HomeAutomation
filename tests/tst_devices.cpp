#include <QString>
#include <QtTest>
#include <QCoreApplication>

#include "devices.h"

class devicesTest : public QObject
{
    Q_OBJECT

public:
    devicesTest() {}

private Q_SLOTS:
    void testEnumConvertionMethods();
    void testCompatibility();

private:

};

void devicesTest::testEnumConvertionMethods()
{
    QCOMPARE(devices::enumFromStringDirection(QStringLiteral("Input")), devices::Input);
    QCOMPARE(devices::enumFromStringType(QStringLiteral("Lamp")), devices::Lamp);
    QCOMPARE(devices::enumFromStringSubType(QStringLiteral("Ambiance")), devices::Ambiance);

    QCOMPARE(devices::stringFromEnumDirection(devices::Input), QStringLiteral("Input"));
    QCOMPARE(devices::stringFromEnumType(devices::Lamp), QStringLiteral("Lamp"));
    QCOMPARE(devices::stringFromEnumSubType(devices::Ambiance), QStringLiteral("Ambiance"));
}

void devicesTest::testCompatibility()
{
    QVERIFY(devices::typeCompatible(devices::Input, devices::Sensor));
    QVERIFY(!devices::typeCompatible(devices::Input, devices::Lamp));

    QVERIFY(devices::subTypeCompatible(devices::Sensor, devices::Temperature));
    QVERIFY(!devices::subTypeCompatible(devices::Lamp, devices::Temperature));
}

QTEST_MAIN(devicesTest)

#include "tst_devices.moc"
