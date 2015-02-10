#include "adminWindow.h"
#include "ui_adminWindow.h"

#include "databaseAdmin.h"
#include "atmelProgrammer.h"
#include "moduleWizard.h"

#include <QDateTime>
#include <QThread>
#include <QStringList>
#include <QDebug>

adminWindow::adminWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::adminWindow)
{
    ui->setupUi(this);

    connect(ui->actionCompileSketch, SIGNAL(triggered()), this, SLOT(onActioncompileSketchTriggered()));
    connect(ui->actionCreatehousestructure, SIGNAL(triggered()), this, SLOT(onActioncreateHouseStructureTriggered()));
    connect(ui->actionCreateUpdatedevicestypes, SIGNAL(triggered()), this, SLOT(onActioncreateUpdateDevicesTypesTriggered()));
    connect(ui->actionCreateUpdatetables, SIGNAL(triggered()), this, SLOT(onActioncreateUpdateTablesTriggered()));
    connect(ui->actionDroptables, SIGNAL(triggered()), this, SLOT(onActiondropTablesTriggered()));
    connect(ui->actionResetdatabase, SIGNAL(triggered()), this, SLOT(onActionresetDatabaseTriggered()));
    connect(ui->actionUpload, SIGNAL(triggered()), this, SLOT(onActionuploadTriggered()));


    // Register types
    qRegisterMetaType<adminToolItem::messageType>("adminToolItem::messageType");

    m_databaseAdmin = new databaseAdmin();
    connect(m_databaseAdmin, SIGNAL(message(QString, adminToolItem::messageType)), this, SLOT(displayLogMessage(QString, adminToolItem::messageType)));
    m_databaseAdmin->connectToDB();

    m_atmelProgrammer = new atmelProgrammer(QDir("/home/benoit/projects/HomeAutomation/atmelSoftware"), this);
    connect(m_atmelProgrammer, SIGNAL(message(QString, adminToolItem::messageType)), this, SLOT(displayLogMessage(QString, adminToolItem::messageType)));
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


void adminWindow::onActioncreateHouseStructureTriggered()
{
    m_databaseAdmin->createHouseStructure();
}

void adminWindow::onActioncreateUpdateDevicesTypesTriggered()
{
    m_databaseAdmin->createDevicesTypes();
}

void adminWindow::onActioncreateUpdateTablesTriggered()
{
    m_databaseAdmin->createTables();
}

void adminWindow::onActiondropTablesTriggered()
{
    m_databaseAdmin->deleteTables();
}

void adminWindow::onActionresetDatabaseTriggered()
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

void adminWindow::onActionuploadTriggered()
{
    m_atmelProgrammer->uploadSketch(tr("dragon_isp"));
}

void adminWindow::onActioncompileSketchTriggered()
{
    m_atmelProgrammer->compileSketch();
}

void adminWindow::on_actionGenerate_new_board_triggered()
{
   QMap<QString, QString> deviceTypes = m_databaseAdmin->extractDeviceListFromFile("://sql/deviceTypes");
   moduleWizard wizard(deviceTypes, this);
   if (wizard.exec() == QDialog::Accepted) {
       qDebug() << wizard.settingsFile() << " compile? " << wizard.uploadDirectly();
   }
}
