#ifndef ATMELSIMULATOR_H
#define ATMELSIMULATOR_H

#include <QWidget>

namespace Ui {
class atmelSimulator;
}

class atmelSimulator : public QWidget
{
    Q_OBJECT
    
public:
    explicit atmelSimulator(QWidget *parent = 0);
    ~atmelSimulator();
    
private:
    Ui::atmelSimulator *ui;
};

#endif // ATMELSIMULATOR_H_
