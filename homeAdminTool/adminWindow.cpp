#include "adminWindow.h"
#include "ui_adminWindow.h"

#include "databaseAdmin.h"

#include <QDateTime>
#include <QThread>

adminWindow::adminWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::adminWindow)
{
    ui->setupUi(this);

    // Register types
    qRegisterMetaType<adminToolItem::messageType>("adminToolItem::messageType");

    m_databaseAdmin = new databaseAdmin();
    connect(m_databaseAdmin, SIGNAL(message(QString, adminToolItem::messageType)), this, SLOT(displayLogMessage(QString, adminToolItem::messageType)));
    m_databaseAdmin->connectToDB();
}

adminWindow::~adminWindow()
{
    delete ui;
}

void adminWindow::displayLogMessage(const QString &text, const adminToolItem::messageType type)
{
    switch (type) {
    case adminToolItem::Info:
        ui->txtLog->setTextColor(Qt::darkGray);
        break;

    case adminToolItem::Success:
        ui->txtLog->setTextColor(Qt::darkGreen);
        break;

    case adminToolItem::Warning:
        ui->txtLog->setTextColor(Qt::darkYellow);
        break;

    case adminToolItem::DatabaseError:
        ui->txtLog->setTextColor(Qt::darkMagenta);
        break;

    case adminToolItem::SoftwareError:
        ui->txtLog->setTextColor(Qt::darkRed);
        break;
    }
    ui->txtLog->append(QStringLiteral("%1: %2").arg(QDateTime::currentDateTime().toString(QStringLiteral("hh:mm:ss"))).arg(text));
}


void adminWindow::on_actionCreate_house_structure_triggered()
{
    m_databaseAdmin->createHouseStructure();
}

void adminWindow::on_actionCreate_Update_devices_types_triggered()
{
    m_databaseAdmin->createDevicesTypes();
}

void adminWindow::on_actionCreate_Update_tables_triggered()
{
    m_databaseAdmin->createTables();
}

void adminWindow::on_actionDrop_tables_triggered()
{
    m_databaseAdmin->deleteTables();
}

void adminWindow::on_actionReset_database_triggered()
{
    if (!m_databaseAdmin->deleteTables())
        return;
    if (!m_databaseAdmin->createTables())
        return;
    if (!m_databaseAdmin->createDevicesTypes())
        return;
    if (!m_databaseAdmin->createHouseStructure())
        return;
}
