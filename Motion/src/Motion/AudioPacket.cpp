#ifndef MOTION_AUDIOPACKET_CPP
#define MOTION_AUDIOPACKET_CPP

#include <Motion/priv/AudioPacket.hpp>

namespace mt
{
    namespace priv
    {
        AudioPacket::AudioPacket(std::size_t SampleCount, std::size_t ChannelCount) :
            m_samples(new int16_t[SampleCount * ChannelCount]),
            m_sampleBufferLength(SampleCount * ChannelCount)
        {
            std::memset(m_samples, 0, sizeof(uint16_t) * m_sampleBufferLength);
        }

        AudioPacket::AudioPacket(void* SamplesSource, std::size_t SampleCount, std::size_t ChannelCount) :
            m_samples(new int16_t[SampleCount * ChannelCount]),
            m_sampleBufferLength(SampleCount * ChannelCount)
        {
            std::memcpy(m_samples, SamplesSource, sizeof(uint16_t) * m_sampleBufferLength);
        }

        AudioPacket::~AudioPacket()
        {
            delete[] m_samples;
        }

        const int16_t* AudioPacket::GetSamplesBuffer()
        {
            return m_samples;
        }

        const std::size_t AudioPacket::GetSamplesBufferLength()
        {
            return m_sampleBufferLength;
        }
    }
}

#endif
