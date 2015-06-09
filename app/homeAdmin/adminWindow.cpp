#include "adminWindow.h"
#include "ui_adminWindow.h"

#include <QDateTime>

adminWindow::adminWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::adminWindow)
{
    ui->setupUi(this);    // Register types
    qRegisterMetaType<adminToolItem::messageType>("adminToolItem::messageType");
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

    case adminToolItem::SoftwareError:
        ui->txtLog->setTextColor(Qt::darkRed);
        break;
    }
    ui->txtLog->append(QStringLiteral("%1: %2").arg(QDateTime::currentDateTime().toString(QStringLiteral("hh:mm:ss"))).arg(text));
}
