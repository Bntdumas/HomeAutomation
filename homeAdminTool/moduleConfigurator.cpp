#include "moduleConfigurator.h"
#include "ui_moduleConfigurator.h"

#include <QStringListModel>
#include <QStringList>
#include <QComboBox>
#include <QPair>


#include <QDebug>

moduleConfigurator::moduleConfigurator(QMap<QString, QString> devicesType, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::moduleConfigurator),
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

moduleConfigurator::~moduleConfigurator()
{
    delete ui;
}

QVariant moduleConfigurator::allGPIO()
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

bool moduleConfigurator::validate()
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

void moduleConfigurator::fillUpEmptyFields()
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

QStringList moduleConfigurator::getDevicesNames() const
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

void moduleConfigurator::handleDuplicateNames(QStringList &list)
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

QString moduleConfigurator::moduleName()
{
    return ui->leBoardName->text().isEmpty() ? "MyModule":ui->leBoardName->text() ;
}

void moduleConfigurator::setModuleName(const QString &text)
{
    ui->leBoardName->setText(text);
}

QString moduleConfigurator::GPIO1()
{
    return QString(ui->cbPort1->currentText());
}

void moduleConfigurator::setGPIO1(const QString &text)
{
    ui->cbPort1->setCurrentText(text);
}

QString moduleConfigurator::GPIO2()
{
    return QString(ui->cbPort2->currentText());
}

void moduleConfigurator::setGPIO2(const QString &text)
{
    ui->cbPort2->setCurrentText(text);
}

QString moduleConfigurator::GPIO3()
{
    return QString(ui->cbPort3->currentText());
}

void moduleConfigurator::setGPIO3(const QString &text)
{
    ui->cbPort3->setCurrentText(text);
}

QString moduleConfigurator::GPIO4()
{
    return QString(ui->cbPort4->currentText());
}

void moduleConfigurator::setGPIO4(const QString &text)
{
    ui->cbPort4->setCurrentText(text); }

QString moduleConfigurator::GPIO5()
{
    return QString(ui->cbPort5->currentText());
}

void moduleConfigurator::setGPIO5(const QString &text)
{
    ui->cbPort5->setCurrentText(text);
}

QString moduleConfigurator::GPIO6()
{
    return QString(ui->cbPort6->currentText());
}

void moduleConfigurator::setGPIO6(const QString &text)
{
    ui->cbPort6->setCurrentText(text);
}

QString moduleConfigurator::GPIO7()
{
    return QString(ui->cbPort7->currentText());
}

void moduleConfigurator::setGPIO7(const QString &text)
{
    ui->cbPort7->setCurrentText(text);
}

QString moduleConfigurator::GPIO8()
{
    return QString(ui->cbPort8->currentText());
}

void moduleConfigurator::setGPIO8(const QString &text)
{
    ui->cbPort8->setCurrentText(text);
}

bool moduleConfigurator::LEDsEnabled()
{
   return  ui->enableLED->isChecked();
}

void moduleConfigurator::setLEDsEnabled(bool flag)
{
    ui->enableLED->setChecked(flag);
}

bool moduleConfigurator::tempSensorEnabled()
{
     return  ui->enableThermometer->isChecked();
}

void moduleConfigurator::setTempSensorEnabled(bool flag)
{
    ui->enableThermometer->setChecked(flag);
}

bool moduleConfigurator::lightSensorEnabled()
{
     return  ui->enableLightSensor->isChecked();
}

void moduleConfigurator::setLightSensorEnabled(bool flag)
{
    ui->enableLightSensor->setChecked(flag);
}

int moduleConfigurator::readWriteCycle()
{
     return  ui->readWriteCycle->value();
}

void moduleConfigurator::setReadWriteCycle(int value)
{
    ui->readWriteCycle->setValue(value);
}

QStringList moduleConfigurator::currentlySelectedGPIO()
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

QList< GPIOWidgetPair > moduleConfigurator::GPIOSelectionWidgets() const
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


