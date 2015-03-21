#include "atmelSettingFileGenerator.h"

#include <QSettings>
#include <QTextStream>
#include <QFile>
#include <QDebug>

static const QString templateFile(":/AVR/otherFiles/moduleTemplate.pde");

// placeholders
static const QString placeholderReadWriteCycle("%READ_WRITE_CYCLE%");
static const QString placeholderSensors("%SENSORS%");
static const QString placeholderLedsEnabled("%LEDS_ENABLED%");
static const QString placeholderPinMode("%PINMODE%");
static const QString placeholderUpdateOutput("%UPDATE_OUTPUTS%");
static const QString placeholderGeneratePayload("%GENERATE_PAYLOAD%");
static const QString placeholderFunctions("%FUNCTIONS%");

atmelSettingFileGenerator::atmelSettingFileGenerator(const QString &settingsFileName, QObject *parent) :
    adminToolItem(parent)
{
    m_settingsFile = new QSettings(settingsFileName);
    
    QFile sourceFileTemplate(templateFile);
    
    if (!sourceFileTemplate.open(QFile::ReadOnly)) {
        Q_EMIT message(QString("Could not open source code template file %1").arg(templateFile), SoftwareError);
        return;        
    }
    m_sourceCode = sourceFileTemplate.readAll();
}

const QString atmelSettingFileGenerator::generateSource()
{
    QString sourceFilePath;
    // Check that all necessary placeholders are in the template.
    Q_EMIT message(QString("Checking placeholders in template file..."), Info);

    QStringList missingPlaceHolders;
    if (!m_sourceCode.contains(placeholderReadWriteCycle))
        missingPlaceHolders << placeholderReadWriteCycle;
    if (!m_sourceCode.contains(placeholderSensors))
        missingPlaceHolders << placeholderSensors;
    if (!m_sourceCode.contains(placeholderLedsEnabled))
        missingPlaceHolders << placeholderLedsEnabled;
    if (!m_sourceCode.contains(placeholderPinMode))
        missingPlaceHolders << placeholderPinMode;
    if (!m_sourceCode.contains(placeholderUpdateOutput))
        missingPlaceHolders << placeholderUpdateOutput;
    if (!m_sourceCode.contains(placeholderGeneratePayload))
        missingPlaceHolders << placeholderGeneratePayload;
    if (!m_sourceCode.contains(placeholderFunctions))
        missingPlaceHolders << placeholderFunctions;

    if (!missingPlaceHolders.isEmpty()) {
        Q_EMIT message(QString("the following placeholders are not in the template file: %1").arg(missingPlaceHolders.join(", ")), SoftwareError);
        return QString();
    } else {
        Q_EMIT message(QString("... OK"), Success);
    }

    // check settings file integrity

           /* QSettings settings(field("moduleSourcePath").toString(),  QSettings::IniFormat);
            settings.setValue("board/name", field("moduleName").toString());

            QMapIterator<int, QPair<QString, QString> > mapIterator(field("allGPIO").value< QMap<int, QPair<QString, QString> > >());
            while (mapIterator.hasNext()) {
                mapIterator.next();
                QPair<QString, QString> currentGPIOState = mapIterator.value();
                settings.setValue(QString("board/GPIO%1/device").arg(mapIterator.key()), currentGPIOState.first);
                settings.setValue(QString("board/GPIO%1/deviceType").arg(mapIterator.key()), currentGPIOState.second );
            }
            settings.setValue("board/LEDS", field("LEDsEnabled").toBool());
            settings.setValue("board/temperatureSensor", field("tempSensorEnabled").toBool());
            settings.setValue("board/lightSensor", field("lightSensorEnabled").toBool());

            settings.setValue("board/readWriteCycle", field("readWriteCycle").toInt());
            settings.setValue("board/programmer", field("programmer").toString());

            settings.sync();

            return (settings.status() == QSettings::NoError );*/

    // replace constants
    Q_EMIT message(QString("Replacing constants parameters"), Info);
    qDebug() << m_settingsFile->value("board/readWriteCycle");
    const QString readWriteCycle = m_settingsFile->value("board/readWriteCycle").toString();
    const QString ledEnabled = m_settingsFile->value("board/LEDS").toString();

    if (readWriteCycle.isEmpty()) {
        Q_EMIT message(QString("Couldn't read the read write cycle."), SoftwareError);
        return QString();
    }

    if (ledEnabled.isEmpty()) {
        Q_EMIT message(QString("Couldn't read the led status."), SoftwareError);
        return QString();
    }

    m_sourceCode.replace(placeholderReadWriteCycle, readWriteCycle);
    m_sourceCode.replace(placeholderLedsEnabled, ledEnabled);
    Q_EMIT message(QString("... OK"), Success);


    Q_EMIT message(m_sourceCode, Info);
    return QString();
}


QString atmelSettingFileGenerator::lightSensorFunction(const QString &sensorPinName) const
{
    QString funcStr;
    QTextStream stream(&funcStr);
    stream <<  "float temperature() "<< endl
    <<"{" << endl
    << "return (5.0 * analogRead(" << sensorPinName <<  ") * 100.0) / 1024;" << endl
    <<"}" << endl;
    return funcStr;
}
QString atmelSettingFileGenerator::thermometerFunction(const QString &sensorPinName) const
{
    QString funcStr;
    QTextStream stream(&funcStr);
    stream <<  "float light() "<< endl
    <<"{" << endl
    << "return analogRead(" << sensorPinName <<  ") * (5.0 / 1023.0);" << endl
    <<"}" << endl;
    return funcStr;
}
