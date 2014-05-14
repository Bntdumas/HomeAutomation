#include "atmelConfigurator.h"
#include "ui_atmelConfigurator.h"

atmelConfigurator::atmelConfigurator(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::atmelConfigurator)
{
    ui->setupUi(this);
}

atmelConfigurator::~atmelConfigurator()
{
    delete ui;
}
