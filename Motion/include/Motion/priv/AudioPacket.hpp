#ifndef MOTION_AUDIOPACKET_HPP
#define MOTION_AUDIOPACKET_HPP

#include <cstdint>
#include <cstddef>
#include <memory>
#include <cstring>

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
    namespace priv
    {
        class AudioPacket
        {
        private:
            int16_t* m_samples;
            std::size_t m_samplebufferlength;
        public:
            AudioPacket(void* SamplesSource, std::size_t SampleCount, std::size_t ChannelCount);
            ~AudioPacket();
            const int16_t* GetSamplesBuffer();
            const std::size_t GetSamplesBufferLength();
        };

        typedef std::shared_ptr<mt::priv::AudioPacket> AudioPacketPtr;
    }
}

#endif
