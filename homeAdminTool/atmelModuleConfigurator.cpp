#include "atmelModuleConfigurator.h"
#include "ui_atmelModuleConfigurator.h"

#include <QStringListModel>
#include <QStringList>
#include <QComboBox>
#include <QPair>


#include <QDebug>

atmelModuleConfigurator::atmelModuleConfigurator(QMap<QString, QString> devicesType, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::atmelModuleConfigurator),
    m_deviceTypes(devicesType)
{
    ui->setupUi(this);


    QStringListModel *mdl = new QStringListModel(this);
    mdl->setStringList(m_deviceTypes.keys());

    const QString defaultValue = devicesType.key("other");
    QList< GPIOWidgetPair > gpioWidgets = GPIOSelectionWidgets();
    GPIOWidgetPair curentPair;
    Q_FOREACH(curentPair, gpioWidgets) {
        QComboBox * comboBox = curentPair.first;
        comboBox->setModel(mdl);
        comboBox->setCurrentText(defaultValue);
    }

    connect(ui->leBoardName, SIGNAL(textChanged(QString)), this, SIGNAL(moduleNameChanged(QString)));

    connect(ui->cbPort1, SIGNAL(currentTextChanged(QString)), this, SIGNAL(GPIO1Changed(QString)));
    connect(ui->cbPort2, SIGNAL(currentTextChanged(QString)), this, SIGNAL(GPIO2Changed(QString)));
    connect(ui->cbPort3, SIGNAL(currentTextChanged(QString)), this, SIGNAL(GPIO3Changed(QString)));
    connect(ui->cbPort4, SIGNAL(currentTextChanged(QString)), this, SIGNAL(GPIO4Changed(QString)));

    connect(ui->cbPort5, SIGNAL(currentTextChanged(QString)), this, SIGNAL(GPIO5Changed(QString)));
    connect(ui->cbPort6, SIGNAL(currentTextChanged(QString)), this, SIGNAL(GPIO6Changed(QString)));
    connect(ui->cbPort7, SIGNAL(currentTextChanged(QString)), this, SIGNAL(GPIO7Changed(QString)));
    connect(ui->cbPort8, SIGNAL(currentTextChanged(QString)), this, SIGNAL(GPIO8Changed(QString)));

    connect(ui->enableLED, SIGNAL(clicked(bool)), this, SIGNAL(LEDsEnabledChanged(bool)));
    connect(ui->enableThermometer, SIGNAL(clicked(bool)), this, SIGNAL(tempSensorEnabledChanged(bool)));
    connect(ui->enableLightSensor, SIGNAL(clicked(bool)), this, SIGNAL(lightSensorEnabledChanged(bool)));
    connect(ui->readWriteCycle, SIGNAL(valueChanged(int)), this, SIGNAL(readWriteCycleChanged(int)));
}

atmelModuleConfigurator::~atmelModuleConfigurator()
{
    delete ui;
}

QVariant atmelModuleConfigurator::allGPIO()
{
    QList< GPIOPin >  gpioStatus;
    QList< GPIOWidgetPair > gpioWidgets = GPIOSelectionWidgets();
    int GPIOIndex = 0;
    GPIOWidgetPair curentPair;
    Q_FOREACH (curentPair, gpioWidgets) {
        GPIOPin  currentGpioPin;
        QComboBox * comboBox = curentPair.first;
        QLineEdit *lineEdit = curentPair.second;
        currentGpioPin.name = lineEdit->text();
        currentGpioPin.index = ++GPIOIndex;
        currentGpioPin.deviceType = comboBox->currentText();
        currentGpioPin.pinDirection = m_deviceTypes.value(currentGpioPin.deviceType);
        gpioStatus.append(currentGpioPin);
    }
    return qVariantFromValue(gpioStatus);
}

bool atmelModuleConfigurator::validate()
{
    fillUpEmptyFields();
    QStringList namesList = getDevicesNames();
    handleDuplicateNames(namesList);
    int index = 0 ;
    GPIOWidgetPair curentPair;
    QList< GPIOWidgetPair > gpioWidgets = GPIOSelectionWidgets();
    Q_FOREACH (curentPair, gpioWidgets) {
        QLineEdit *lineEdit = curentPair.second;
        lineEdit->setText(namesList.at(index++));
    }
    return true;
}

void atmelModuleConfigurator::fillUpEmptyFields()
{
    QStringList nameList;
    QList< GPIOWidgetPair > gpioWidgets = GPIOSelectionWidgets();
    GPIOWidgetPair curentPair;
    Q_FOREACH (curentPair, gpioWidgets) {
        QComboBox *comboBox = curentPair.first;
        QLineEdit *lineEdit = curentPair.second;
        if (lineEdit->text().isEmpty() && (comboBox->currentText() != "Disabled")) {
            nameList << comboBox->currentText();
        } else
            nameList << lineEdit->text();
    }

    if (ui->leBoardName->text().isEmpty())
        ui->leBoardName->setText("MyModule");
}

