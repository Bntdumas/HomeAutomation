#ifndef DATABASETOOLS_H
#define DATABASETOOLS_H

#include <QObject>
#include <QSqlDatabase>
#include <QStringList>

/**
 * @brief The databaseTools class offers methods to access the home automation database.
 */

class databaseTools : public QObject
{
    Q_OBJECT

public:
    explicit databaseTools(QObject *parent = 0);


    /**
     * @brief Creates a database object with the correct credential, IP, etc.
     * You only need to call QSqlDatabase::open() to get a valid database connection.
     * @param adminMode: wether the database should be opened with admin privileges.
     * @return a prepared QSqlDatabase
     */
    static QSqlDatabase getDatabase(bool adminMode = false);

    /**
     * @brief Opens the file passed as parameter and retrieve SQL statements separated by "STATEMENT_END"
     * @param fileName: The name of the file to parse
     * @param listToPopulate: the list to fill
     * @return True if parsing succeeded.
     * @note if parsing failed, the list is populated with only one element: a human readable error string.
     */
    static bool extractStatementsFromFile(const QString &fileName, QStringList *listToPopulate);

    /**
     * @brief Create a text-based uuid for the database
     * @return a 8 characters uuid
     */
    static QString createUuid();

};

#endif // DATABASETOOLS_H
