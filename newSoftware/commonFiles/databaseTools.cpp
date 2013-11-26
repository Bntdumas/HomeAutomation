#include "databaseTools.h"

#include <QFile>

databaseTools::databaseTools(QObject *parent) :
    QObject(parent)
{

}


QSqlDatabase databaseTools::getDatabase(bool adminMode)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("acidalia");
    db.setDatabaseName("customdb");
    if (adminMode) {
        db.setUserName("mojito");
        db.setPassword("J0a1m8");
    } else {
        db.setUserName("mojito");
        db.setPassword("J0a1m8");
    }
    return db;
}


bool databaseTools::extractStatementsFromFile(const QString &fileName, QStringList *listToPopulate)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        listToPopulate->clear();
        listToPopulate->append(tr("Could not open file %1").arg(fileName));
        return false;
      }

      QString fileContents = file.readAll();
      *listToPopulate = fileContents.split("STATEMENT_END");

      if (listToPopulate->isEmpty()) {
          listToPopulate->append(tr("No SQL statements were found in the %1 file."));
          return false;
      }

      return true;
}