QStringList atmelModuleConfigurator::getDevicesNames() const
{
    QStringList nameList;
    QList< GPIOWidgetPair > gpioWidgets = GPIOSelectionWidgets();
    GPIOWidgetPair curentPair;
    Q_FOREACH (curentPair, gpioWidgets) {
        QComboBox *comboBox = curentPair.first;
        QLineEdit *lineEdit = curentPair.second;
        if (comboBox->currentText() == "Disabled") {
            nameList << "Disabled";
        } else if (lineEdit->text().isEmpty()) {
            nameList << comboBox->currentText();
        } else {
            nameList << lineEdit->text();
        }
    }
    return nameList;
}

void atmelModuleConfigurator::handleDuplicateNames(QStringList &list)
{
    Q_FOREACH(const QString &name, list) {
        int count = list.count(name);
        if (count == 1)
            continue;

        int index = list.lastIndexOf(name);
        do {
            const QString newName = QString("%1_%2").arg(list.at(index)).arg(count);
            list.replace(index, newName);
            count = list.count(name);
            index = list.lastIndexOf(name);
        } while (index != -1 && count >= 2);
    }
}

QString atmelModuleConfigurator::moduleName()
{
    return ui->leBoardName->text().isEmpty() ? "MyModule":ui->leBoardName->text() ;
}

void atmelModuleConfigurator::setModuleName(const QString &text)
{
    ui->leBoardName->setText(text);
}

QString atmelModuleConfigurator::GPIO1()
{
    return QString(ui->cbPort1->currentText());
}

void atmelModuleConfigurator::setGPIO1(const QString &text)
{
    ui->cbPort1->setCurrentText(text);
}

QString atmelModuleConfigurator::GPIO2()
{
    return QString(ui->cbPort2->currentText());
}

void atmelModuleConfigurator::setGPIO2(const QString &text)
{
    ui->cbPort2->setCurrentText(text);
}

QString atmelModuleConfigurator::GPIO3()
{
    return QString(ui->cbPort3->currentText());
}

void atmelModuleConfigurator::setGPIO3(const QString &text)
{
    ui->cbPort3->setCurrentText(text);
}

QString atmelModuleConfigurator::GPIO4()
{
    return QString(ui->cbPort4->currentText());
}

void atmelModuleConfigurator::setGPIO4(const QString &text)
{
    ui->cbPort4->setCurrentText(text); }

QString atmelModuleConfigurator::GPIO5()
{
    return QString(ui->cbPort5->currentText());
}

void atmelModuleConfigurator::setGPIO5(const QString &text)
{
    ui->cbPort5->setCurrentText(text);
}

QString atmelModuleConfigurator::GPIO6()
{
    return QString(ui->cbPort6->currentText());
}

void atmelModuleConfigurator::setGPIO6(const QString &text)
{
    ui->cbPort6->setCurrentText(text);
}

QString atmelModuleConfigurator::GPIO7()
{
    return QString(ui->cbPort7->currentText());
}

void atmelModuleConfigurator::setGPIO7(const QString &text)
{
    ui->cbPort7->setCurrentText(text);
}

QString atmelModuleConfigurator::GPIO8()
{
    return QString(ui->cbPort8->currentText());
}

void atmelModuleConfigurator::setGPIO8(const QString &text)
{
    ui->cbPort8->setCurrentText(text);
}

bool atmelModuleConfigurator::LEDsEnabled()
{
   return  ui->enableLED->isChecked();
}

void atmelModuleConfigurator::setLEDsEnabled(bool flag)
{
    ui->enableLED->setChecked(flag);
}

bool atmelModuleConfigurator::tempSensorEnabled()
{
     return  ui->enableThermometer->isChecked();
}

void atmelModuleConfigurator::setTempSensorEnabled(bool flag)
{
    ui->enableThermometer->setChecked(flag);
}

bool atmelModuleConfigurator::lightSensorEnabled()
{
     return  ui->enableLightSensor->isChecked();
}

void atmelModuleConfigurator::setLightSensorEnabled(bool flag)
{
    ui->enableLightSensor->setChecked(flag);
}

int atmelModuleConfigurator::readWriteCycle()
{
     return  ui->readWriteCycle->value();
}

void atmelModuleConfigurator::setReadWriteCycle(int value)
{
    ui->readWriteCycle->setValue(value);
}

QStringList atmelModuleConfigurator::currentlySelectedGPIO()
{
    QStringList list;
    QList< GPIOWidgetPair > gpioWidgets = GPIOSelectionWidgets();
    GPIOWidgetPair curentPair;
    Q_FOREACH (curentPair, gpioWidgets) {
        QComboBox *comboBox = curentPair.first;
        list.append(comboBox->currentText());
    }
    return list;
}

QList< GPIOWidgetPair > atmelModuleConfigurator::GPIOSelectionWidgets() const
{
    QList< GPIOWidgetPair > list;

    list << GPIOWidgetPair(ui->cbPort1, ui->portName1)
            <<  GPIOWidgetPair(ui->cbPort2, ui->portName2)
            <<  GPIOWidgetPair(ui->cbPort3, ui->portName3)
            <<  GPIOWidgetPair(ui->cbPort4, ui->portName4)
            <<  GPIOWidgetPair(ui->cbPort5, ui->portName5)
            <<  GPIOWidgetPair(ui->cbPort6, ui->portName6)
            <<  GPIOWidgetPair(ui->cbPort7, ui->portName7)
            << GPIOWidgetPair(ui->cbPort8, ui->portName8);
    return list;
}


