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

    /**
     * @brief User requested updating the house structure
     * Set the house rooms and input/output devices.
     */
    void onActioncreateHouseStructureTriggered();
    void onActioncreateUpdateDevicesTypesTriggered();
    void onActioncreateUpdateTablesTriggered();
    void onActiondropTablesTriggered();
    void onActionresetDatabaseTriggered();
    void onActionuploadTriggered();
    void onActioncompileSketchTriggered();
    void on_actionGenerate_new_board_triggered();
    void on_actionGenerate_source_file_triggered();

private:
    Ui::adminWindow *ui;

    // pages
    databaseAdmin * m_databaseAdmin;
};

#endif // ADMINWINDOW_H
