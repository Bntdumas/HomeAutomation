#ifndef DEBUGCENTER_H
#define DEBUGCENTER_H

#include <QObject>
#include "sharedElements_global.h"
#include "utils.h"

/**
 * @brief This class can output to the command line the emitted messages() from the different elements.
 */

class SHAREDELEMENTSSHARED_EXPORT debugCenter: public QObject
{
    Q_OBJECT
public:
    explicit debugCenter(QObject *parent = Q_NULLPTR) : QObject(parent) {}
    ~debugCenter() {}

    template <typename Arg>
    void addWatcher(const Arg *object) {
        // cannot use C++11 marking because of the template
        connect(object, SIGNAL(message(QString, utils::messageType)), this, SLOT(displayMessage(QString,utils::messageType)));
    }

    void setDebugVisible(bool visible) {m_showDebugs = visible;}


public Q_SLOTS:
    void displayMessage(const QString &msg, utils::messageType type);

private:
    bool m_showDebugs;
};

#endif // DEBUGCENTER_H

