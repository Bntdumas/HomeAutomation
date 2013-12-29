#include "QHFDeviceThread.h"
#include "RF24.h"

#include <QTimer>
#include <QByteArray>
#include <QDebug>
#include <QtGlobal>

QHFDeviceThread::QHFDeviceThread(const quint16 nodeAddress) :
    QObject(),
    m_stopRequested(false),
    m_listeningTime(1000),
    m_nodeAddress(nodeAddress)
{
}

void QHFDeviceThread::setListeningTime(const unsigned int ms)
{
    m_listeningTime = ms;
}

void QHFDeviceThread::mainRadioLoop(RF24 *radioModule)
{
    qDebug() << "loop";


    RF24Network network(*radioModule);
    network.begin(90, m_nodeAddress);
    radioModule->printDetails();

    while (!m_stopRequested) {

        network.update();

        // Listen for new data
       for (int i = 0; i < 100; ++i) {
            listenForNewData(network);
        }

        // Write data
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

void QHFDeviceThread::listenForNewData(RF24Network networkNode)
{
    if (networkNode.available()) {
        qDebug() << "Data available!";
        RF24NetworkHeader header;
        char buff[50];
        QByteArray data(buff, sizeof(buff));
        if (!networkNode.read(header, buff, sizeof(buff))) {
            Q_EMIT error(tr("Reading failed."));
        }
        qDebug() << "Header: " << header.toString();
        Q_EMIT newData(QString(data), header.from_node);
    }
    delayMicroseconds(m_listeningTime * 1000 / 100);
}

void QHFDeviceThread::write(RF24 *radioModule)
{
//    for (int i = 0; i < m_writeQueue.count(); ++i) {
//        QPair<quint64, QString> data = m_writeQueue.at(i);
//        radioModule->openWritingPipe(data.first);
//        if (!radioModule->write(qPrintable(data.second), data.second.length())) {
//            Q_EMIT error(tr("Writing to pipe %1 failed.").arg(QString::number(data.first, 16)));
//        }
//        m_writeQueue.remove(i);
//    }
}
