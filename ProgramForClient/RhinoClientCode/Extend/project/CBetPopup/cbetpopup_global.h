#ifndef CBETPOPUP_GLOBAL_H
#define CBETPOPUP_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef CBETPOPUP_LIB
# define CBETPOPUP_EXPORT Q_DECL_EXPORT
#else
# define CBETPOPUP_EXPORT Q_DECL_IMPORT
#endif

#endif // CBETPOPUP_GLOBAL_H
