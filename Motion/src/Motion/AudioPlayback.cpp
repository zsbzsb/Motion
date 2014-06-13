#ifndef MOTION_AUDIOPLAYBACK_CPP
#define MOTION_AUDIOPLAYBACK_CPP

#include <Motion/AudioPlayback.hpp>

namespace mt
{
    AudioPlayback::AudioPlayback(DataSource& DataSource, sf::Time AudioOffsetCorrection) :
        m_audioplayrate(sf::seconds(static_cast<float>(DataSource.GetAudioSampleRate()))),
        m_channelcount(DataSource.GetAudioChannelCount()),
        m_audioposition(),
        m_audiooffsetcorrection(AudioOffsetCorrection),
        m_datasource(&DataSource),
        m_protectionlock(),
        m_queuedaudiopackets(),
        m_activepacket(nullptr)
    {
        if (m_datasource->HasAudio())
        {
            m_datasource->m_audioplaybacks.push_back(this);
            initialize(m_channelcount, m_datasource->GetAudioSampleRate());
        }
    }

    AudioPlayback::~AudioPlayback()
    {
        if (m_datasource)
        {
            for (auto& audioplayback : m_datasource->m_audioplaybacks)
            {
                if (audioplayback == this)
                {
                    std::swap(audioplayback, m_datasource->m_audioplaybacks.back());
                    m_datasource->m_audioplaybacks.pop_back();
                    break;
                }
            }
            m_datasource = nullptr;
        }
    }

    bool AudioPlayback::onGetData(Chunk& data)
    {
        bool hasdata = false;
        {
            sf::Lock lock(m_protectionlock);
            hasdata = m_queuedaudiopackets.size() > 0;
        }
        while (!hasdata && m_datasource && !m_datasource->m_playingtoeof)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(3));
            {
                sf::Lock lock(m_protectionlock);
                hasdata = m_queuedaudiopackets.size() > 0;
            }
        }
        sf::Lock lock(m_protectionlock);
        if (m_datasource && hasdata)
        {
            if (m_audioposition >= m_audiooffsetcorrection)
            {
                // add samples
                std::size_t samplecount = static_cast<std::size_t>(std::floor(m_audioposition.asSeconds() * m_audioplayrate.asSeconds() * m_channelcount));
                std::vector<int16_t> newsamples;
                newsamples.resize(samplecount);
                m_activepacket = std::make_shared<priv::AudioPacket>(&newsamples[0], samplecount / m_channelcount, m_channelcount);
                m_audioposition -= sf::seconds(m_activepacket->GetSamplesBufferLength() / m_audioplayrate.asSeconds() / m_channelcount);
            }
            else if (m_audioposition < -m_audiooffsetcorrection)
            {
                // skip samples
                while (m_audioposition <= -m_audiooffsetcorrection && m_queuedaudiopackets.size() > 1)
                {
                    m_audioposition += sf::seconds(m_queuedaudiopackets.front()->GetSamplesBufferLength() / m_audioplayrate.asSeconds() / m_channelcount);
                    m_queuedaudiopackets.pop();
                }
                m_activepacket = m_queuedaudiopackets.front();
                m_queuedaudiopackets.pop();
                m_audioposition += sf::seconds(m_activepacket->GetSamplesBufferLength() / m_audioplayrate.asSeconds() / m_channelcount) * 2.f;
            }
            else
            {
                // continue as normal
                if (m_queuedaudiopackets.size() > 0)
                {
                    m_activepacket = m_queuedaudiopackets.front();
                    m_queuedaudiopackets.pop();
                    m_audioposition += sf::seconds(m_activepacket->GetSamplesBufferLength() / m_audioplayrate.asSeconds() / m_channelcount);
                }
            }
            data.samples = m_activepacket->GetSamplesBuffer();
            data.sampleCount = m_activepacket->GetSamplesBufferLength();
            return true;
        }
        else return false;
    }

    void AudioPlayback::onSeek(sf::Time timeOffset)
    {
        // nothing to do
    }

    void AudioPlayback::Update(sf::Time DeltaTime)
    {
        sf::Lock lock(m_protectionlock);
        m_audioposition -= DeltaTime;
    }
}

#endif
