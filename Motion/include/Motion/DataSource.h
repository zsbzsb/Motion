#ifndef MOTION_DATASOURCE_H
#define MOTION_DATASOURCE_H

#include <Motion/Export.h>
#include <Motion/Types.h>

MOTION_C_API mtDataSource* mtDataSource_Create(void);
MOTION_C_API void mtDataSource_Destroy(mtDataSource* DataSource);
MOTION_C_API sfBool mtDataSource_LoadFromFile(mtDataSource* DataSource, const char* Filename, sfBool EnableVideo, sfBool EnableAudio);

#endif
