#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H

#include <QMainWindow>
#include "adminToolItem.h"

namespace Ui {
class adminWindow;
}

class databaseAdmin;

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

    /**
     * @brief User requested updating the house structure
     * Set the house rooms and input/output devices.
     */
    void on_actionCreate_house_structure_triggered();

    void on_actionCreate_Update_devices_types_triggered();

    void on_actionCreate_Update_tables_triggered();

    void on_actionDrop_tables_triggered();

    void on_actionReset_database_triggered();

private:
    Ui::adminWindow *ui;

    // pages
    databaseAdmin * m_databaseAdmin;
};

#endif // ADMINWINDOW_H
