#ifndef HOUSEDATAHANDLER_H
#define HOUSEDATAHANDLER_H

#include "houseDataHandler_global.h"
#include <QObject>

/**
 * @brief This class holds the current state of the house
 */

class HOUSEDATAHANDLERSHARED_EXPORT houseDataHandler: public QObject
{
    Q_OBJECT
public:
    explicit houseDataHandler(QObject *parent = Q_NULLPTR);
    ~houseDataHandler();

};

#endif // HOUSEDATAHANDLER_H

