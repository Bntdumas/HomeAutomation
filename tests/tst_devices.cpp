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
    QCOMPARE(Devices::enumFromStringDirection(QStringLiteral("Input")), Devices::Input);
    QCOMPARE(Devices::enumFromStringType(QStringLiteral("Lamp")), Devices::Lamp);
    QCOMPARE(Devices::enumFromStringSubType(QStringLiteral("Ambiance")), Devices::Ambiance);

    QCOMPARE(Devices::stringFromEnumDirection(Devices::Input), QStringLiteral("Input"));
    QCOMPARE(Devices::stringFromEnumType(Devices::Lamp), QStringLiteral("Lamp"));
    QCOMPARE(Devices::stringFromEnumSubType(Devices::Ambiance), QStringLiteral("Ambiance"));
}

void devicesTest::testCompatibility()
{
    QVERIFY(Devices::typeCompatible(Devices::Input, Devices::Sensor));
    QVERIFY(!Devices::typeCompatible(Devices::Input, Devices::Lamp));

    QVERIFY(Devices::subTypeCompatible(Devices::Sensor, Devices::Temperature));
    QVERIFY(!Devices::subTypeCompatible(Devices::Lamp, Devices::Temperature));
}

QTEST_MAIN(devicesTest)

#include "tst_devices.moc"
