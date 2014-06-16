#ifndef MOTION_DATASOURCE_H
#define MOTION_DATASOURCE_H

#include <Motion/CSFMLStructs.h>
#include <Motion/MotionStructs.h>
#include <Motion/Export.h>
#include <Motion/Types.h>

typedef enum
{
    mtStateStopped,
    mtStatePlaying,
    mtStatePaused
} mtState;

MOTION_C_API mtDataSource* mtDataSource_Create(void);
MOTION_C_API void mtDataSource_Destroy(mtDataSource* DataSource);
MOTION_C_API sfBool mtDataSource_LoadFromFile(mtDataSource* DataSource, const char* Filename, sfBool EnableVideo, sfBool EnableAudio);
MOTION_C_API void mtDataSource_Play(mtDataSource* DataSource);
MOTION_C_API void mtDataSource_Pause(mtDataSource* DataSource);
MOTION_C_API void mtDataSource_Stop(mtDataSource* DataSource);
MOTION_C_API sfBool mtDataSource_HasVideo(mtDataSource* DataSource);
MOTION_C_API sfBool mtDataSource_HasAudio(mtDataSource* DataSource);
MOTION_C_API sfVector2i mtDataSource_GetVideoSize(mtDataSource* DataSource);
MOTION_C_API mtState mtDataSource_GetState(mtDataSource* DataSource);
MOTION_C_API sfTime mtDataSource_GetVideoFrameTime(mtDataSource* DataSource);
MOTION_C_API int mtDataSource_GetAudioChannelCount(mtDataSource* DataSource);
MOTION_C_API int mtDataSource_GetAudioSampleRate(mtDataSource* DataSource);
MOTION_C_API sfTime mtDataSource_GetFileLength(mtDataSource* DataSource);
MOTION_C_API sfTime mtDataSource_GetPlayingOffset(mtDataSource* DataSource);
MOTION_C_API void mtDataSource_SetPlayingOffset(mtDataSource* DataSource, sfTime PlayingOffset);
MOTION_C_API void mtDataSource_Update(mtDataSource* DataSource);
MOTION_C_API float mtDataSource_GetPlaybackSpeed(mtDataSource* DataSource);
MOTION_C_API void mtDataSource_SetPlaybackSpeed(mtDataSource* DataSource, float PlaybackSpeed);
MOTION_C_API sfBool mtDataSource_GetIsEndofFileReached(mtDataSource* DataSource);

#endif
