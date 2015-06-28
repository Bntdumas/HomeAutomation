#include "espWifiModulesTest.h"
#include "ui_espWifiModulesTest.h"
#include "moduleServer.h"

#include <QDateTime>
#include <QTimer>

espWifiModulesTest::espWifiModulesTest(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::espWifiModulesTest)
  ,m_server(Q_NULLPTR)
  ,m_devicesCount(0)
{
    ui->setupUi(this);
    ui->txtLog->setReadOnly(true);

    // ip | ID | connected since | reconnections | frameSent | frameReceived
    ui->tblViewDevicesConnected->setColumnCount(7);
    QStringList headers;
    headers << QStringLiteral("IP") << QStringLiteral("ID")
            <<  QStringLiteral("Connected since") << QStringLiteral("Reconnections")
            << QStringLiteral("Frames sent") << QStringLiteral("Frames received")
            << QStringLiteral("Catastrophic failures");
    ui->tblViewDevicesConnected->setHorizontalHeaderLabels(headers);

    qRegisterMetaType<utils::messageType>("utils::messageType");    // Register types

    m_pollingTimer = new QTimer(this);
    m_pollingTimer->setInterval(10);
    m_pollingTimer->setSingleShot(false);
    connect(m_pollingTimer, &QTimer::timeout, this, &espWifiModulesTest::pollingTimerTimeout);
    m_pollingTimer->start();

    m_server = new moduleServer(this);
    m_server->setAutomaticPolling(true);
    connect(m_server, &moduleServer::message, this, &espWifiModulesTest::displayLogMessage);
    connect(m_server, &moduleServer::newClient, this, &espWifiModulesTest::deviceConnectedIP);
    connect(m_server, &moduleServer::newModuleConnected, this, &espWifiModulesTest::deviceConnectedID);
    connect(m_server, &moduleServer::gpioChanged, this, &espWifiModulesTest::gpioChanged);

    on_action_re_start_triggered();
}

espWifiModulesTest::~espWifiModulesTest()
{
    delete ui;
}

void espWifiModulesTest::displayLogMessage(const QString &text, const utils::messageType type)
{
    switch (type) {
    case utils::Info:
        return; //ui->txtLog->setTextColor(Qt::darkGray);
        break;

    case utils::Success:
        ui->txtLog->setTextColor(Qt::darkGreen);
        break;

    case utils::Warning:
        ui->txtLog->setTextColor(Qt::darkYellow);
        break;

    case utils::SoftwareError:
        ui->txtLog->setTextColor(Qt::darkRed);
        break;

    case utils::NetworkError:
        ui->txtLog->setTextColor(Qt::darkMagenta);
        break;
    }
    ui->txtLog->append(QStringLiteral("%1: %2").arg(QDateTime::currentDateTime().toString(QStringLiteral("hh:mm:ss"))).arg(text));
}

void espWifiModulesTest::on_action_re_start_triggered()
{
    m_server->createTCPServer();
    m_server->setAutomaticPolling(true);
}

void espWifiModulesTest::deviceConnectedID(QVariant deviceID)
{
    const QHostAddress ip = m_server->clientIPFromID(deviceID);
    const int existingRow = rowForIP(ip);
    if (existingRow != -1) {
        ui->tblViewDevicesConnected->setItem(existingRow, 1, new QTableWidgetItem(deviceID.toString()));
    } else {
        displayLogMessage(tr("espWifiModulesTest: Got a device ID %1 that can not be mapped to a device IP").arg(deviceID.toString()), utils::SoftwareError);
    }
}

void espWifiModulesTest::deviceConnectedIP(QHostAddress ip)
{
    const int existingRow = rowForIP(ip);
    const int lastRow = ui->tblViewDevicesConnected->rowCount();

    if (existingRow == -1) { //new device

        ui->tblViewDevicesConnected->setRowCount(++m_devicesCount);

        // IP
        ui->tblViewDevicesConnected->setItem(lastRow, 0, new QTableWidgetItem(ip.toString()));

        // Connected since now
        ui->tblViewDevicesConnected->setItem(lastRow, 2,
                                             new QTableWidgetItem(QTime::currentTime().toString(QStringLiteral("hh:mm:ss:zzz"))));

        // Reconnections, frames sent, fraes received == 0
        ui->tblViewDevicesConnected->setItem(lastRow, 3
                                             , new QTableWidgetItem(QString::number(0)));

        ui->tblViewDevicesConnected->setItem(lastRow, 4
                                             , new QTableWidgetItem(QString::number(0)));

        ui->tblViewDevicesConnected->setItem(lastRow, 5
                                             , new QTableWidgetItem(QString::number(0)));

        ui->tblViewDevicesConnected->setItem(lastRow, 6
                                             , new QTableWidgetItem(QString::number(0)));

        ui->tblViewDevicesConnected->resizeColumnsToContents();
    } else {
        m_waitingForAnswer.clear();
        incrementConnectionCount(existingRow);
        ui->tblViewDevicesConnected->setItem(lastRow, 2,
                                             new QTableWidgetItem(QTime::currentTime().toString(QStringLiteral("hh:mm:ss:zzz"))));

    }
}

