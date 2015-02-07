#ifndef ATMELSETTINGFILEGENERATOR_H
#define ATMELSETTINGFILEGENERATOR_H
#include "adminToolItem.h"

class atmelSettingFileGenerator : public adminToolItem
{
    Q_OBJECT
public:
    explicit atmelSettingFileGenerator(QObject *parent = 0);QString constants(const int thisNode, const int masterNode, const bool ledEnabled) const;


public Q_SLOTS:    
    //void createConstants(const int thisNode, const int masterNode, const bool ledEnabled) const;
   /* void createGPIOPinModeFunction;
    void createUpdateOutputsFunction;
    void createGeneratePayloadFunction;
    void createPayloadInput;
    void createPayloadOutput;
    void createTemperatureFunction;
    void createlLghtFunction;*/


private :
    struct settingFile {
        QString constants;
        QString GPIOPinModeFunction;
        QString updateOutputsFunction;
        QString generatePayloadFunction;
        QString payloadInput;
        QString payloadOutput;
        QString temperatureFunction;
        QString lightFunction;
    };
    settingFile m_settingFile;
};

#endif // ATMELSETTINGFILEGENERATOR_H
