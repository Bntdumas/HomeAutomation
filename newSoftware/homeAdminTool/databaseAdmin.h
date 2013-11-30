#ifndef DATABASEADMIN_H
#define DATABASEADMIN_H

#include "adminToolItem.h"

/**
 * @brief Allow admin manipulation of the database
 */
class databaseAdmin : public adminToolItem
{
    Q_OBJECT
public:
    /**
     * @brief Instanciate the databaseTool object
     * @param parent: the instance parent
     */
    explicit databaseAdmin(QObject *parent = 0);

public Q_SLOTS:
    /**
     * @brief connect to the database
     * @return true if the connection succeeded.
     */
    bool connectToDB();

    /**
     * @brief Create the database tables
     * @return true if the creation succeeded.
     */
    bool createTables();

    /**
     * @brief Delete the databse structure
     * @return true if the deletion succeeded.
     */
    bool deleteTables();

    /**
     * @brief Create the database static data (devices types, etc)
     * @return true if the connection succeeded.
     */
    bool createDevicesTypes();

    /**
     * @brief Create the house structure, based on the "houseStructure" file.
     *        This method will create the house rooms and all the devices available for this house.
     * @Note to see how to populate the file, see example file's header.
     * @return true if the creation succeeded.
     */
    bool createHouseStructure();
    
    /**
     * @brief Add a room to the database
     * @param the room name
     * @return true if the room was correctly added
     */
    bool addRoom(const QString &room);
    
    /**
     * @brief Add an output to the database
     * @param room: the room name
     * @param outputName: the output name (ex "Bed lamp")
     * @param deviceName: the device name (from \b deviceType file)
     * @return true if the output was correctly added
     */
    bool addOutput(const QString &room, const QString &outputName, const QString &deviceName);
    
    /**
     * @brief Add an input to the database
     * @param inputName: the output name (ex "Room's thermometer")
     * @param deviceName: the device name (from \b deviceType file)
     * @return true if the output was correctly added
     */
    bool addInput(const QString &room, const QString &inputName, const QString &deviceName);

private:
    /**
     * @brief Execute statements included in the passed list
     * @param statements: the SQL statements list
     * @return true if succeeded
     */
    bool executeStatements(const QStringList &statements);

    /**
     * @brief Retrieves a list of coma separated values from a file
     * @param Filename: the file to open
     * @return the list of retrieved data
     */
    QStringList extractDataListFromFile(const QString &Filename);

    /**
     * @brief execute the passed SQL query.
     * @param queryString: the query to execute
     * @return true if query worked
     */
    bool executeSQL(const QString &queryString);
    
};

#endif // DATABASEADMIN_H
