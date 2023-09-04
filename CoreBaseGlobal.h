#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(COREBASE_LIB)
#  define COREBASE_EXPORT Q_DECL_EXPORT
# else
#  define COREBASE_EXPORT Q_DECL_IMPORT
# endif
#else
# define COREBASE_EXPORT
#endif
