#include <QString>
#include <QtTest>

class testServer : public QObject
{
    Q_OBJECT
    
public:
    testServer();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
};

testServer::testServer()
{
}

void testServer::initTestCase()
{
}

void testServer::cleanupTestCase()
{
}

void testServer::testCase1()
{
    QVERIFY2(true, "Failure");
}

QTEST_APPLESS_MAIN(testServer)

#include "tst_testServer.moc"
