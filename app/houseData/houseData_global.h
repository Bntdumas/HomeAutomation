#ifndef HOUSEDATAGLOBAL_H
#define HOUSEDATAGLOBAL_H

#include <QtCore/qglobal.h>

#if defined(HOUSEDATA_LIBRARY)
#  define HOUSEDATASHARED_EXPORT Q_DECL_EXPORT
#else
#  define HOUSEDATASHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // HOUSEDATAGLOBAL_H