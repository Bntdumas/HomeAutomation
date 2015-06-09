#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H

#include <QMainWindow>
#include "adminToolItem.h"

namespace Ui {
class adminWindow;
}

class databaseAdmin;
class atmelProgrammer;

/**
 * @brief The home automation admin app.
 */
class adminWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit adminWindow(QWidget *parent = 0);
    ~adminWindow();
    
private Q_SLOTS:

    /**
     * @brief Displays a message with color code and timestamp.
     * @param The message text
     * @param The message type
     */
    void displayLogMessage(const QString &text, const adminToolItem::messageType type);

private:
    Ui::adminWindow *ui;
};

#endif // ADMINWINDOW_H
