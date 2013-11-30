#ifndef ADMINTOOLITEM_H
#define ADMINTOOLITEM_H

#include <QObject>

class adminToolItem : public QObject
{
    Q_OBJECT
public:
    explicit adminToolItem(QObject *parent = 0);
    enum messageType {
        Info = 0,
        Success,
        Warning,
        DatabaseError,
        SoftwareError
    };

Q_SIGNALS:
    void message(const QString &text, const adminToolItem::messageType type);
    
public slots:
    
};
Q_DECLARE_METATYPE(adminToolItem::messageType)

#endif // ADMINTOOLITEM_H
