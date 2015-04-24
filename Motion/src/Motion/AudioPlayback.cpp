#ifndef MOTION_AUDIOPLAYBACK_CPP
#define MOTION_AUDIOPLAYBACK_CPP

#include <Motion/AudioPlayback.h>
#include <Motion/AudioPlayback.hpp>

namespace mt
{
    AudioPlayback::AudioPlayback(DataSource& DataSource, sf::Time AudioOffsetCorrection) :
        m_audioplayrate(sf::Time::Zero),
        m_channelcount(0),
        m_audioposition(),
        m_offsetcorrection(AudioOffsetCorrection),
        m_updateclock(),
        m_datasource(&DataSource),
        m_protectionlock(),
        m_queuedaudiopackets(),
        m_activepacket(nullptr)
    {
        SourceReloaded();
        sf::Lock lock(m_datasource->m_playbacklock);
        m_datasource->m_audioplaybacks.push_back(this);
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
        stop();
    }

    bool AudioPlayback::onGetData(Chunk& data)
    {
        bool hasdata = false;
        while (!hasdata && m_datasource && !m_datasource->m_eofreached)
        {
            {
                sf::Lock lock(m_protectionlock);
                hasdata = m_queuedaudiopackets.size() > 0;
            }
            if(!hasdata) std::this_thread::sleep_for(std::chrono::milliseconds(3));
        }
        sf::Lock lock(m_protectionlock);
        m_audioposition -= m_updateclock.restart() * getPitch();
        if (m_datasource && hasdata)
        {
            if (m_offsetcorrection != sf::Time::Zero && m_audioposition >= m_offsetcorrection)
            {
                // add samples
                std::size_t samplecount = static_cast<std::size_t>(std::floor(m_audioposition.asSeconds() * m_audioplayrate.asSeconds() * m_channelcount));
                std::vector<int16_t> newsamples;
                newsamples.resize(samplecount);
                m_activepacket = std::make_shared<priv::AudioPacket>(&newsamples[0], samplecount / m_channelcount, m_channelcount);
                m_audioposition -= sf::seconds(m_activepacket->GetSamplesBufferLength() / m_audioplayrate.asSeconds() / m_channelcount);
            }
            else if (m_offsetcorrection != sf::Time::Zero && m_audioposition < -m_offsetcorrection)
            {
                // skip samples
                while (m_audioposition <= -m_offsetcorrection && m_queuedaudiopackets.size() > 1)
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

    void AudioPlayback::SourceReloaded()
    {
        if (m_datasource->HasAudio())
        {
            m_audioplayrate = sf::seconds(static_cast<float>(m_datasource->GetAudioSampleRate()));
            m_channelcount = m_datasource->GetAudioChannelCount();
            initialize(m_channelcount, m_datasource->GetAudioSampleRate());
            setPitch(m_datasource->GetPlaybackSpeed());
            StateChanged(m_datasource->GetState(), m_datasource->GetState());
        }
    }

    void AudioPlayback::StateChanged(State PreviousState, State NewState)
    {
        if (NewState == State::Playing && m_datasource->HasAudio())
        {
            play();
            sf::Lock lock(m_protectionlock);
            m_updateclock.restart();
        }
        else if (NewState == State::Paused && m_datasource->HasAudio())
        {
            pause();
        }
        else if (NewState == State::Stopped)
        {
            stop();
            sf::Lock lock(m_protectionlock);
            m_audioposition = sf::Time::Zero;
            while (m_queuedaudiopackets.size() > 0)
            {
                m_queuedaudiopackets.pop();
            }
        }
    }

    const float AudioPlayback::GetVolume()
    {
        return getVolume();
    }

    void AudioPlayback::SetVolume(float Volume)
    {
        setVolume(Volume);
    }

    const sf::Time AudioPlayback::GetOffsetCorrection()
    {
        sf::Lock lock(m_protectionlock);
        return m_offsetcorrection;
    }

    void AudioPlayback::SetOffsetCorrection(sf::Time OffsetCorrection)
    {
        sf::Lock lock(m_protectionlock);
        m_offsetcorrection = OffsetCorrection;
    }
}

mtAudioPlayback* mtAudioPlayback_Create(mtDataSource* DataSource, sfTime OffsetCorrection)
{
    mtAudioPlayback* audioplayback = new mtAudioPlayback();
    audioplayback->Value = new mt::AudioPlayback(*DataSource->Value, sf::microseconds(OffsetCorrection.microseconds));
    return audioplayback;
}

void mtAudioPlayback_Destroy(mtAudioPlayback* AudioPlayback)
{
    delete AudioPlayback->Value;
    delete AudioPlayback;
}

float mtAudioPlayback_GetVolume(mtAudioPlayback* AudioPlayback)
{
    return AudioPlayback->Value->GetVolume();
}

void mtAudioPlayback_SetVolume(mtAudioPlayback* AudioPlayback, float Volume)
{
    AudioPlayback->Value->SetVolume(Volume);
}

sfTime mtAudioPlayback_GetOffsetCorrection(mtAudioPlayback* AudioPlayback)
{
    sfTime retval;
    retval.microseconds = AudioPlayback->Value->GetOffsetCorrection().asMicroseconds();
    return retval;
}

void mtAudioPlayback_SetOffsetCorrection(mtAudioPlayback* AudioPlayback, sfTime OffsetCorrection)
{
    AudioPlayback->Value->SetOffsetCorrection(sf::microseconds(OffsetCorrection.microseconds));
}

#endif
