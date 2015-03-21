#ifndef ATMELSETTINGFILEGENERATOR_H
#define ATMELSETTINGFILEGENERATOR_H

#include "adminToolItem.h"

#include <QFile>

/**
 * Creates a pde file from the template in otherFiles/moduleTemplate.pde
 * using predefiend settings found in the settings file passed as ctor parameter
 */
class QSettings;

class atmelSettingFileGenerator : public adminToolItem
{
    Q_OBJECT
public:   
    explicit atmelSettingFileGenerator(const QString &settingsFileName, QObject *parent = 0);

public Q_SLOTS:
    const QString generateSource();

protected :
    QString thermometerFunction(const QString &sensorName) const;
    QString lightSensorFunction(const QString &sensorName) const;

private:
    QSettings *m_settingsFile;
    QString m_sourceCode;

};

#endif // ATMELSETTINGFILEGENERATOR_H
