#ifndef ATMELCONFIGURATOR_H
#define ATMELCONFIGURATOR_H

#include <QWidget>

namespace Ui {
class atmelConfigurator;
}

class atmelConfigurator : public QWidget
{
    Q_OBJECT

public:
    explicit atmelConfigurator(QWidget *parent = 0);
    ~atmelConfigurator();

private:
    Ui::atmelConfigurator *ui;
};

#endif // ATMELCONFIGURATOR_H
