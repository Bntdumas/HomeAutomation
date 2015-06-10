#ifndef ADMINTOOLITEM_H
#define ADMINTOOLITEM_H

#include <QObject>
#include "utils.h"

/**
 * @brief The adminToolItem class is a base class for tools outputing information to the admin window.
 */
class adminToolItem : public QObject
{
    Q_OBJECT
public:
    explicit adminToolItem(QObject *parent = 0) : QObject(parent){}

Q_SIGNALS:
    void message(const QString &text, const utils::messageType type);
};


#endif // ADMINTOOLITEM_H
