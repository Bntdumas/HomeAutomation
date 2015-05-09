#pragma once

#include <QWidget>
#include <QMap>
#include <QVariant>
#include <QDebug>

namespace Ui {
class atmelModuleConfigurator;
}

class QComboBox;
class QLineEdit;

typedef  QPair<QComboBox *, QLineEdit *>  GPIOWidgetPair;

/**
 * @brief Allow the user to configure the atmel module by speficfying what's plugged on it's GPIO, and some other settings.
 */
class atmelModuleConfigurator : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(QString moduleName READ moduleName WRITE setModuleName NOTIFY moduleNameChanged)

    Q_PROPERTY(QVariant allGPIO READ allGPIO)

    Q_PROPERTY(QString GPIO1 READ GPIO1 WRITE setGPIO1 NOTIFY GPIO1Changed)
    Q_PROPERTY(QString GPIO2 READ GPIO2 WRITE setGPIO2 NOTIFY GPIO2Changed)
    Q_PROPERTY(QString GPIO3 READ GPIO3 WRITE setGPIO3 NOTIFY GPIO3Changed)
    Q_PROPERTY(QString GPIO4 READ GPIO4 WRITE setGPIO4 NOTIFY GPIO4Changed)

    Q_PROPERTY(QString GPIO5 READ GPIO5 WRITE setGPIO5 NOTIFY GPIO5Changed)
    Q_PROPERTY(QString GPIO6 READ GPIO6 WRITE setGPIO6 NOTIFY GPIO6Changed)
    Q_PROPERTY(QString GPIO7 READ GPIO7 WRITE setGPIO7 NOTIFY GPIO7Changed)
    Q_PROPERTY(QString GPIO8 READ GPIO8 WRITE setGPIO8 NOTIFY GPIO8Changed)

    Q_PROPERTY(bool LEDsEnabled READ LEDsEnabled WRITE setLEDsEnabled NOTIFY LEDsEnabledChanged)
    Q_PROPERTY(bool tempSensorEnabled READ tempSensorEnabled WRITE setTempSensorEnabled NOTIFY tempSensorEnabledChanged)
    Q_PROPERTY(bool lightSensorEnabled READ lightSensorEnabled WRITE setLightSensorEnabled NOTIFY lightSensorEnabledChanged)
    Q_PROPERTY(int readWriteCycle READ readWriteCycle WRITE setReadWriteCycle NOTIFY readWriteCycleChanged)

public:
    explicit atmelModuleConfigurator(QMap<QString, QString> devicesType, QWidget *parent = 0);
    ~atmelModuleConfigurator();

    /**
     * @brief GPIOConf returns a map of all the defined GPIO entries.
     */
    QVariant  allGPIO(); /// QList<GPIOPin>

    struct GPIOPin {
        int index;
        QString name;
        QString deviceType;
        QString pinDirection; //input|output|other

        bool isValid() {
            bool dataIsNotEmpty =  (!name.isEmpty() && !deviceType.isEmpty() && !pinDirection.isEmpty());
            bool indexIsWithinRange = index >= 0;
            bool directionIsValid = (pinDirection == QLatin1String("input") ||
                                     pinDirection == QLatin1String("output")||
                                     pinDirection == QLatin1String("other"));
            return (dataIsNotEmpty && indexIsWithinRange && directionIsValid);
        }
    };

    /**
     * @brief Property getters/setters
     */
    QString moduleName();
    void setModuleName(const QString &text);

    QString GPIO1();
    void setGPIO1(const QString &text);

    QString GPIO2();
    void setGPIO2(const QString &text);

    QString GPIO3();
    void setGPIO3(const QString &text);

    QString GPIO4();
    void setGPIO4(const QString &text);

    QString GPIO5();
    void setGPIO5(const QString &text);

    QString GPIO6();
    void setGPIO6(const QString &text);

    QString GPIO7();
    void setGPIO7(const QString &text);

    QString GPIO8();
    void setGPIO8(const QString &text);

    bool LEDsEnabled();
    void setLEDsEnabled(bool flag);

    bool tempSensorEnabled();
    void setTempSensorEnabled(bool flag);

    bool lightSensorEnabled();
    void setLightSensorEnabled(bool flag);

    int readWriteCycle();
    void setReadWriteCycle(int value);

    /**
     * @brief Check if the informations entered are correct, and fill up default data if needed.
     */
    bool validate();

    /**
     * @brief Put default values in the fields that are left empty.
     */
    void fillUpEmptyFields();

    /**
     * @brief returns an ordered list containing the device names from to the lineedits
     */
    QStringList getDevicesNames() const;
signals:
    void moduleNameChanged(const QString &text);

    void GPIO1Changed(const QString &text);
    void GPIO2Changed(const QString &text);
    void GPIO3Changed(const QString &text);
    void GPIO4Changed(const QString &text);

    void GPIO5Changed(const QString &text);
    void GPIO6Changed(const QString &text);
    void GPIO7Changed(const QString &text);
    void GPIO8Changed(const QString &text);

    void LEDsEnabledChanged(bool state);
    void tempSensorEnabledChanged(bool state);
    void lightSensorEnabledChanged(bool state);
    void readWriteCycleChanged(int cycle);

protected:
    QStringList currentlySelectedGPIO();
    QList< QPair<QComboBox *, QLineEdit *> > GPIOSelectionWidgets() const;
    /**
     * @brief Rename duplicate with _2, _3, etc
     */
    void handleDuplicateNames(QStringList &list);
private:
    Ui::atmelModuleConfigurator *ui;

    /// device: <type, direction>
    QMap<QString, QString> m_deviceTypes;
};

Q_DECLARE_METATYPE(atmelModuleConfigurator::GPIOPin)
