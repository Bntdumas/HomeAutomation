#include "databaseAdmin.h"

#include "databaseTools.h"

#include <QSqlDatabase>
#include <QStringList>
#include <QSqlQuery>
#include <QSqlError>
#include <QFile>
#include <QUuid>

databaseAdmin::databaseAdmin(QObject *parent) :
    adminToolItem(parent)
{
}

bool databaseAdmin::connectToDB()
{
    Q_EMIT message(tr("Connecting to the database..."), Info);
    QSqlDatabase db = databaseTools::getDatabase(true);
    bool dbOpened = db.open();
    if (!dbOpened) {
        Q_EMIT message(tr("Failed to connect to the databse: %1").arg(db.lastError().text()), DatabaseError);
    } else {
        Q_EMIT message(tr("Connected"), Success);
    }
    return dbOpened;
}

bool databaseAdmin::createTables()
{
    Q_EMIT message(tr("Creating tables..."), Info);
    QStringList tablesStatements;
    if (!databaseTools::extractStatementsFromFile(QStringLiteral("://sql/tables.sql"), &tablesStatements)) {
        Q_EMIT message(tablesStatements.first(), SoftwareError);
        return false;
    }
    if (executeStatements(tablesStatements)) {
        Q_EMIT message(tr("Tables created succesfully"), Success);
        return true;
    } else
        return false;
}

bool databaseAdmin::deleteTables()
{
    Q_EMIT message(tr("Deleting tables..."), Info);
    QStringList tablesStatements;
    if (!databaseTools::extractStatementsFromFile(QStringLiteral("://sql/tables.sql"), &tablesStatements)) {
        Q_EMIT message(tablesStatements.first(), SoftwareError);
        return false;
    }

    // Retrieve tables names
    QStringList tableNames;
    Q_FOREACH(const QString &tableStatement, tablesStatements) {
        QRegExp parseRegExp(QStringLiteral("CREATE TABLE (\\w+)"));
        int pos = parseRegExp.indexIn(tableStatement);
        if (pos > -1) {
            tableNames.append(parseRegExp.cap(1));
        }
    }

    if (tableNames.isEmpty()) {
        Q_EMIT message("Could not retrieve tables to delete.", SoftwareError);
        return false;
    }

    Q_FOREACH(const QString &table, tableNames) {
        executeSQL(QStringLiteral("DROP TABLE %1;").arg(table));
    }
    Q_EMIT message(tr("Tables deleted succesfully"), Success);
    return true;
}

bool databaseAdmin::createDevicesTypes()
{
    Q_EMIT message(tr("Creating devices types..."), Info);
    const QStringList list =  extractDataListFromFile("://sql/deviceTypes");
    Q_FOREACH(const QString &deviceType, list) {
        if (deviceType.isEmpty())
            continue;
        Q_EMIT message(tr("Adding device: %1").arg(deviceType), Info);
        if (!executeSQL(QStringLiteral("INSERT INTO tblType(deviceName) VALUES('%1');")
                        .arg((deviceType)))) {
            Q_EMIT message(tr("Failed to create type %1.").arg(deviceType), SoftwareError);
            return false;
        }
    }
    Q_EMIT message(tr("Devices created succesfully"), Success);
    return true;
}

bool databaseAdmin::createHouseStructure()
{
    Q_EMIT message(tr("Creating house structure..."), Info);
    QFile file("://sql/houseStructure");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        Q_EMIT message(tr("Could not open file"), SoftwareError);
        return false;
      }

    QString currentRoomName;
    while (!file.atEnd()) {
        QString line = file.readLine();
        line.remove('\n');
        if (line.startsWith("#")) { //comment
            continue;
        }

        if (line.startsWith("@ ")) { // roomm name
            currentRoomName = line.remove("@ ");
            addRoom(currentRoomName);

        } else if (line.startsWith("O ")) { // output
            line.remove("O ");
            QStringList parameters = line.split(", ");
            if (parameters.count() != 2) {
                Q_EMIT message(tr("Output device parameter mismatch (2 required).").arg(line), SoftwareError);
                return false;
            }
            addOutput(currentRoomName, parameters.at(1), parameters.at(0));

        } else if (line.startsWith("I ")) { // input
            line.remove("I ");
            QStringList parameters = line.split(", ");
            if (parameters.count() != 2) {
                Q_EMIT message(tr("Input device parameter mismatch (2 required).").arg(line), SoftwareError);
                return false;
            }
            addInput(currentRoomName, parameters.at(1), parameters.at(0));
        }

    }

    Q_EMIT message(tr("House structure created succesfully"), Success);
    return true;
}

