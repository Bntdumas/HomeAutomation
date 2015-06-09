#ifndef CLIENTSIMULATOR_GLOBAL_H
#define CLIENTSIMULATOR_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(CLIENTSIMULATOR_LIBRARY)
#  define CLIENTSIMULATORSHARED_EXPORT Q_DECL_EXPORT
#else
#  define CLIENTSIMULATORSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // CLIENTSIMULATOR_GLOBAL_H
