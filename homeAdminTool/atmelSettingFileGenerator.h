#ifndef ATMELSETTINGFILEGENERATOR_H
#define ATMELSETTINGFILEGENERATOR_H

#include "adminToolItem.h"
#include "atmelModuleConfigurator.h"

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

    /// test if all the placeholders are in the settings file
    bool testPlaceholdersPresenceInTemplateFile();

    /// Retrieve GPIO information (index, name, type, direction) from the settings file
    const QList<atmelModuleConfigurator::GPIOPin> getGPIOFromSettings();

    /// Replace the constants in the source code, using data form the settings file
    bool replaceConstants();

    /// set the different pin modes for the GPIOS
    bool setPinModes();

    /// creates a struct for the payload, and add the function to generate it.
    bool generatePayload();

    /// returns a C++ data type for a given device (bool, int, etc)
    QString dataTypeForDevice(const QString &device);


private:
    QSettings *m_settingsFile;
    QString m_sourceCode;
    QList< atmelModuleConfigurator::GPIOPin > m_gpioList;
};

#endif // ATMELSETTINGFILEGENERATOR_H
