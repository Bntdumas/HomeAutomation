#include "atmelModuleConfigurator.h"
#include "ui_atmelModuleConfigurator.h"

#include <QStringListModel>
#include <QStringList>

atmelModuleConfigurator::atmelModuleConfigurator(QStringList devicesType, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::atmelModuleConfigurator)
{
    ui->setupUi(this);
    QStringListModel *mdl = new QStringListModel(this);
    mdl->setStringList(devicesType);

    ui->cbPort1->setModel(mdl);
    ui->cbPort2->setModel(mdl);
    ui->cbPort3->setModel(mdl);
    ui->cbPort4->setModel(mdl);
    ui->cbPort5->setModel(mdl);
    ui->cbPort6->setModel(mdl);
    ui->cbPort7->setModel(mdl);
    ui->cbPort8->setModel(mdl);
}

atmelModuleConfigurator::~atmelModuleConfigurator()
{
    delete ui;
}
