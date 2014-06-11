#ifndef MOTION_DATASOURCE_H
#define MOTION_DATASOURCE_H

#include <Motion/Export.h>
#include <Motion/Types.h>

MOTION_C_API mtDataSource* mtDataSource_Create(void);
MOTION_C_API void mtDataSource_Destroy(mtDataSource* DataSource);
MOTION_C_API sfBool mtDataSource_LoadFromFile(mtDataSource* DataSource, const char* Filename, sfBool EnableVideo, sfBool EnableAudio);
MOTION_C_API void mtDataSource_Play(mtDataSource* DataSource);
MOTION_C_API void mtDataSource_Pause(mtDataSource* DataSource);
MOTION_C_API void mtDataSource_Stop(mtDataSource* DataSource);
MOTION_C_API sfBool mtDataSource_HasVideo(mtDataSource* DataSource);
MOTION_C_API sfBool mtDataSource_HasAudio(mtDataSource* DataSource);

#endif
