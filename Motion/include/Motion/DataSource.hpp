#ifndef MOTION_DATASOURCE_HPP
#define MOTION_DATASOURCE_HPP

#include <string>
#include <cstdint>

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Vector2.hpp>

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
        AVFrame* CreatePictureFrame(enum PixelFormat SelectedPixelFormat, int Width, int Height, unsigned char*& PictureBuffer);
        void DestroyPictureFrame(AVFrame*& PictureFrame, unsigned char*& PictureBuffer);
        void Cleanup();
    public:
        DataSource();
        ~DataSource();
        bool LoadFromFile(const std::string& Filename, bool EnableVideo = true, bool EnableAudio = true);
        void Play();
        void Pause();
        void Stop();
        bool HasVideo();
        bool HasAudio();
    };
}

#endif
