#include <QString>
#include <QtTest>
#include <QCoreApplication>
#include <QTcpSocket>

#include "homeServer.h"

class networkModuleWithHardwareTest : public QObject
{
    Q_OBJECT

public:
    networkModuleWithHardwareTest();

private Q_SLOTS:
    void init();
    void cleanup();

    //using real esp8266 module connected via wifi
    void testWifiConnection();
    void testCommands();


private:
    homeServer *m_server;
    void waitForRealClientConnected(int numberOfExpectedClients);
};

networkModuleWithHardwareTest::networkModuleWithHardwareTest()
{
}

void networkModuleWithHardwareTest::init()
{
    m_server = new homeServer();
    m_server->createTCPServer();
    QVERIFY(m_server->isListening());
}

void networkModuleWithHardwareTest::cleanup()
{
    delete m_server;
}

void networkModuleWithHardwareTest::testWifiConnection()
{
  //  waitForRealClientConnected(3);
}

void networkModuleWithHardwareTest::testCommands()
{
    waitForRealClientConnected(3);
    m_server->resetLineCounter();
    const int messageToSend = 100;
    for (int i = 0; i < messageToSend; i++) {
        m_server->sendAll(QStringLiteral("DATA"));
    }

    QTimer timer;
    timer.setSingleShot(true);
    timer.start(90000);

    while (timer.isActive())
        QCoreApplication::instance()->processEvents();

    QCOMPARE(m_server->receivedLines(), messageToSend * 3);
}

void networkModuleWithHardwareTest::waitForRealClientConnected(int numberOfExpectedClients)
{
    QTimer timer;
    timer.setSingleShot(true);
    qDebug() << "######## NOW RESET THE MODULES ########";
    timer.start(3000);
    while (timer.isActive())
        QCoreApplication::instance()->processEvents();

    // wait for 5seconds, that should give enough time for clients to connect
    timer.start(5000);
    while (timer.isActive())
        QCoreApplication::instance()->processEvents();


    QCOMPARE(m_server->connectedClients(), numberOfExpectedClients);
}

QTEST_MAIN(networkModuleWithHardwareTest)

#include "tst_networkModuleWithHardware.moc"
