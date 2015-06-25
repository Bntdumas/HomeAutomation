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
    initSimulator();

    const int messagesToSend = 100;
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

void networkModuleSimulationTest::initSimulator()
{
    QTimer timer;
    timer.setSingleShot(true);
    m_client = new clientSimulator();
    QSignalSpy spyNewData(m_client->socket(), SIGNAL(stateChanged(QAbstractSocket::SocketState)));

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
}

QTEST_MAIN(networkModuleSimulationTest)

#include "tst_networkModuleSimulation.moc"
