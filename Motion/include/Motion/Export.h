#ifndef MOTION_EXPORT_H
#define MOTION_EXPORT_H

#include <Motion/Config.h>

#if defined(MOTION_EXPORTS)
	#define MOTION_API MOTION_API_EXPORT
#else
    #define MOTION_API MOTION_API_IMPORT
#endif

#endif
