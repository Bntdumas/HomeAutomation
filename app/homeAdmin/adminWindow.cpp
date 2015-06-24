#include "adminWindow.h"
#include "ui_adminWindow.h"
#include "moduleServer.h"

#include <QDateTime>

adminWindow::adminWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::adminWindow)
    ,m_server(Q_NULLPTR)
{
    ui->setupUi(this);    // Register types
    qRegisterMetaType<utils::messageType>("utils::messageType");
}

adminWindow::~adminWindow()
{
    delete ui;
}

void adminWindow::displayLogMessage(const QString &text, const utils::messageType type)
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

void adminWindow::on_action_re_start_triggered()
{
    if (m_server) {
        delete m_server;
    }

    m_server = new moduleServer(this);
    connect(m_server, &moduleServer::message, this, &adminWindow::displayLogMessage);
    m_server->createTCPServer();
}
