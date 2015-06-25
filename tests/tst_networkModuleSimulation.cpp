#include <QString>
#include <QtTest>
#include <QCoreApplication>
#include <QTcpSocket>

#include "moduleServer.h"
#include "clientSimulator.h"

class networkModuleSimulationTest : public QObject
{
    Q_OBJECT

public:
    networkModuleSimulationTest() {}

private Q_SLOTS:
    void init();
    void cleanup();

    void testSendLotsOfLines();
    void testSendIDRequest();
    void testSendDataRequest();

private:
    moduleServer *m_server;
    clientSimulator *m_client;

    void initSimulator();
};

void networkModuleSimulationTest::init()
{
    m_server = new moduleServer();
    m_server->createTCPServer();
    QVERIFY(m_server->isListening());
    m_server->setAutomaticPolling(false);
}

void networkModuleSimulationTest::cleanup()
{
    delete m_server;
}

// simple test,
void networkModuleSimulationTest::testSendLotsOfLines()
{
    /*
     * This test starts a server and a client, then sends 100 requests for the client to reset.
     * For each reset command sent, we want an answer, so we expect 100 answers
     */
    initSimulator();

    const int messagesToSend = 10;
    // Send lots of commands to the client and make sure they all come back.
    for (int i = 0; i < messagesToSend; ++i) {
       m_server->resetModules();
       QCoreApplication::instance()->processEvents();
    }

    QTimer timer;
    timer.setSingleShot(true);
    timer.start(500);

    while (timer.isActive())
        QCoreApplication::instance()->processEvents();

    QCOMPARE(m_server->receivedLines(), messagesToSend);

    delete m_client;
    m_client = 0;
}

void networkModuleSimulationTest::testSendIDRequest()
{
    initSimulator();

    QSignalSpy spyID(m_server, &moduleServer::newModuleConnected);

    QVERIFY(m_server->requestIDFromModules());
    QTimer timer;
    timer.setSingleShot(true);
    timer.start(10);
    while (timer.isActive())
        QCoreApplication::instance()->processEvents();


    QVERIFY(m_server->requestIDFromModule(123456));
    timer.start(10);
    while (timer.isActive())
        QCoreApplication::instance()->processEvents();

    QCOMPARE(spyID.count(), 2);
    QList<QVariant> arguments = spyID.takeFirst();
    QVERIFY(arguments.at(0).toInt() == 123456);
    arguments = spyID.takeLast();
    QVERIFY(arguments.at(0).toInt() == 123456);

    delete m_client;
    m_client = 0;
}

void networkModuleSimulationTest::testSendDataRequest()
{
    initSimulator();

    QSignalSpy spyGPIOChanged(m_server, &moduleServer::gpioChanged);

    QVERIFY(m_server->requestDataFromModules());
    QTimer timer;
    timer.setSingleShot(true);
    timer.start(10);
    while (timer.isActive())
        QCoreApplication::instance()->processEvents();

    QVERIFY(m_server->requestDataFromModule(123456));
    timer.start(10);
    while (timer.isActive())
        QCoreApplication::instance()->processEvents();

    QCOMPARE(spyGPIOChanged.count(), 2);
    QList<QVariant> arguments = spyGPIOChanged.takeFirst();

    for (int i =0; i < 5; ++i) {
        QCOMPARE(arguments.at(0).toInt(), 123456);
        QCOMPARE(arguments.at(1).toInt(), i);
        QCOMPARE(arguments.at(2).toInt(), 1);
    }

    delete m_client;
    m_client = 0;
}

void networkModuleSimulationTest::initSimulator()
{
    QTimer timer;
    timer.setSingleShot(true);
    m_client = new clientSimulator();
    QSignalSpy spyNewData(m_client->socket(), &QTcpSocket::stateChanged);
    QSignalSpy spyInitID(m_server, &moduleServer::newModuleConnected);

    // connect to server, timeout 3 seconds
    m_client->connectToServer();
    timer.start(3000);

    while (!m_client->isConnected() && timer.isActive())
        QCoreApplication::instance()->processEvents();

    //QAbstractSocket::HostLookupState
    //QAbstractSocket::ConnectingState
    //QAbstractSocket::ConnectedState
    QVERIFY(spyNewData.count() == 3);
    QVERIFY(m_client->isConnected());
    QCOMPARE(m_server->connectedClients(), 1);

    // wait for the simulator to send his ID
    timer.start(10);
    while (timer.isActive())
        QCoreApplication::instance()->processEvents();

    // make sure the simulator sent his ID
    QCOMPARE(spyInitID.count(), 1);
    QList<QVariant> arguments = spyInitID.takeFirst();
    QVERIFY(arguments.at(0).toInt() == 123456);

    m_server->resetLineCounter();
}

QTEST_MAIN(networkModuleSimulationTest)

#include "tst_networkModuleSimulation.moc"
