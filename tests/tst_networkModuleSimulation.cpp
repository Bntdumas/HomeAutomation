#include <QString>
#include <QtTest>
#include <QCoreApplication>
#include <QTcpSocket>

#include "moduleServer.h"
#include "clientSimulator.h"
#include "debugCenter.h"


namespace {
static const int simulatorID = 123456;
}

class networkModuleSimulationTest : public QObject
{
    Q_OBJECT

public:
    networkModuleSimulationTest() : m_serverWatcher(Q_NULLPTR), m_server(Q_NULLPTR)  {}

private Q_SLOTS:
    void init();
    void cleanup();

    void testSendLotsOfLines();
    void testSendIDRequest();
    void testNoIDOnConnect();
    void testSendDataRequestToAllModules();
    void testSendDataRequestToSingleModule();
    void testSetGPIO();

private:
    moduleServer *m_server;
    clientSimulator *m_client;
    debugCenter *m_serverWatcher;

    void initSimulator();
};

void networkModuleSimulationTest::init()
{
    m_server = new moduleServer();
    m_serverWatcher = new debugCenter(this);
    m_serverWatcher->addWatcher<moduleServer>(m_server);

    m_server->createTCPServer();
    QVERIFY(m_server->isListening());
    m_server->setAutomaticPolling(false);
}

void networkModuleSimulationTest::cleanup()
{
    delete m_server;
    m_server = Q_NULLPTR;
    if (m_serverWatcher) {
        delete m_serverWatcher;
        m_serverWatcher = Q_NULLPTR;
    }
}

// simple test,
void networkModuleSimulationTest::testSendLotsOfLines()
{
    /*
     * This test starts a server and a client, then sends 100 requests for the client to reset.
     * For each reset command sent, we want an answer, so we expect 100 answers
     */
    m_serverWatcher->setDebugVisible(false);
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
    m_serverWatcher->setDebugVisible(false);
    initSimulator();

    QSignalSpy spyID(m_server, &moduleServer::newModuleConnected);

    QVERIFY(m_server->requestIDFromModules());
    QTimer timer;
    timer.setSingleShot(true);
    timer.start(10);
    while (timer.isActive())
        QCoreApplication::instance()->processEvents();


    QVERIFY(m_server->requestIDFromModule(simulatorID));
    timer.start(10);
    while (timer.isActive())
        QCoreApplication::instance()->processEvents();

    QCOMPARE(spyID.count(), 2);
    QList<QVariant> arguments = spyID.takeFirst();
    QVERIFY(arguments.at(0).toInt() == simulatorID);
    arguments = spyID.takeLast();
    QVERIFY(arguments.at(0).toInt() == simulatorID);

    delete m_client;
    m_client = 0;
}

void networkModuleSimulationTest::testNoIDOnConnect()
{
    // client needs to send ID on connect, check safety in case the client doesn't send it.
    QTimer timer;
    timer.setSingleShot(true);
    m_client = new clientSimulator();
    m_client->sendIDOnConnect(false);

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
    timer.start(6000);
    while (timer.isActive())
        QCoreApplication::instance()->processEvents();

    // make sure the simulator his ID
    QCOMPARE(spyInitID.count(), 0);
}

