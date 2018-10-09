#ifndef MOTION_AUDIOPLAYBACKBASE_CPP
#define MOTION_AUDIOPLAYBACKBASE_CPP

#include <Motion/AudioPlayback.h>
#include <Motion/AudioPlaybackBase.hpp>

constexpr std::size_t FILL_SAMPLE_COUNT = 2048;

namespace mt
{
    AudioPlaybackBase::AudioPlaybackBase(DataSource& DataSource, sf::Time AudioOffsetCorrection) :
        m_dataSource(&DataSource),
        m_initialSetupDone(false),
        m_sampleRate(0),
        m_channelCount(0),
        m_offsetCorrection(AudioOffsetCorrection),
        m_protectionMutex(),
        m_queuedAudioPackets(),
        m_activePacket(nullptr),
        m_audioPosition()
    {
        sf::Lock lock(m_dataSource->m_playbackMutex);

        m_dataSource->m_audioPlaybacks.push_back(this);
    }

    AudioPlaybackBase::~AudioPlaybackBase()
    {
        if (m_dataSource && m_initialSetupDone)
        {
            sf::Lock lock(m_dataSource->m_playbackMutex);

            for (auto& audioplayback : m_dataSource->m_audioPlaybacks)
            {
                if (audioplayback == this)
                {
                    std::swap(audioplayback, m_dataSource->m_audioPlaybacks.back());
                    m_dataSource->m_audioPlaybacks.pop_back();

                    break;
                }
            }

            m_dataSource = nullptr;
        }
    }

    void AudioPlaybackBase::Update(sf::Time DeltaTime)
    {
        if (!m_initialSetupDone)
        {
            SourceReloaded();

            m_initialSetupDone = true;
        }

        if (m_dataSource && m_dataSource->HasAudio() && m_dataSource->GetState() == State::Playing)
        {
            sf::Lock lock(m_protectionMutex);

            m_audioPosition += DeltaTime;
        }
    }

    DataSource* AudioPlaybackBase::GetDataSource() const
    {
        return m_dataSource;
    }

    bool AudioPlaybackBase::GetNextBuffer(const int16_t*& Samples, std::size_t& SampleCount)
    {
        WaitForData();

        sf::Lock lock(m_protectionMutex);

        bool hasData = m_queuedAudioPackets.size() > 0;

        if (m_dataSource && hasData)
        {
            m_activePacket = nullptr;
            bool creatingTime = false;

            if (m_offsetCorrection != sf::Time::Zero && m_audioPosition >= m_offsetCorrection)
            {
                while (m_audioPosition >= m_offsetCorrection && hasData)
                {
                    m_audioPosition -= GetPacketLength(m_queuedAudioPackets.front());

                    m_queuedAudioPackets.pop();

                    m_protectionMutex.unlock(); // unlock to wait for data
                    WaitForData();
                    m_protectionMutex.lock(); // relock after waiting for data

                    hasData = m_queuedAudioPackets.size() > 0;
                }

                if (hasData)
                {
                    m_activePacket = m_queuedAudioPackets.front();
                    m_queuedAudioPackets.pop();
                }
                else
                    creatingTime = true;
            }
            else if (m_offsetCorrection != sf::Time::Zero && m_audioPosition < -m_offsetCorrection)
                creatingTime = true;
            else
            {
                m_activePacket = m_queuedAudioPackets.front();
                m_queuedAudioPackets.pop();
            }

            if (!creatingTime)
                m_audioPosition -= GetPacketLength(m_activePacket);
            else
                m_activePacket = std::make_shared<priv::AudioPacket>(FILL_SAMPLE_COUNT, m_channelCount);
        }
        else if (!m_dataSource)
            m_activePacket = nullptr;

        if (m_activePacket != nullptr)
        {
            Samples = m_activePacket->GetSamplesBuffer();
            SampleCount = m_activePacket->GetSamplesBufferLength();

            return true;
        }
        else
            return false;
    }

    sf::Time AudioPlaybackBase::GetPacketLength(const priv::AudioPacketPtr& packet) const
    {
        return sf::seconds(static_cast<float>(packet->GetSamplesBufferLength() / static_cast<float>(m_sampleRate) / static_cast<float>(m_channelCount)));
    }

