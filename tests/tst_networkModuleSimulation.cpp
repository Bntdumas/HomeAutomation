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
}

void networkModuleSimulationTest::cleanup()
{
    delete m_server;
}

// simple test,
void networkModuleSimulationTest::testSendLotsOfLines()
{
    initSimulator();

    const int messageToSend = 100;
    //have the client send lots of lines and make sure the server got them all
    for (int i = 0; i < messageToSend; ++i) {
       QVERIFY(m_client->sendID());
       QCoreApplication::instance()->processEvents();
    }

    QTimer timer;
    timer.setSingleShot(true);
    timer.start(3000);
    while (timer.isActive())
        QCoreApplication::instance()->processEvents();

    QCOMPARE(m_server->receivedLines(), messageToSend);

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
