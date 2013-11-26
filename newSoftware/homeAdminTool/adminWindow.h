#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H

#include <QMainWindow>

namespace Ui {
class adminWindow;
}

class adminWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit adminWindow(QWidget *parent = 0);
    ~adminWindow();
    
private slots:
    void on_actionDeleteTables_triggered();
    
    void on_actionCreateTables_triggered();
    
    void on_actionCreateStatic_triggered();
    
private:
    Ui::adminWindow *ui;
};

#endif // ADMINWINDOW_H
