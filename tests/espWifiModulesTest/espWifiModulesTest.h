#ifndef ESPWIFIMODULETEST_H
#define ESPWIFIMODULETEST_H

#include <QMainWindow>
#include "utils.h"

#include <QHostAddress>


namespace Ui {
class espWifiModulesTest;
}

class moduleServer;
class QTimer;

/**
 * @brief The home automation admin app.
 */
class espWifiModulesTest : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit espWifiModulesTest(QWidget *parent = 0);
    ~espWifiModulesTest();
    
private Q_SLOTS:
    /**
     * @brief Displays a message with color code and timestamp.
     * @param The message text
     * @param The message type
     */
    void displayLogMessage(const QString &text, const utils::messageType type);
    void on_action_re_start_triggered();
    void deviceConnectedID(QVariant deviceID);
    void deviceConnectedIP(QHostAddress ip);
    void gpioChanged(int moduleID, int gpioPin, bool state);

    void pollingTimerTimeout();

private:
    Ui::espWifiModulesTest *ui;
    moduleServer *m_server;

    int m_devicesCount;

    /**
     * @brief to retrieve in which row the device is
     */
    int rowForIP(QHostAddress ip);
    int rowForID(int deviceID);

    void incrementConnectionCount(int row);
    void incrementFrameReceivedCount(int chipID);
    void incrementFrameSentCount(int chipID);
    void incrementCatastrophicFailCount(int chipID);
    void incrementCell(int row, int column);

    void trySendGpioFrame(int chipID);

    QMap<int, bool> m_waitingForAnswer;

    QTimer *m_pollingTimer;

};

#endif // ESPWIFIMODULETEST_H
