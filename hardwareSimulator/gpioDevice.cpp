#include "gpioDevice.h"
#include "ui_gpioDevice.h"

#include <QTimer>
#include <QPalette>
#include <QColor>

gpioDevice::gpioDevice(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::gpioDevice)
{
    ui->setupUi(this);
}

gpioDevice::~gpioDevice()
{
    delete ui;
}

void gpioDevice::setLampState(const bool&state)
{
    QPalette pal = ui->lblOutputText->palette();
    pal.setColor(QPalette::WindowText, QColor(Qt::black));
    ui->lblOutputText->setPalette(pal);
    ui->lblOutputText->setText(state ? tr("ON"):("OFF"));
}

void gpioDevice::setRGBLampState(const QColor& color, const bool state)
{
    ui->lblOutputText->setText(state ? tr("ON"):("OFF"));
    QPalette pal(ui->lblOutputText->palette());
    pal = QPalette(ui->lblOutputText->palette());
    if (state) {
        pal.setColor(QPalette::WindowText, color);
    } else {
        pal.setColor(QPalette::WindowText, QColor(Qt::black));
    }
    ui->lblOutputText->setPalette(pal);
}


void gpioDevice::switchComputer()
{
    ui->lblOutputText->setText(tr("CLOSED"));
    QTimer::singleShot(2000, this, SLOT(switchComputerBack()));
}


void gpioDevice::setLCDText(const QStringList &text)
{
    // Make sure to only display authorized amount of lines/characters
    for (int i = 0; i < ui->spxLCDLines->value(); ++i) {
        ui->txtLCDOutput->appendPlainText(text.at(i).left(ui->spxLCDCharacters->value()));
    }
}


void gpioDevice::setupDevice(deviceType type)
{
    switch (type) {
    case Lamp_Ambiance:
        ui->lblType->setText(QStringLiteral("Ambiance lamp"));
        ui->lblOutputText->setText(tr("OFF"));
        ui->deviceOptions->setCurrentWidget(ui->pageText);
        break;

    case Lamp_RoomLight:
        ui->lblType->setText(QStringLiteral("Room lamp"));
        ui->lblOutputText->setText(tr("OFF"));
        ui->deviceOptions->setCurrentWidget(ui->pageText);
        break;

    case Lamp_SpotLight:
        ui->lblType->setText(QStringLiteral("Spotlight lamp"));
        ui->lblOutputText->setText(tr("OFF"));
        ui->deviceOptions->setCurrentWidget(ui->pageText);
        break;

    case Lamp_RGB:
        ui->lblType->setText(QStringLiteral("RGB lamp"));
        ui->lblOutputText->setText(tr("OFF"));
        ui->deviceOptions->setCurrentWidget(ui->pageText);
        break;

    case Lamp_LED:
        ui->lblType->setText(QStringLiteral("Led lamp"));
        ui->lblOutputText->setText(tr("OFF"));
        ui->deviceOptions->setCurrentWidget(ui->pageText);
        break;

    case ComputerSwitch:
        ui->lblType->setText(QStringLiteral("Computer switch"));
        ui->lblOutputText->setText(tr("OPEN"));
        ui->deviceOptions->setCurrentWidget(ui->pageText);
        break;

    case Sensor_Temperature:
        ui->lblType->setText(QStringLiteral("Temperature sensor"));
        ui->deviceOptions->setCurrentWidget(ui->pageSensor);
        ui->spxSensor->setRange(-50, 50);
        ui->spxSensor->setSingleStep(0.5);
        connect(ui->spxSensor, SIGNAL(valueChanged(double)), this, SIGNAL(sensorChanged(double)));
        break;

    case Sensor_Light:
        ui->lblType->setText(QStringLiteral("Light sensor"));
        ui->deviceOptions->setCurrentWidget(ui->pageSensor);
        ui->spxSensor->setRange(0, 100);
        ui->spxSensor->setSingleStep(1);
        connect(ui->spxSensor, SIGNAL(valueChanged(double)), this, SIGNAL(sensorChanged(double)));
        break;

    case Sensor_Humidity:
        ui->lblType->setText(QStringLiteral("Humidity sensor"));
        ui->deviceOptions->setCurrentWidget(ui->pageSensor);
        ui->spxSensor->setRange(0, 100);
        ui->spxSensor->setSingleStep(1);
        connect(ui->spxSensor, SIGNAL(valueChanged(double)), this, SIGNAL(sensorChanged(double)));
        break;

    case User_PushButton:
        ui->lblType->setText(QStringLiteral("Physical button"));
        ui->deviceOptions->setCurrentWidget(ui->pageButton);
        connect(ui->cbButtonHold, SIGNAL(toggled(bool)), this, SIGNAL(buttonToggled(bool)));
        connect(ui->cbButtonHold, SIGNAL(toggled(bool)), ui->btButton, SLOT(toggle()));
        connect(ui->btButton, SIGNAL(clicked()), this, SIGNAL(buttonClicked()));
        break;

    case User_Potentiometer:
        ui->lblType->setText(QStringLiteral("Potentiometer"));
        ui->deviceOptions->setCurrentWidget(ui->pagePotentiometer);
        connect(ui->dlPotentiometer, SIGNAL(sliderMoved(int)), this, SIGNAL(potentiometerChanged(int)));
        connect(ui->dlPotentiometer, SIGNAL(sliderMoved(int)), ui->lcdPotentiometerPercent, SLOT(display(int)));
        break;

    default:
        qWarning(("Didn't recognize device type"));
        break;
    }
}

void gpioDevice::switchComputerBack()
{
    ui->lblOutputText->setText(tr("OPEN"));
}
