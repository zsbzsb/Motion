#ifndef MOTION_DATASOURCE_HPP
#define MOTION_DATASOURCE_HPP

#include <string>
#include <cstdint>
#include <memory>
#include <thread>
#include <chrono>
#include <atomic>
#include <queue>
#include <vector>
#include <iostream>

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Mutex.hpp>
#include <SFML/System/Lock.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Clock.hpp>

#include <Motion/Export.h>
#include <Motion/priv/AudioPacket.hpp>
#include <Motion/AudioPlaybackBase.hpp>
#include <Motion/priv/VideoPacket.hpp>
#include <Motion/VideoPlaybackBase.hpp>
#include <Motion/State.hpp>

extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
#include <libavutil/opt.h>
}

namespace mt
{
    class VideoPlaybackBase;
    class AudioPlayback;

    class MOTION_CXX_API DataSource : private sf::NonCopyable
    {
        friend class VideoPlaybackBase;
        friend class AudioPlaybackBase;

    private:
        sf::Clock m_updateClock;
        sf::Time m_playingOffset;
        sf::Time m_fileLength;
        sf::Vector2i m_videoSize;
        int m_audioChannelCount;
        float m_playbackSpeed;

        int m_videoStreamID;
        int m_audioStreamID;
        AVFormatContext* m_formatContext;
        AVCodecContext* m_videoContext;
        AVCodecContext* m_audioContext;
        AVCodec* m_videoCodec;
        AVCodec* m_audioCodec;
        AVFrame* m_videoRawFrame;
        AVFrame* m_videoRGBAFrame;
        AVFrame* m_audioRawBuffer;
        uint8_t* m_videoRawBuffer;
        uint8_t* m_videoRGBABuffer;
        uint8_t* m_audioPCMBuffer;
        SwsContext* m_videoSWContext;
        SwrContext* m_audioSWContext;

        State m_state;
        std::unique_ptr<std::thread> m_decodeThread;
        std::atomic<bool> m_shouldThreadRun;
        std::atomic<bool> m_EOFReached;
        std::atomic<bool> m_PlayingToEOF;

        sf::Mutex m_playbackMutex;
        std::vector<mt::VideoPlaybackBase*> m_videoPlaybacks;
        std::vector<mt::AudioPlaybackBase*> m_audioPlaybacks;

        AVFrame* CreatePictureFrame(AVPixelFormat SelectedPixelFormat, int Width, int Height, uint8_t*& PictureBuffer);
        void DestroyPictureFrame(AVFrame*& PictureFrame, uint8_t*& PictureBuffer);
        void Cleanup();
        void StartDecodeThread();
        void StopDecodeThread();
        void DecodeThreadRun();
        bool IsFull();
        void NotifyStateChanged(State NewState);

    public:
        DataSource();
        ~DataSource();
        bool LoadFromFile(const std::string& Filename, bool EnableVideo = true, bool EnableAudio = true);
        void Play();
        void Pause();
        void Stop();
        bool HasVideo() const;
        bool HasAudio() const;
        sf::Vector2i GetVideoSize() const;
        State GetState() const;
        sf::Time GetVideoFrameTime() const;
        int GetAudioChannelCount() const;
        int GetAudioSampleRate() const;
        sf::Time GetFileLength() const;
        sf::Time GetPlayingOffset() const;
        void SetPlayingOffset(sf::Time PlayingOffset);
        void Update();
        float GetPlaybackSpeed() const;
        void SetPlaybackSpeed(float PlaybackSpeed);
        bool IsEndofFileReached() const;
    };
}

#endif
