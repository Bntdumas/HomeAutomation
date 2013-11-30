#include "databaseAdmin.h"

#include "databaseTools.h"

#include <QSqlDatabase>
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

}

bool databaseAdmin::createDevicesTypes()
{
    const QStringList list =  extractDataListFromFile("://sql/deviceTypes");
    Q_FOREACH(const QString &deviceType, list) {
        if (deviceType.isEmpty())
            continue;
        if (!executeSQL(QStringLiteral("INSERT INTO tblType(typeID, name) VALUES(%1, %2)").arg(QUuid::createUuid()).arg((deviceType)))) {
            Q_EMIT message(tr("Failed to create type %1.").arg(deviceType), SoftwareError);
            return false;
        }
    }
    return true;
}

bool databaseAdmin::createHouseStructure()
{
    QStringList list;
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        Q_EMIT message(tr("Could not open file %1").arg(fileName), SoftwareError);
        return false;
      }

    while (!file.atEnd()) {
        QString line = file.readLine();
        QString currentRoomName;
        if (line.startsWith("#")) { //comment
            continue;
        }

        if (line.startsWith("@ ")) { // roomm name
            currentRoomName = line.remove("@ ");
            if (!executeSQL(QStringLiteral("INSERT INTO tblRoom(roomName) VALUES(%1)").arg(line))) {
                Q_EMIT message(tr("Could not create room %1.").arg(line), SoftwareError);
                return false;
            }

        } else if (line.startsWith("O ")) { // output
            if (currentRoomName.isEmpty()) {
                Q_EMIT message(tr("Could not add output device: no associated room"), SoftwareError);
                return false;
            }

            currentDevice = line.remove("O ");
            QStringList parameters = line.split(", ");
            if (parameters.count() != 2) {
                Q_EMIT message(tr("Output device parameter mismatch (2 required).").arg(line), SoftwareError);
                return false;
            }

            if (!executeSQL(QStringLiteral("INSERT INTO tblOutput(outputID, outputName, fk_roomName, fk_deviceName) VALUES(%1, %2, %3, %4)")
                            .arg(QUuid::createUuid().toString())
                            .arg(parameters.at(1))
                            .arg(currentRoomName)
                            .arg(parameters.at(0)))) {
                Q_EMIT message(tr("Could not create output device %1.").arg(parameter.at(1)), SoftwareError);
                return false;
            }
        } else if (line.startsWith("I ")) { // input
            if (currentRoomName.isEmpty()) {
                Q_EMIT message(tr("Could not add input device: no associated room"), SoftwareError);
                return false;
            }

            currentDevice = line.remove("O ");
            QStringList parameters = line.split(", ");
            if (parameters.count() != 2) {
                Q_EMIT message(tr("Input device parameter mismatch (2 required).").arg(line), SoftwareError);
                return false;
            }

            if (!executeSQL(QStringLiteral("INSERT INTO tblInput(inputID, inputName, fk_roomName, fk_deviceName) VALUES(%1, %2, %3, %4)")
                            .arg(QUuid::createUuid().toString())
                            .arg(parameters.at(1))
                            .arg(currentRoomName)
                            .arg(parameters.at(0)))) {
                Q_EMIT message(tr("Could not create input device %1.").arg(parameter.at(1)), SoftwareError);
                return false;
            }
        }
    }

    if (list.isEmpty()) {
        Q_EMIT message(tr("No items were found in the %1 file.").arg(Filename), SoftwareError);
        return QStringList();
    }

      return list;
}

bool databaseAdmin::addRoom(const QString &room)
{

}

bool databaseAdmin::addOutput(const QString &room, const QString &type)
{

}

bool databaseAdmin::addInput(const QString &room, const QString &type)
{

}

bool databaseAdmin::executeStatements(const QStringList &statements)
{
    Q_FOREACH(const QString &statement, tablesStatements) {
        executeSQL(statement);
    }
    return true;
}

QStringList databaseAdmin::extractDataListFromFile(const QString &Filename)
{
    QStringList list;
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        Q_EMIT message(tr("Could not open file %1").arg(fileName), SoftwareError);
        return QStringList();
      }

    while (!file.atEnd()) {
        const QString line = file.readLine();
        if (line.startsWith("#")) { //comment
            continue;
        }

        list.append(line);
    }

      if (list.isEmpty()) {
          Q_EMIT message(tr("No items were found in the %1 file.").arg(Filename), SoftwareError);
          return QStringList();
      }

      return list;
}

bool databaseAdmin::executeSQL(const QString &query)
{
    QSqlQuery query(query);
    const bool succeeded = query.exec();
    if (!succeeded) {
        Q_EMIT message(tr("Failed to execute statement:  %1. \n executed statement: %2").arg(db.lastError().text()).arg(statement), DatabaseError);
        return false;
    }
    return true;
}




