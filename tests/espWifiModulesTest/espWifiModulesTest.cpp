#include "espWifiModulesTest.h"
#include "ui_espWifiModulesTest.h"
#include "moduleServer.h"

#include <QDateTime>

espWifiModulesTest::espWifiModulesTest(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::espWifiModulesTest)
  ,m_server(Q_NULLPTR)
  ,m_devicesCount(0)
{
    ui->setupUi(this);
    ui->txtLog->setReadOnly(true);

    // ip | ID | connected since | reconnections
    ui->tblViewDevicesConnected->setColumnCount(4);
    QStringList headers;
    headers << QStringLiteral("IP") << QStringLiteral("ID") <<
               QStringLiteral("Connected since") << QStringLiteral("Reconnections");
    ui->tblViewDevicesConnected->setHorizontalHeaderLabels(headers);

    qRegisterMetaType<utils::messageType>("utils::messageType");    // Register types

    m_server = new moduleServer(this);
    m_server->setAutomaticPolling(true);
    connect(m_server, &moduleServer::message, this, &espWifiModulesTest::displayLogMessage);
    connect(m_server, &moduleServer::newClient, this, &espWifiModulesTest::deviceConnectedIP);
    connect(m_server, &moduleServer::newModuleConnected, this, &espWifiModulesTest::deviceConnectedID);

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
        ui->txtLog->setTextColor(Qt::darkGray);
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

        // ID (to be filled out later
        // ui->tblViewDevicesConnected->setItem(lastRow, 0, new QTableWidgetItem());

        // Connected since now
        ui->tblViewDevicesConnected->setItem(lastRow, 2,
                                             new QTableWidgetItem(QTime::currentTime().toString(QStringLiteral("hh:mm:ss:zzz"))));

        // Reconnections == 0
        ui->tblViewDevicesConnected->setItem(lastRow, 3
                                             , new QTableWidgetItem(QString::number(0)));
    } else {
        incrementConnectionCount(existingRow);
        ui->tblViewDevicesConnected->setItem(lastRow, 2,
                                             new QTableWidgetItem(QTime::currentTime().toString(QStringLiteral("hh:mm:ss:zzz"))));

    }
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
    QTableWidgetItem *itm = ui->tblViewDevicesConnected->item(row, 3);
    if (!itm){
        return ;
    }
    bool ok;
    const int currentID = itm->text().toInt(&ok) + 1;
    if (!ok) {
        displayLogMessage(tr("espWifiModulesTest: The reconnections  count %1 could not be converted to int").arg(ui->tblViewDevicesConnected->item(row, 3)->text()), utils::SoftwareError);
        return;
    }

    QTableWidgetItem *recoCell = new QTableWidgetItem(QString::number(currentID));
    ui->tblViewDevicesConnected->setItem(row, 3, recoCell);
}
