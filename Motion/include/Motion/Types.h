#ifndef MOTION_TYPES_H
#define MOTION_TYPES_H

// CSFML Types
typedef struct sfRenderTexture sfRenderTexture;
typedef struct sfRenderWindow sfRenderWindow;
typedef struct sfView sfView;
typedef struct sfShader sfShader;
typedef struct sfTexture sfTexture;
typedef struct sfImage sfImage;
typedef int sfBool;
#define sfFalse 0
#define sfTrue  1
// Motion Types
typedef struct mtDataSource mtDataSource;
typedef struct mtAudioPlaybackBase mtAudioPlaybackBase;
typedef struct mtVideoPlaybackBase mtVideoPlaybackBase;
typedef struct mtSFMLAudioPlayback mtSFMLAudioPlayback;
typedef struct mtSFMLVideoPlayback mtSFMLVideoPlayback;
typedef void(*mtCreateTextureCB)(int Width, int Height);
typedef void(*mtUpdateTextureCB)(const uint8_t* RGBABuffer);
typedef void(*mtClearTextureCB)();
typedef void(*mtSetupStreamCB)(unsigned int ChannelCount, int SampleRate);
typedef void(*mtSetPlaybackSpeedCB)(float PlaybackSpeed);
typedef void(*mtChangeStateCB)();

#endif
