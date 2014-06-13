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

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Mutex.hpp>
#include <SFML/System/Lock.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>

#include <Motion/Export.h>
#include <Motion/priv/AudioPacket.hpp>
#include <Motion/AudioPlayback.hpp>
#include <Motion/priv/VideoPacket.hpp>
#include <Motion/VideoPlayback.hpp>

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
    class MOTION_CXX_API DataSource : private sf::NonCopyable
    {
        friend class VideoPlayback;
        friend class AudioPlayback;

    public:
        enum State
        {
            Stopped,
            Playing,
            Paused
        };

    private:
        int m_videostreamid;
        int m_audiostreamid;
        int64_t m_videolength;
        sf::Vector2i m_videosize;
        int m_audiochannelcount;
        AVFormatContext* m_formatcontext;
        AVCodecContext* m_videocontext;
        AVCodecContext* m_audiocontext;
        AVCodec* m_videocodec;
        AVCodec* m_audiocodec;
        AVFrame* m_videorawframe;
        AVFrame* m_videorgbaframe;
        AVFrame* m_audiorawbuffer;
        uint8_t* m_videorawbuffer;
        uint8_t* m_videorgbabuffer;
        uint8_t* m_audiopcmbuffer;
        SwsContext* m_videoswcontext;
        SwrContext* m_audioswcontext;
        State m_state;
        std::unique_ptr<std::thread> m_decodethread;
        std::atomic<bool> m_shouldthreadrun;
        std::atomic<bool> m_playingtoeof;
        sf::Mutex m_decodedqueuelock;
        std::queue<priv::VideoPacketPtr> m_decodedvideopackets;
        std::queue<priv::AudioPacketPtr> m_decodedaudiopackets;
        std::vector<mt::VideoPlayback*> m_videoplaybacks;
        std::vector<mt::AudioPlayback*> m_audioplaybacks;
        std::atomic<bool> m_isstarving;

        AVFrame* CreatePictureFrame(enum PixelFormat SelectedPixelFormat, int Width, int Height, unsigned char*& PictureBuffer);
        void DestroyPictureFrame(AVFrame*& PictureFrame, unsigned char*& PictureBuffer);
        void Cleanup();
        void StartDecodeThread();
        void StopDecodeThread();
        void DecodeThreadRun();

    public:
        DataSource();
        ~DataSource();
        bool LoadFromFile(const std::string& Filename, bool EnableVideo = true, bool EnableAudio = true);
        void Play();
        void Pause();
        void Stop();
        const bool HasVideo();
        const bool HasAudio();
        const sf::Vector2i GetVideoSize();
        const mt::DataSource::State GetState();
        const sf::Time GetVideoFrameTime();
        void Update();
    };
}

#endif