    void AudioPlaybackBase::WaitForData()
    {
        while (m_dataSource && !m_dataSource->m_EOFReached)
        {
            {
                sf::Lock lock(m_protectionMutex);

                if (m_queuedAudioPackets.size() > 0)
                    return;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }

    void AudioPlaybackBase::SourceReloaded()
    {
        if (m_dataSource->HasAudio())
        {
            m_sampleRate = m_dataSource->GetAudioSampleRate();
            m_channelCount = m_dataSource->GetAudioChannelCount();

            StopStream();
            SetupStream(m_channelCount, m_dataSource->GetAudioSampleRate());
            SetPlaybackSpeed(m_dataSource->GetPlaybackSpeed());
        }
    }

    void AudioPlaybackBase::StateChanged(State PreviousState, State NewState)
    {
        if (!m_initialSetupDone)
        {
            SourceReloaded();

            m_initialSetupDone = true;
        }

        if (NewState == State::Playing && m_dataSource->HasAudio())
            StartStream();
        else if (NewState == State::Paused && m_dataSource->HasAudio())
            PauseStream();
        else if (NewState == State::Stopped)
        {
            StopStream();

            sf::Lock lock(m_protectionMutex);

            m_audioPosition = sf::Time::Zero;

            while (m_queuedAudioPackets.size() > 0)
            {
                m_queuedAudioPackets.pop();
            }
        }
    }

    sf::Time AudioPlaybackBase::GetOffsetCorrection()
    {
        sf::Lock lock(m_protectionMutex);

        return m_offsetCorrection;
    }

    void AudioPlaybackBase::SetOffsetCorrection(sf::Time OffsetCorrection)
    {
        sf::Lock lock(m_protectionMutex);

        m_offsetCorrection = OffsetCorrection;
    }
}

DummyAudioPlayback::DummyAudioPlayback(mt::DataSource& DataSource, sf::Time OffsetCorrection, mtSetupStreamCB SetupCB, mtSetPlaybackSpeedCB SetSpeedCB, mtChangeStateCB StartCB, mtChangeStateCB PauseCB, mtChangeStateCB StopCB) :
    mt::AudioPlaybackBase(DataSource, OffsetCorrection),
    m_setupcb(SetupCB),
    m_setspeedcb(SetSpeedCB),
    m_startcb(StartCB),
    m_pausecb(PauseCB),
    m_stopcb(StopCB)
{ }

bool DummyAudioPlayback::OnGetNextBuffer(const int16_t*& Samples, std::size_t& SampleCount)
{
    return GetNextBuffer(Samples, SampleCount);
}

void DummyAudioPlayback::SetupStream(unsigned int ChannelCount, int SampleRate)
{
    if (m_setupcb) m_setupcb(ChannelCount, SampleRate);
}

void DummyAudioPlayback::SetPlaybackSpeed(float PlaybackSpeed)
{
    if (m_setspeedcb) m_setspeedcb(PlaybackSpeed);
}

void DummyAudioPlayback::StartStream()
{
    if (m_startcb) m_startcb();
}

void DummyAudioPlayback::PauseStream()
{
    if (m_pausecb) m_pausecb();
}

void DummyAudioPlayback::StopStream()
{
    if (m_stopcb) m_stopcb();
}

mtAudioPlaybackBase* mtAudioPlaybackBase_Create(mtDataSource* DataSource, sfTime OffsetCorrection, mtSetupStreamCB SetupCB, mtSetPlaybackSpeedCB SetSpeedCB, mtChangeStateCB StartCB, mtChangeStateCB PauseCB, mtChangeStateCB StopCB)
{
    mtAudioPlaybackBase* audioplayback = new mtAudioPlaybackBase();

    audioplayback->Value = new DummyAudioPlayback(*DataSource->Value, sf::microseconds(OffsetCorrection.microseconds), SetupCB, SetSpeedCB, StartCB, PauseCB, StopCB);

    return audioplayback;
}

void mtAudioPlaybackBase_Destroy(mtAudioPlaybackBase* AudioPlayback)
{
    delete AudioPlayback->Value;
    delete AudioPlayback;
}

sfBool mtAudioPlaybackBase_GetNextBuffer(mtAudioPlaybackBase* AudioPlayback, const int16_t** Samples, unsigned long* SampleCount)
{
    std::size_t tempSampleCount;
    bool gotBuffer = AudioPlayback->Value->OnGetNextBuffer(*Samples, tempSampleCount);

    *SampleCount = tempSampleCount;

    return gotBuffer;
}

sfTime mtAudioPlaybackBase_GetOffsetCorrection(mtAudioPlaybackBase* AudioPlayback)
{
    sfTime retval;

    retval.microseconds = AudioPlayback->Value->GetOffsetCorrection().asMicroseconds();

    return retval;
}

void mtAudioPlaybackBase_SetOffsetCorrection(mtAudioPlaybackBase* AudioPlayback, sfTime OffsetCorrection)
{
    AudioPlayback->Value->SetOffsetCorrection(sf::microseconds(OffsetCorrection.microseconds));
}

#endif
