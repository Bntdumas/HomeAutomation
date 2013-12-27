#ifndef QHFDEVICETHREAD_H
#define QHFDEVICETHREAD_H

#include <QObject>
#include <QVector>
#include <QPair>

/**
 * @brief Handle the communication loop for the radio module.
 */

class RF24;

class QHFDeviceThread : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief QHFDeviceThread
     */
    explicit QHFDeviceThread();

    /**
     * @brief Set the time spent listening for new data.
     * @param ms: the amount of time in milliseconds
     * @note: default is 500ms
     */
    void setListeningTime(const unsigned int ms);

Q_SIGNALS:
    /**
     * @brief Emitted when the thread exits the main loop.
     * @param message: a human redable message giving the reason
     * @param error: true if the thread died because of an error
     */
    void finished(const QString &message, const bool error);

    /**
     * @brief emitted when new data is available.
     * @param data: the received data
     * @param pipe: the pipe on which the data is aavilable
     */
    void newData(const QString &data, quint64 pipe);

    /**
     * @brief Emitted if an error occured
     * @param message: a text describing the error
     */
    void error(const QString &message);
    
public Q_SLOTS:
    /**
     * @brief Thread main loop.
     * - Listen for a determined amount of time (set with \b setListeningTime), if data is available on either of the pipes,
     *   retreive the data and it's corresponding pipe and emit \b newData(const QString &data, quint64 pipe)
     * - After the listening, check for new data to write on the pipes.
     *
     * @param radioModule: a configured RF24 radio module
     */
    void mainRadioLoop(RF24 *radioModule);

    /**
     * @brief Write data to the passed pipe (using Queue)
     * @param data: the data to write
     * @param pipe: the pipe to write into
     */
    void requestWrite(const QString &data, const quint64 pipe);

    /**
     * @brief Ask the thread to finish execution.
     */
    void stop();

private:
    /**
     * @brief A queue containing the data to write into the radio pipes
     */
    QVector< QPair<quint64, QString> > m_writeQueue;

    /**
     * @brief used to exit the main loop on user request.
     */
    bool m_stopRequested;

    /**
     * @brief Time spent listening for new data available for each loop iteration
     */
    int m_listeningTime;

    /**
     * @brief Listen for new data on the pipes and emit \b if available.
     * @param radioModule: the radio module instance
     */
    void listenForNewData(RF24 *radioModule);

    /**
     * @brief write all data from the \b m_writeQueue queue.
     * @param radioModule: the radio module instance
     */
    void write(RF24 *radioModule);

    
};

#endif // QHFDEVICETHREAD_H