void espWifiModulesTest::gpioChanged(int moduleID, int gpioPin, bool state)
{
    incrementFrameReceivedCount(moduleID);
    if (state != m_waitingForAnswer.value(moduleID)) {
        displayLogMessage(tr("espWifiModulesTest: ========= DATA CORRUPTED ===== sent a gpio value and received another"), utils::SoftwareError);
        incrementCatastrophicFailCount(moduleID);
    }

    if (gpioPin != 4) {
        displayLogMessage(tr("espWifiModulesTest: ========= DATA CORRUPTED ===== wrong gpio pin"), utils::SoftwareError);
        incrementCatastrophicFailCount(moduleID);
    }
    m_waitingForAnswer.remove(moduleID);
}

void espWifiModulesTest::pollingTimerTimeout()
{
    trySendGpioFrame(10557940);
    trySendGpioFrame(16670068);
    trySendGpioFrame(16670915);
    trySendGpioFrame(16669492);
}

int espWifiModulesTest::rowForIP(QHostAddress ip)
{
    const QString ipStr = ip.toString();
    for (int i = 0; i < ui->tblViewDevicesConnected->rowCount(); ++i) {
        QTableWidgetItem *itm = ui->tblViewDevicesConnected->item(i, 0);
        if (itm && itm->text() == ipStr)
            return i;
    }
    return -1;
}

int espWifiModulesTest::rowForID(int deviceID)
{
    for (int i = 0; i < ui->tblViewDevicesConnected->rowCount(); ++i) {
        QTableWidgetItem *itm = ui->tblViewDevicesConnected->item(i, 1);
        if (!itm){
            return -1;
        }
        bool ok;
        const int currentID = itm->text().toInt(&ok);
        if (!ok) {
            displayLogMessage(tr("espWifiModulesTest: The device ID %1 could not be converted to int").arg(ui->tblViewDevicesConnected->item(i, 1)->text()), utils::SoftwareError);
            return -1;
        }
        if (currentID == deviceID)
            return i;
    }
    return -1;
}

void espWifiModulesTest::incrementConnectionCount(int row)
{
    incrementCell(row, 3);
}

void espWifiModulesTest::incrementFrameReceivedCount(int chipID)
{
    const int row = rowForID(chipID);
    incrementCell(row, 5);

}

void espWifiModulesTest::incrementFrameSentCount(int chipID)
{
    const int row = rowForID(chipID);
    incrementCell(row, 4);
}

void espWifiModulesTest::incrementCatastrophicFailCount(int chipID)
{
    const int row = rowForID(chipID);
    incrementCell(row, 6);
}

void espWifiModulesTest::incrementCell(int row, int column)
{
    QTableWidgetItem *itm = ui->tblViewDevicesConnected->item(row, column);
    if (!itm){
        return;
    }
    bool ok;
    const int currentID = itm->text().toInt(&ok) + 1;
    if (!ok) {
        displayLogMessage(tr("espWifiModulesTest: The cell value %1 could not be converted to int").arg(ui->tblViewDevicesConnected->item(row, 3)->text()), utils::SoftwareError);
        return;
    }

    QTableWidgetItem *recoCell = new QTableWidgetItem(QString::number(currentID));
    ui->tblViewDevicesConnected->setItem(row, column, recoCell);
}

void espWifiModulesTest::trySendGpioFrame(int chipID)
{
    bool state = (qrand() % 2) == 1;
    if (!m_waitingForAnswer.contains(chipID) && rowForID(chipID) != -1) {
        m_server->sendTestGPIOSwitch(chipID, state);
        incrementFrameSentCount(chipID);
        m_waitingForAnswer.insert(chipID, state);
    }
}
