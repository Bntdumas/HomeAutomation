#ifndef QHFDEVICE_H
#define QHFDEVICE_H

#include <QObject>
#include "qhfdevice_global.h"

#include <QThread>

#define TESTING

/**
 * @brief This library handle communication with a NRF24L01 module, using the C++ library found here:
 * https://github.com/stanleyseow/RF24
 * You need to make/install it to link it against this lib.
 */

class QThread;
class QTimer;
class RF24;
class QHFDeviceThread;

class QHFDEVICESHARED_EXPORT QHFDevice: public QObject
{
    Q_OBJECT
public:
    explicit QHFDevice(QObject *parent = 0);
    ~QHFDevice();

    /**
     * @brief Initialize the NRF24L01 module to act as a hub in a star network
     */
    void initModule();

public Q_SLOTS:
    /**
     * @brief Write data to the passed pipe (using Queue)
     * @param data: the data to write
     * @param pipe: the pipe to write into
     */
    void requestWrite(const QString &data, const quint64 pipe);

    /**
     * @brief Stop the radio module loop and destroy the thread.
     * @note delete the RF24* passed to the main loop.
     */
    void terminateRadioModule();

Q_SIGNALS:
    /**
     * @brief Emitted if an error occured
     * @param message: a text describing the error
     */
    void error(const QString &message);

    /**
     * @brief emitted when new data is available.
     * @param data: the received data
     * @param pipe: the pipe on which the data is aavilable
     */
    void newData(const QString &data, quint64 pipe);

    /**
     * @brief Starts the main loop of the radio module thread
     */
    void startThread(RF24 *radioModule);

private:
    /**
     * @brief The radio module loop
     */
    QThread m_moduleThread;
    QHFDeviceThread *m_moduleWorker;

#ifdef TESTING
private:
    QTimer *m_radioModuleLifeTimer;
    bool m_terminated;
public:
    /**
     * @brief Test the state of the thread
     * @return true if thread is running
     */
    bool threadTerminated() const {return m_terminated;}
#endif
};

#endif // QHFDEVICE_H
