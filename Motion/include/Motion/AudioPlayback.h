#ifndef MOTION_AUDIOPLAYBACK_H
#define MOTION_AUDIOPLAYBACK_H

#include <Motion/CSFMLStructs.h>
#include <Motion/MotionStructs.h>
#include <Motion/Export.h>
#include <Motion/Types.h>

MOTION_C_API mtAudioPlayback* mtAudioPlayback_Create(mtDataSource* DataSource, sfTime AudioOffsetCorrection);
MOTION_C_API void mtAudioPlayback_Destroy(mtAudioPlayback* AudioPlayback);
MOTION_C_API float mtAudioPlayback_GetVolume(mtAudioPlayback* AudioPlayback);
MOTION_C_API void mtAudioPlayback_SetVolume(mtAudioPlayback* AudioPlayback, float Volume);

#endif
