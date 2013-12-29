#include "QHFDevice.h"
#include "QHFDeviceThread.h"
#include "RF24.h"

#include <QDebug>
#include <QTimer>

QHFDevice::QHFDevice(QObject *parent) :
    QObject(parent)
  ,m_terminated(false)
{
}

QHFDevice::~QHFDevice()
{
    qDebug() << "Stopping thread";
    terminateRadioModule();
    qDebug() << "Thread stopped";
}


void QHFDevice::initModule()
{
    RF24 *radioModule = new RF24(RPI_V2_GPIO_P1_15, RPI_V2_GPIO_P1_26, BCM2835_SPI_SPEED_8MHZ);

    // radio module configuration
    if (!radioModule->begin()) {
        Q_EMIT error(tr("The radio module couldn't be initialized"));
        return;
    }

    radioModule->enableDynamicPayloads();
    radioModule->setRetries(5,15);

    if (!radioModule->setDataRate(RF24_1MBPS)) {
        Q_EMIT error(tr("The radio module data rate couldn't be set"));
        return;
    }

    radioModule->setPALevel(RF24_PA_MAX);
    radioModule->setChannel(100);
    radioModule->setCRCLength(RF24_CRC_16);

    m_moduleWorker = new QHFDeviceThread(0);
    m_moduleWorker->moveToThread(&m_moduleThread);
    connect(&m_moduleThread, SIGNAL(finished()), m_moduleWorker, SLOT(deleteLater()));
    connect(m_moduleWorker, SIGNAL(newData(QString,quint64)), this, SIGNAL(newData(QString,quint64)), Qt::QueuedConnection);
    connect(this, SIGNAL(startThread(RF24*)), m_moduleWorker, SLOT(mainRadioLoop(RF24*)), Qt::QueuedConnection);
    m_moduleThread.start();


#ifdef TESTING
    qDebug() << "Testing mode. the radio module will be up for 5seconds.";

    m_radioModuleLifeTimer = new QTimer();
    m_radioModuleLifeTimer->setSingleShot(true);
    connect(m_radioModuleLifeTimer, SIGNAL(timeout()), this, SLOT(terminateRadioModule()));
    m_radioModuleLifeTimer->start(10000);
#endif

    Q_EMIT startThread(radioModule);
}

void QHFDevice::requestWrite(const QString &data, const quint64 pipe)
{
    m_moduleWorker->requestWrite(data, pipe);
}

void QHFDevice::terminateRadioModule()
{
    m_moduleWorker->stop();
    m_moduleThread.exit();
    m_moduleThread.wait();
    m_terminated = true;
}
