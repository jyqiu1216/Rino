#ifndef EXTENDT_GLOBAL_H
#define EXTENDT_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef EXTENDT_LIB
# define EXTENDT_EXPORT Q_DECL_EXPORT
#else
# define EXTENDT_EXPORT Q_DECL_IMPORT
#endif

#endif // EXTENDT_GLOBAL_H
