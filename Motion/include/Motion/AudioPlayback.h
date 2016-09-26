#ifndef MOTION_AUDIOPLAYBACK_H
#define MOTION_AUDIOPLAYBACK_H

#include <Motion/CSFMLStructs.h>
#include <Motion/MotionStructs.h>
#include <Motion/Export.h>
#include <Motion/Types.h>

MOTION_C_API mtAudioPlaybackBase* mtAudioPlaybackBase_Create(mtDataSource* DataSource, sfTime OffsetCorrection, mtSetupStreamCB SetupCB, mtSetPlaybackSpeedCB SetSpeedCB, mtChangeStateCB StartCB, mtChangeStateCB PauseCB, mtChangeStateCB StopCB);
MOTION_C_API void mtAudioPlaybackBase_Destroy(mtAudioPlaybackBase* AudioPlayback);
MOTION_C_API sfBool mtAudioPlaybackBase_GetNextBuffer(mtAudioPlaybackBase* AudioPlayback, const int16_t** Samples, unsigned long* SampleCount);
MOTION_C_API sfTime mtAudioPlaybackBase_GetOffsetCorrection(mtAudioPlaybackBase* AudioPlayback);
MOTION_C_API void mtAudioPlaybackBase_SetOffsetCorrection(mtAudioPlaybackBase* AudioPlayback, sfTime OffsetCorrection);

MOTION_C_API mtSFMLAudioPlayback* mtSFMLAudioPlayback_Create(mtDataSource* DataSource, sfTime OffsetCorrection);
MOTION_C_API void mtSFMLAudioPlayback_Destroy(mtSFMLAudioPlayback* AudioPlayback);
MOTION_C_API float mtSFMLAudioPlayback_GetVolume(mtSFMLAudioPlayback* AudioPlayback);
MOTION_C_API void mtSFMLAudioPlayback_SetVolume(mtSFMLAudioPlayback* AudioPlayback, float Volume);
MOTION_C_API sfTime mtSFMLAudioPlayback_GetOffsetCorrection(mtSFMLAudioPlayback* AudioPlayback);
MOTION_C_API void mtSFMLAudioPlayback_SetOffsetCorrection(mtSFMLAudioPlayback* AudioPlayback, sfTime OffsetCorrection);

#endif
