#ifndef MOTION_VIDOEPLAYBACK_H
#define MOTION_VIDEOPLAYBACK_H

#include <Motion/CSFMLStructs.h>
#include <Motion/MotionStructs.h>
#include <Motion/Export.h>
#include <Motion/Types.h>

MOTION_C_API mtVideoPlayback* mtVideoPlayback_Create(mtDataSource* DataSource, sfColor BufferColor);
MOTION_C_API void mtVideoPlayback_Destroy(mtVideoPlayback* VideoPlayback);
MOTION_C_API sfColor mtVideoPlayback_GetBufferColor(mtVideoPlayback* VideoPlayback);
MOTION_C_API void mtVideoPlayback_SetBufferColor(mtVideoPlayback* VideoPlayback, sfColor BufferColor);
MOTION_C_API void mtVideoPlayback_DrawRenderWindow(mtVideoPlayback* VideoPlayback, sfRenderWindow* RenderWindow, sfRenderStates* RenderStates);
MOTION_C_API void mtVideoPlayback_DrawRenderTexture(mtVideoPlayback* VideoPlayback, sfRenderTexture* RenderTexture, sfRenderStates* RenderStates);
MOTION_C_API void mtVideoPlayback_GetLastFrame(mtVideoPlayback* VideoPlayback, sfImage* Image);
MOTION_C_API unsigned int mtVideoPlayback_GetPlayedFrameCount(mtVideoPlayback* VideoPlayback);

#endif
