#ifndef MOTION_AUDIOPACKET_HPP
#define MOTION_AUDIOPACKET_HPP

#include <cstdint>
#include <cstddef>
#include <memory>
#include <cstring>

namespace mt
{
    namespace priv
    {
        class AudioPacket
        {
        private:
            int16_t* m_samples;
            std::size_t m_sampleBufferLength;

        public:
            AudioPacket(std::size_t SampleCount, std::size_t ChannelCount);
            AudioPacket(void* SamplesSource, std::size_t SampleCount, std::size_t ChannelCount);
            ~AudioPacket();
            const int16_t* GetSamplesBuffer();
            const std::size_t GetSamplesBufferLength();
        };

        typedef std::shared_ptr<mt::priv::AudioPacket> AudioPacketPtr;
    }
}

#endif
