#ifndef MOTION_VIDOEPLAYBACK_H
#define MOTION_VIDEOPLAYBACK_H

#include <Motion/CSFMLStructs.h>
#include <Motion/MotionStructs.h>
#include <Motion/Export.h>
#include <Motion/Types.h>

MOTION_C_API mtVideoPlaybackBase* mtVideoPlaybackBase_Create(mtDataSource* DataSource, mtCreateTextureCB CreateCB, mtUpdateTextureCB UpdateCB, mtClearTextureCB ClearCB);
MOTION_C_API void mtVideoPlaybackBase_Destroy(mtVideoPlaybackBase* VideoPlayback);
MOTION_C_API unsigned int mtVideoPlayback_GetPlayedFrameCount(mtVideoPlaybackBase* VideoPlayback);

MOTION_C_API mtSFMLVideoPlayback* mtSFMLVideoPlayback_Create(mtDataSource* DataSource, sfColor BufferColor);
MOTION_C_API void mtSFMLVideoPlayback_Destroy(mtSFMLVideoPlayback* VideoPlayback);
MOTION_C_API sfColor mtSFMLVideoPlayback_GetBufferColor(mtSFMLVideoPlayback* VideoPlayback);
MOTION_C_API void mtSFMLVideoPlayback_SetBufferColor(mtSFMLVideoPlayback* VideoPlayback, sfColor BufferColor);
MOTION_C_API void mtSFMLVideoPlayback_DrawRenderWindow(mtSFMLVideoPlayback* VideoPlayback, sfRenderWindow* RenderWindow, sfRenderStates* RenderStates);
MOTION_C_API void mtSFMLVideoPlayback_DrawRenderTexture(mtSFMLVideoPlayback* VideoPlayback, sfRenderTexture* RenderTexture, sfRenderStates* RenderStates);
MOTION_C_API void mtSFMLVideoPlayback_GetLastFrame(mtSFMLVideoPlayback* VideoPlayback, sfImage* Image);
MOTION_C_API unsigned int mtSFMLVideoPlayback_GetPlayedFrameCount(mtSFMLVideoPlayback* VideoPlayback);

#endif
