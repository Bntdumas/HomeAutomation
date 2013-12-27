#include <QtTest/QtTest>
#include <QCoreApplication>
#include <QtTest/QSignalSpy>

#include "QHFDevice.h"

/**
 * @brief Test that the NRF24L01 module is properly working.
 * This test covers:
 * - NRF24L01 proper connection
 * - NRF24L01 functionnality
 * - Qt library using the NRF24L01 module
 */

class QHFDeviceTest : public QObject
{
    Q_OBJECT

public:
    QHFDeviceTest(): m_device(0){}

private Q_SLOTS:
    void testRead()
    {
        m_device = new QHFDevice();
        QSignalSpy spyError(m_device, SIGNAL(error(QString)));
        QSignalSpy spyNewData(m_device, SIGNAL(newData(QString,quint64)));
        m_device->initModule();
        QCOMPARE(spyError.count(), 0); // No errors triggered

        qDebug() << "Start waiting";
        while (!m_device->threadTerminated())
            QCoreApplication::instance()->processEvents();
        qDebug() << "Done!";

        QCOMPARE(spyNewData.count(), 1); // No errors triggered


        delete m_device;
        m_device = 0;
    }

//    void testWrite()
//    {
//        m_device = new QHFDevice();
//        QSignalSpy spyError(m_device, SIGNAL(error(QString)));
//        QSignalSpy spyNewData(m_device, SIGNAL(newData(QString,quint64)));
//        m_device->initModule();
//        QCOMPARE(spyError.count(), 0); // No errors triggered

//        m_device->requestWrite(tr("Writing Test"), 0xAABBCCDDA1);

//        delete m_device;
//        m_device = 0;
//    }



private:
    QHFDevice *m_device;
};

QTEST_MAIN(QHFDeviceTest)
#include "tst_QHFDeviceTest.moc"
