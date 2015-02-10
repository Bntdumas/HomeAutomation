#include "atmelModuleConfigurator.h"
#include "ui_atmelModuleConfigurator.h"

#include <QStringListModel>
#include <QStringList>
#include <QComboBox>
#include <QPair>

atmelModuleConfigurator::atmelModuleConfigurator(QMap<QString, QString> devicesType, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::atmelModuleConfigurator),
    m_deviceTypes(devicesType)
{
    ui->setupUi(this);


    QStringListModel *mdl = new QStringListModel(this);
    mdl->setStringList(m_deviceTypes.keys());

    const QString defaultValue = devicesType.key("other");
    Q_FOREACH(QComboBox *currentGPIO, GPIOSelectionWidgets()) {
        currentGPIO->setModel(mdl);
        currentGPIO->setCurrentText(defaultValue);
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
    QMap<int, QPair<QString, QString> >  gpioStatus;
    const QStringList currentDevicesStatus = currentlySelectedGPIO();
    int GPIOIndex = 0;
    Q_FOREACH (const QString &currentDevice, currentDevicesStatus) {
        QPair<QString, QString>  currentGpioStatus;
        currentGpioStatus.first = currentDevice;
        currentGpioStatus.second = m_deviceTypes.value(currentDevice);
        gpioStatus.insert(++GPIOIndex, currentGpioStatus);
    }
    return qVariantFromValue(gpioStatus);
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
    Q_FOREACH(const QComboBox *currentGPIO, GPIOSelectionWidgets()) {
        list.append(currentGPIO->currentText());
    }
    return list;
}

QList<QComboBox *> atmelModuleConfigurator::GPIOSelectionWidgets()
{
    QList<QComboBox *> list;
    list << ui->cbPort1
         << ui->cbPort2
         << ui->cbPort3
         << ui->cbPort4
         << ui->cbPort5
         << ui->cbPort6
         << ui->cbPort7
         << ui->cbPort8;
    return list;
}
