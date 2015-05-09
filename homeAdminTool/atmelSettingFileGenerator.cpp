#include "atmelSettingFileGenerator.h"

#include <QSettings>
#include <QTextStream>
#include <QFile>
#include <QDebug>

static const QString templateFile(":/AVR/otherFiles/moduleTemplate.pde");

// placeholders
static const QString placeholderReadWriteCycle("%READ_WRITE_CYCLE%");
static const QString placeholderPayloadStruct("%PAYLOAD_STRUCT%");
static const QString placeholderLedsEnabled("%LEDS_ENABLED%");
static const QString placeholderPinMode("%PINMODE%");
static const QString placeholderUpdateOutput("%UPDATE_OUTPUTS%");
static const QString placeholderGeneratePayload("%GENERATE_PAYLOAD%");
static const QString placeholderFunctions("%FUNCTIONS%");

atmelSettingFileGenerator::atmelSettingFileGenerator(const QString &settingsFileName, QObject *parent) :
    adminToolItem(parent)
{
    m_settingsFile = new QSettings(settingsFileName, QSettings::IniFormat);
    
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
    Q_EMIT message(QString("=== Creation of a source code for a module board ==="), Info);
    Q_EMIT message(QString("Settings file is %1").arg(m_settingsFile->fileName()), Info);

    // check that the placeholders are present in the given template
    if (!testPlaceholdersPresenceInTemplateFile()) {
        return QString();
    }

    //retrieve GPIO list
    m_gpioList = getGPIOFromSettings();
    if (gpioList.count() != 8) {
        return QString();
    }

    // replace constants
    if (!replaceConstants()) {
        return QString();
    }

    // set pin modes
    if (!setPinModes()) {
        return QString();
    }

    // payload generation



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

bool atmelSettingFileGenerator::testPlaceholdersPresenceInTemplateFile()
{
    Q_EMIT message(QString("Checking placeholders in template file..."), Info);

    QStringList missingPlaceHolders;
    if (!m_sourceCode.contains(placeholderReadWriteCycle))
        missingPlaceHolders << placeholderReadWriteCycle;
    if (!m_sourceCode.contains(placeholderPayloadStruct))
        missingPlaceHolders << placeholderPayloadStruct;
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
        return false;
    }
    Q_EMIT message(QString("... OK"), Success);
    return true;
}

const QList<atmelModuleConfigurator::GPIOPin> atmelSettingFileGenerator::getGPIOFromSettings()
{
    QList<atmelModuleConfigurator::GPIOPin> pins;
    for (int i = 1; i <= 8; i++) {
        atmelModuleConfigurator::GPIOPin currentPin;
        const QString gpioID = QString("GPIO" + QString::number(i));
        currentPin.index = i;
        currentPin.pinDirection = m_settingsFile->value("board/" + gpioID + "/Direction").toString();
        currentPin.name = m_settingsFile->value("board/" + gpioID + "/Name").toString();
        currentPin.deviceType = m_settingsFile->value("board/" + gpioID + "/Type").toString();

        if (!currentPin.isValid()) {
            Q_EMIT message(QString("Couldn't read the GPIO information for port number %1.").arg(i), SoftwareError);
            return QList<atmelModuleConfigurator::GPIOPin>();
        }
        pins.append(currentPin);
    }
    return pins;
}

bool atmelSettingFileGenerator::replaceConstants()
{
    Q_EMIT message(QString("Replacing constants parameters"), Info);
    const QString readWriteCycle = m_settingsFile->value("board/readWriteCycle").toString();
    const QString ledEnabled = m_settingsFile->value("board/LEDS").toBool() ? "1":"0";

    if (readWriteCycle.isEmpty()) {
        Q_EMIT message(QString("Couldn't read the read write cycle."), SoftwareError);
        return false;
    }

    if (ledEnabled.isEmpty()) {
        Q_EMIT message(QString("Couldn't read the led status."), SoftwareError);
        return false;
    }

    m_sourceCode.replace(placeholderReadWriteCycle, readWriteCycle);
    m_sourceCode.replace(placeholderLedsEnabled, ledEnabled);
    Q_EMIT message(QString("... OK"), Success);
    return true;
}

bool atmelSettingFileGenerator::setPinModes()
{
    // pinMode(redLed, OUTPUT);
    QStringList pinModes;
    Q_FOREACH( atmelModuleConfigurator::GPIOPin pin, m_gpioList) {
        const QString input("INPUT");
        const QString output("OUTPUT");
        const QString instruction("    pinMode(GPIO%1, %2);");
        bool isOutput = pin.pinDirection == "output" || pin.pinDirection == "other";
        pinModes << instruction.arg(pin.index).arg(isOutput?output:input);
    }

    if (pinModes.isEmpty()) {
        return false;
    }

    const QString instructionsBlock(pinModes.join('\n'));
    m_sourceCode.replace(placeholderPinMode, instructionsBlock);
    return true;
}

bool atmelSettingFileGenerator::generatePayload()
{
    Q_EMIT message(QString("Creating payload structure"), Info);

    /*
     *struct payloadSensors
{
    float temperature;
    float light;
};
 */

    // First, the struct
    QString payloadStruct;
    QTextStream stream(&payloadStruct);
    stream << "struct payloadStruct" << endl << '{';

    if ( m_settingsFile->value("board/LEDS").toBool() ?)
    stream << "float embedded_light;" << endl;
    stream << "float embedded_temperature;" << endl;

    Q_FOREACH( atmelModuleConfigurator::GPIOPin pin, m_gpioList) {
        const QString dataType = dataTypeForDevice(pin.deviceType);
        if (dataType.isEmpty()) {
            Q_EMIT message(QString("Couldn't parse the data type for device \"%1\".").arg(pin.name), SoftwareError);
            return false;
        }
        stream << dataTypeForDevice << ' ' << pin.name << ';' << endl;
    }
    stream <<  endl << "};";
}

QString atmelSettingFileGenerator::dataTypeForDevice(const QString &device)
{
    // take the word after last char '_'
    const QString dataTypeStr =  device.split('_').last();

    //check that the type is syntaxically correct
    if (dataTypeStr == "bool"||
        dataTypeStr == "int" ||
        dataTypeStr == "float")
        return dataTypeStr;
    else return QString();
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
