#ifndef UTILS_H
#define UTILS_H
#include <QObject>

struct utils {
    enum messageType {
        Info = 0,
        Success,
        Warning,
        SoftwareError,
        NetworkError
    };
};
Q_DECLARE_METATYPE(utils::messageType)

#endif // UTILS_H

