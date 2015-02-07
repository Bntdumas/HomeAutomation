#include "atmelSettingFileGenerator.h"

#include <QStringList>

atmelSettingFileGenerator::atmelSettingFileGenerator(QObject *parent) :
    adminToolItem(parent)
{
}


/*QString atmelSettingFileGenerator::createConstants(const int thisNode, const int masterNode, const bool ledEnabled) const
{
    QStringList lines;
    lines << QString("// constants");
             << QString("const uint16_t thisNode = %1;").arg(thisNode);
             << QString("const uint16_t masterNode = %1;").arg(masterNode);
             << QString("#define LED_ENABLE %1").arg(ledEnabled);

    m_settingFile.constants =  lines.join('\n');
}





void setGPIOPinMode()
{
    pinMode(GPIO1, <GPIO1_mode>);
    pinMode(GPIO2, <GPIO2_mode>);
    pinMode(GPIO3, <GPIO3_mode>);
    pinMode(GPIO4, <GPIO4_mode>);
    pinMode(GPIO5, <GPIO5_mode>);
    pinMode(GPIO6, <GPIO6_mode>);
    pinMode(GPIO7, <GPIO7_mode>);
    pinMode(GPIO8, <GPIO8_mode>);
}

void updateOutputs(const payloadInput &input)
{

}

payloadOutput generatePayload() const
{

}

struct payloadInput
{

};

struct payloadOutput
{

};

float temperature()
{
    return (5.0 * analogRead(tempSensor) * 100.0) / 1024;
}

float light()
{
    return analogRead(lightSensor) * (5.0 / 1023.0);
}


 * */
