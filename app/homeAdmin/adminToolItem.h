#ifndef ADMINTOOLITEM_H
#define ADMINTOOLITEM_H

#include <QObject>
/**
 * @brief The adminToolItem class is a base class for tools outputing information to the admin window.
 */
class adminToolItem : public QObject
{
    Q_OBJECT
public:
    explicit adminToolItem(QObject *parent = 0) : QObject(parent){}
    enum messageType {
        Info = 0,
        Success,
        Warning,
        SoftwareError
    };

Q_SIGNALS:
    void message(const QString &text, const adminToolItem::messageType type);

    
};
Q_DECLARE_METATYPE(adminToolItem::messageType)

#endif // ADMINTOOLITEM_H
