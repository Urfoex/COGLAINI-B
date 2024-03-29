#ifndef __SIGNAL_CATCH_H__
#define __SIGNAL_CATCH_H__

#if defined( __linux__ ) && !defined( linux)
#define linux 1
#endif
#define USE_GLUT

#include <XnPlatform.h>

#if XN_PLATFORM == XN_PLATFORM_WIN32

#define CatchSignals(x)

#else
// Stores pointer to bSignalWasCaught, and starts catching signals
// when a signal is caught, *bSignalWasCaught will be set to true
void CatchSignals(XnBool* bSignalWasCaught);
#endif

#endif
