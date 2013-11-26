#include "adminWindow.h"
#include "ui_adminWindow.h"

adminWindow::adminWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::adminWindow)
{
    ui->setupUi(this);
}

adminWindow::~adminWindow()
{
    delete ui;
}

void adminWindow::on_actionDeleteTables_triggered()
{
    
}

void adminWindow::on_actionCreateTables_triggered()
{
    
}

void adminWindow::on_actionCreateStatic_triggered()
{
    
}
