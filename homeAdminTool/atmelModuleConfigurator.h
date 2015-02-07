#ifndef ATMELMODULEBUILDER_H
#define ATMELMODULEBUILDER_H

#include <QWidget>

namespace Ui {
class atmelModuleConfigurator;
}

class atmelModuleConfigurator : public QWidget
{
    Q_OBJECT

public:
    explicit atmelModuleConfigurator(QStringList devicesType, QWidget *parent = 0);
    ~atmelModuleConfigurator();

private:
    Ui::atmelModuleConfigurator *ui;
};

#endif // ATMELMODULEBUILDER_H
