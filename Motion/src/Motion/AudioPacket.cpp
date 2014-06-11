#ifndef MOTION_AUDIOPACKET_CPP
#define MOTION_AUDIOPACKET_CPP

#include <Motion/priv/AudioPacket.hpp>

namespace mt
{
    namespace priv
    {
        AudioPacket::AudioPacket(uint8_t* SamplesSource, std::size_t SampleCount, std::size_t ChannelCount) :
            m_samples(new int16_t[SampleCount * ChannelCount]),
            m_samplebufferlength(SampleCount * ChannelCount)
        {
            std::memcpy(m_samples, SamplesSource, sizeof(uint16_t)* SampleCount * ChannelCount);
        }

        AudioPacket::~AudioPacket()
        {
            delete[] m_samples;
        }
    }
}

#endif
