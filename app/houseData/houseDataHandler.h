#ifndef HOUSEDATAHANDLER_H
#define HOUSEDATAHANDLER_H

#include "houseData_global.h"
#include <QObject>

/**
 * @brief This class communicates with the house data structure
 */

class HOUSEDATASHARED_EXPORT houseDataHandler: public QObject
{
    Q_OBJECT
public:

    static houseDataHandler& instance()
    {
        static houseDataHandler instance;
        return instance;
    }

    /**
     * @brief Updates the house structure with a new value from a physical device.
     */
    void changeDeviceValue(int chipid, int gpio, float value);
    void changeDeviceValue(const QString &roomName, const QString &deviceName, float value);

private:
    houseDataHandler(houseDataHandler const&) = delete;
    void operator=(houseDataHandler const&) = delete;
    explicit houseDataHandler(QObject *parent = Q_NULLPTR);
    ~houseDataHandler();

};

#endif // HOUSEDATAHANDLER_H

