#include "QHFDeviceThread.h"
#include "RF24.h"

#include <QTimer>
#include <QByteArray>
#include <QDebug>
#include <QtGlobal>

#define PIPE_PREFIX 0xAABBCCDD00

const quint64 radioModulePipes[12] =
{
    // Write pipes
     0x00000000A1 + PIPE_PREFIX,
     0x00000000A2 + PIPE_PREFIX,
     0x00000000A3 + PIPE_PREFIX,
     0x00000000A4 + PIPE_PREFIX,
     0x00000000A5 + PIPE_PREFIX,
     0x00000000A6 + PIPE_PREFIX,

    // Read pipes
     0x00000000B1 + PIPE_PREFIX,
     0x00000000B2 + PIPE_PREFIX,
     0x00000000B3 + PIPE_PREFIX,
     0x00000000B4 + PIPE_PREFIX,
     0x00000000B5 + PIPE_PREFIX,
     0x00000000B6 + PIPE_PREFIX,
};

QHFDeviceThread::QHFDeviceThread() :
    QObject(),
    m_stopRequested(false),
    m_listeningTime(500)
{
}

void QHFDeviceThread::setListeningTime(const unsigned int ms)
{
    m_listeningTime = ms;
}

void QHFDeviceThread::mainRadioLoop(RF24 *radioModule)
{
    qDebug() << "loop";

    radioModule->openReadingPipe(0,radioModulePipes[6]);
    radioModule->openReadingPipe(1,radioModulePipes[7]);
    radioModule->openReadingPipe(2,radioModulePipes[8]);
    radioModule->openReadingPipe(3,radioModulePipes[9]);
    radioModule->openReadingPipe(4,radioModulePipes[10]);
    radioModule->openReadingPipe(5,radioModulePipes[11]);

    while (!m_stopRequested) {

        // Listen for new data
        radioModule->startListening();
       for (int i = 0; i < 100; ++i) {
            listenForNewData(radioModule);
        }

        // Write data
        radioModule->stopListening();
        while (m_writeQueue.count())
            write(radioModule);
    }
    delete radioModule;
    Q_EMIT finished(tr("Radio module thread finished."), false);
}

void QHFDeviceThread::requestWrite(const QString &data, const quint64 pipe)
{
    QPair<quint64, QString> payload;
    payload.first = pipe;
    payload.second = data;

    m_writeQueue.append(payload);
}


void QHFDeviceThread::stop()
{
    m_stopRequested = true;
}

void QHFDeviceThread::listenForNewData(RF24 *radioModule)
{
    quint8 pipe;
    if (radioModule->available(&pipe)) {
        qDebug() << "Data available!";
        char buff[50];
        int len = radioModule->getDynamicPayloadSize();
        QByteArray data(buff, len);
        if (!radioModule->read(buff, len)) {
            Q_EMIT error(tr("Reading from pipe %1 failed.").arg(QString::number(pipe, 16)));
        }
        Q_EMIT newData(QString(data), PIPE_PREFIX & pipe);
    } else {
        delayMicroseconds(m_listeningTime * 1000 / 100);
    }
}

void QHFDeviceThread::write(RF24 *radioModule)
{
    for (int i = 0; i < m_writeQueue.count(); ++i) {
        QPair<quint64, QString> data = m_writeQueue.at(i);
        radioModule->openWritingPipe(data.first);
        if (!radioModule->write(qPrintable(data.second), data.second.length())) {
            Q_EMIT error(tr("Writing to pipe %1 failed.").arg(QString::number(data.first, 16)));
        }
        m_writeQueue.remove(i);
    }
}