void networkModuleSimulationTest::testSendDataRequestToAllModules()
{
    initSimulator();

    QSignalSpy spyGPIOChanged(m_server, &moduleServer::gpioChanged);
    QSignalSpy spysensorUpdated(m_server, &moduleServer::sensorUpdated);

    QVERIFY(m_server->requestDataFromModules());
    QTimer timer;
    timer.setSingleShot(true);
    timer.start(100);
    while (timer.isActive())
        QCoreApplication::instance()->processEvents();

    QCOMPARE(spyGPIOChanged.count(), 5);

    for (int i =0; i < 5; ++i) {
        QList<QVariant> arguments = spyGPIOChanged.at(i);
        QCOMPARE(arguments.at(0).toInt(), simulatorID);
        QCOMPARE(arguments.at(1).toInt(), i);
        QCOMPARE(arguments.at(2).toInt(), 1);
    }

    QCOMPARE(spysensorUpdated.count(), 4);

    QList<QVariant> arguments = spysensorUpdated.at(0);
    QCOMPARE(arguments.at(0).toInt(), simulatorID);
    QCOMPARE(arguments.at(1).toInt(), 5);
    QCOMPARE(arguments.at(2).toInt(), static_cast<int>(devices::Temperature));
    QCOMPARE(arguments.at(3).toDouble(), 22.5);

    arguments = spysensorUpdated.at(1);
    QCOMPARE(arguments.at(0).toInt(), simulatorID);
    QCOMPARE(arguments.at(1).toInt(), 6);
    QCOMPARE(arguments.at(2).toInt(), static_cast<int>(devices::Temperature));
    QCOMPARE(arguments.at(3).toDouble(), 22.5);

    arguments = spysensorUpdated.at(2);
    QCOMPARE(arguments.at(0).toInt(), simulatorID);
    QCOMPARE(arguments.at(1).toInt(), 7);
    QCOMPARE(arguments.at(2).toInt(), static_cast<int>(devices::Light));
    QCOMPARE(arguments.at(3).toDouble(), static_cast<double>(1));

    arguments = spysensorUpdated.at(3);
    QCOMPARE(arguments.at(0).toInt(), simulatorID);
    QCOMPARE(arguments.at(1).toInt(), 8);
    QCOMPARE(arguments.at(2).toInt(), static_cast<int>(devices::Humidity));
    QCOMPARE(arguments.at(3).toDouble(), static_cast<double>(0));
    delete m_client;
    m_client = 0;
}

void networkModuleSimulationTest::testSendDataRequestToSingleModule()
{
    initSimulator();

    QSignalSpy spyGPIOChanged(m_server, &moduleServer::gpioChanged);
    QSignalSpy spysensorUpdated(m_server, &moduleServer::sensorUpdated);

    QVERIFY(m_server->requestDataFromModule(simulatorID));
    QTimer timer;
    timer.setSingleShot(true);
    timer.start(100);
    while (timer.isActive())
        QCoreApplication::instance()->processEvents();

    QCOMPARE(spyGPIOChanged.count(), 5);

    for (int i =0; i < 5; ++i) {
        QList<QVariant> arguments = spyGPIOChanged.at(i);
        QCOMPARE(arguments.at(0).toInt(), simulatorID);
        QCOMPARE(arguments.at(1).toInt(), i);
        QCOMPARE(arguments.at(2).toInt(), 1);
    }

    QCOMPARE(spysensorUpdated.count(), 4);

    QList<QVariant> arguments = spysensorUpdated.at(0);
    QCOMPARE(arguments.at(0).toInt(), simulatorID);
    QCOMPARE(arguments.at(1).toInt(), 5);
    QCOMPARE(arguments.at(2).toInt(), static_cast<int>(devices::Temperature));
    QCOMPARE(arguments.at(3).toDouble(), 22.5);

    arguments = spysensorUpdated.at(1);
    QCOMPARE(arguments.at(0).toInt(), simulatorID);
    QCOMPARE(arguments.at(1).toInt(), 6);
    QCOMPARE(arguments.at(2).toInt(), static_cast<int>(devices::Temperature));
    QCOMPARE(arguments.at(3).toDouble(), 22.5);

    arguments = spysensorUpdated.at(2);
    QCOMPARE(arguments.at(0).toInt(), simulatorID);
    QCOMPARE(arguments.at(1).toInt(), 7);
    QCOMPARE(arguments.at(2).toInt(), static_cast<int>(devices::Light));
    QCOMPARE(arguments.at(3).toDouble(), static_cast<double>(1));

    arguments = spysensorUpdated.at(3);
    QCOMPARE(arguments.at(0).toInt(), simulatorID);
    QCOMPARE(arguments.at(1).toInt(), 8);
    QCOMPARE(arguments.at(2).toInt(), static_cast<int>(devices::Humidity));
    QCOMPARE(arguments.at(3).toDouble(), static_cast<double>(0));

    delete m_client;
    m_client = 0;
}

void networkModuleSimulationTest::testSetGPIO()
{
    initSimulator();

    //QSignalSpy spyID(m_server, &moduleServer::newModuleConnected);

    QVERIFY(m_server->setModuleGPIO(simulatorID, 3, false));
    QTimer timer;
    timer.setSingleShot(true);
    timer.start(10);
    while (timer.isActive())
        QCoreApplication::instance()->processEvents();

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
    QVERIFY(arguments.at(0).toInt() == simulatorID);

    m_server->resetLineCounter();
}

QTEST_MAIN(networkModuleSimulationTest)

#include "tst_networkModuleSimulation.moc"
