#ifndef GPIODEVICE_H
#define GPIODEVICE_H

#include <QFrame>

namespace Ui {
class gpioDevice;
}

/**
 * @brief Emulates a GPIO device.
 */

class gpioDevice : public QFrame
{
    Q_OBJECT
    
public:
    explicit gpioDevice(QWidget *parent = 0);
    ~gpioDevice();

    /**
     * @brief Hardcode the device types. To keep up to date with deviceType file.
     */
    enum deviceType {
        Lamp_Ambiance = 0,
        Lamp_SpotLight,
        Lamp_RGB,
        Lamp_RoomLight,
        Lamp_LED,
        ComputerSwitch,
        User_Screen,
        Sensor_Temperature,
        Sensor_Light,
        Sensor_Humidity,
        User_PushButton,
        User_Potentiometer,

        DeviceTypeCount
    };

public:
    /**
     * @brief Setup the widget according to it's type
     */
    void setupDevice(deviceType type);

    /**
     * @brief Switch a lamp ON/OFF
     * @param Lamp is ON if state == true
     */
    void setLampState(const bool&state);

    /**
     * @brief Set the RGB lamp color/state
     * @param color: the light color
     * @param state: Lamp is ON if state == true
     */
    void setRGBLampState(const QColor& color, const bool state);

    /**
     * @brief Sends an impulse to the computer (== click on the power button)
     * @param state
     */
    void switchComputer();

    /**
     * @brief Set the LCD text.
     * @param text: One line for each list item. Must match the LCD characteristics
     */
    void setLCDText(const QStringList &text);
    
Q_SIGNALS:
    /**
     * @brief potentiometer
     */
    void potentiometerChanged(int action);

    /**
     * @brief Physical button (momentary)
     */
    void buttonClicked();

    /**
     * @brief Physical button (toggle)
     */
    void buttonToggled(bool checked);

    /**
     * @brief Sensor value
     */
    void sensorChanged(double arg1);

    /**
     * @brief Allowed characters per LCD line
     */
    void lcdCharactersChanged(int arg1);

    /**
     * @brief LCD available lines
     */
    void lcdLinesChanged(int arg1);

private Q_SLOTS:
    /**
     * @brief computer switch only gets ON for 2 seconds.
     */
    void switchComputerBack();

private:
    Ui::gpioDevice *ui;





};

#endif // GPIODEVICE_H
