#include "atmelSimulator.h"
#include "ui_atmelSimulator.h"

atmelSimulator::atmelSimulator(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::atmelSimulator)
{
    ui->setupUi(this);

    ui->embeddedTemperature->setupDevice(gpioDevice::Sensor_Temperature);
    ui->embeddedLight->setupDevice(gpioDevice::Sensor_Light);

    ui->gpio1->setupDevice(gpioDevice::Lamp_Ambiance);
    ui->gpio2->setupDevice(gpioDevice::Sensor_Temperature);
    ui->gpio3->setupDevice(gpioDevice::Lamp_RGB);
    ui->gpio4->setupDevice(gpioDevice::Lamp_RoomLight);
    ui->gpio5->setupDevice(gpioDevice::ComputerSwitch);
    ui->gpio6->setupDevice(gpioDevice::Sensor_Humidity);
    ui->gpio7->setupDevice(gpioDevice::User_PushButton);
    ui->gpio8->setupDevice(gpioDevice::User_Potentiometer);
}

atmelSimulator::~atmelSimulator()
{
    delete ui;
}
