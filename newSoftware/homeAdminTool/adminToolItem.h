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
        Warning,
        DatabaseError,
        SoftwareError
    };

Q_SIGNALS:
    void message(const QString &text, const messageType type);
    
public slots:
    
};
Q_DECLARE_METATYPE(messageType)

#endif // ADMINTOOLITEM_H