bool databaseAdmin::addRoom(const QString &room)
{
    Q_EMIT message(tr("Adding room: %1").arg(room), Info);
    if (!executeSQL(QStringLiteral("INSERT INTO tblRoom(roomName) VALUES('%1');").arg(room))) {
        Q_EMIT message(tr("Could not create room %1.").arg(room), SoftwareError);
        return false;
    }
    Q_EMIT message(tr("Room %1 created succesfully").arg(room), Success);
    return true;
}

bool databaseAdmin::addOutput(const QString &room, const QString &outputName, const QString &deviceName)
{
    if (room.isEmpty()) {
        Q_EMIT message(tr("Could not add output device: no associated room"), SoftwareError);
        return false;
    }

    Q_EMIT message(tr("Adding output device: %1 to room %2").arg(outputName).arg(room), Info);
    const QString newOutputID = databaseTools::createUuid();
    if (!executeSQL(QStringLiteral("INSERT INTO tblOutput(outputID, outputName, fk_roomName, fk_deviceName) VALUES('%1', '%2', '%3', '%4');")
                    .arg(newOutputID)
                    .arg(outputName)
                    .arg(room)
                    .arg(deviceName))) {
        Q_EMIT message(tr("Could not create output device %1.").arg(outputName), SoftwareError);
        return false;
    }
    if (!executeSQL(QStringLiteral("INSERT INTO tblOutputState(fk_outputID, state) VALUES('%1', %2);")
                    .arg(newOutputID)
                    .arg(QStringLiteral("FALSE")))) {
        Q_EMIT message(tr("Could not create output device %1.").arg(outputName), SoftwareError);
        return false;
    }
    Q_EMIT message(tr("Output %1 created succesfully").arg(outputName), Success);
    return true;
}

bool databaseAdmin::addInput(const QString &room, const QString &inputName, const QString &deviceName)
{
    if (room.isEmpty()) {
        Q_EMIT message(tr("Could not add input device: no associated room"), SoftwareError);
        return false;
    }

    Q_EMIT message(tr("Adding input device: %1 to room %2").arg(inputName).arg(room), Info);
    const QString newInputID = databaseTools::createUuid();
    if (!executeSQL(QStringLiteral("INSERT INTO tblInput(inputID, inputName, fk_roomName, fk_deviceName) VALUES('%1', '%2', '%3', '%4');")
                    .arg(newInputID)
                    .arg(inputName)
                    .arg(room)
                    .arg(deviceName))) {
        Q_EMIT message(tr("Could not create input device %1.").arg(inputName), SoftwareError);
        return false;
    }
    if (!executeSQL(QStringLiteral("INSERT INTO tblInputState(fk_inputID, state) VALUES('%1', %2);")
                    .arg(newInputID)
                    .arg(QStringLiteral("0")))) {
        Q_EMIT message(tr("Could not create input device %1.").arg(inputName), SoftwareError);
        return false;
    }
    Q_EMIT message(tr("Input %1 created succesfully").arg(inputName), Success);
    return true;
}

bool databaseAdmin::executeStatements(const QStringList &statements)
{
    Q_FOREACH(const QString &statement, statements) {
        executeSQL(statement);
    }
    return true;
}

QStringList databaseAdmin::extractDataListFromFile(const QString &filename)
{
    QStringList list;
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        Q_EMIT message(tr("Could not open file %1").arg(filename), SoftwareError);
        return QStringList();
      }

    while (!file.atEnd()) {
        QString line = file.readLine();
        line.remove('\n');
        if (line.isEmpty() || line.startsWith("#")) { //comment
            continue;
        }

        list.append(line);
    }

      if (list.isEmpty()) {
          Q_EMIT message(tr("No items were found in the %1 file.").arg(filename), SoftwareError);
          return QStringList();
      }

      return list;
}

bool databaseAdmin::executeSQL(const QString &queryString)
{
    QSqlQuery q;
    const bool succeeded = q.exec(queryString);
    if (!succeeded) {
        Q_EMIT message(tr("Failed to execute statement:  %1. \n executed statement: %2")
                       .arg(q.lastError().text())
                       .arg(queryString), DatabaseError);
        return false;
    }
    return true;
}




