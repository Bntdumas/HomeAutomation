#include "debugCenter.h"
#include <QDebug>
#include <QDateTime>

void debugCenter::displayMessage(const QString &msg, utils::messageType type)
{
    if (m_showDebugs)
        qDebug() << QDateTime::currentDateTime().toString(QStringLiteral("hh:mm:ss")) << ": " << utils::stringFromEnum(type) << msg;
}
