#ifndef MOTION_AUDIOPLAYBACKBASE_HPP
#define MOTION_AUDIOPLAYBACKBASE_HPP

#include <memory>
#include <queue>
#include <thread>
#include <chrono>
#include <cmath>
#include <vector>
#include <cstdint>

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Mutex.hpp>
#include <SFML/System/Lock.hpp>
#include <SFML/System/Clock.hpp>

#include <Motion/Export.h>
#include <Motion/DataSource.hpp>
#include <Motion/priv/AudioPacket.hpp>
#include <Motion/State.hpp>

namespace mt
{
    class DataSource;

    class MOTION_CXX_API AudioPlaybackBase : private sf::NonCopyable
    {
        friend class DataSource;

    private:
        DataSource* m_dataSource;
        bool m_initialSetupDone;

        int m_sampleRate;
        int m_channelCount;
        sf::Time m_offsetCorrection;

        sf::Mutex m_protectionMutex;
        std::queue<priv::AudioPacketPtr> m_queuedAudioPackets;
        priv::AudioPacketPtr m_activePacket;
        sf::Time m_audioPosition;

        sf::Time GetPacketLength(const priv::AudioPacketPtr& packet) const;

        void WaitForData();
        void SourceReloaded();
        void StateChanged(State PreviousState, State NewState);
        void Update(sf::Time DeltaTime);

    public:
        virtual ~AudioPlaybackBase();
        sf::Time GetOffsetCorrection();
        void SetOffsetCorrection(sf::Time OffsetCorrection);

    protected:
        AudioPlaybackBase(DataSource& DataSource, sf::Time OffsetCorrection = sf::milliseconds(50));
        DataSource* GetDataSource() const;
        bool GetNextBuffer(const int16_t*& Samples, std::size_t& SampleCount);
        virtual void SetupStream(unsigned int ChannelCount, int SampleRate) = 0;
        virtual void SetPlaybackSpeed(float PlaybackSpeed) = 0;
        virtual void StartStream() = 0;
        virtual void PauseStream() = 0;
        virtual void StopStream() = 0;
    };
}

#endif
