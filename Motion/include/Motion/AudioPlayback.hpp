#ifndef MOTION_AUDIOPLAYBACK_HPP
#define MOTION_AUDIOPLAYBACK_HPP

#include <memory>
#include <queue>
#include <thread>
#include <chrono>
#include <cmath>
#include <vector>
#include <cinttypes>

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Mutex.hpp>
#include <SFML/System/Lock.hpp>
#include <SFML/Audio/SoundStream.hpp>

#include <Motion/Export.h>
#include <Motion/DataSource.hpp>
#include <Motion/priv/AudioPacket.hpp>
#include <Motion/State.hpp>

namespace mt
{
    class DataSource;

    class MOTION_CXX_API AudioPlayback : private sf::NonCopyable, private sf::SoundStream
    {
        friend class DataSource;

    private:
        sf::Time m_audioplayrate;
        int m_channelcount;
        sf::Time m_audioposition;
        sf::Time m_audiooffsetcorrection;
        DataSource* m_datasource;
        sf::Mutex m_protectionlock;
        std::queue<priv::AudioPacketPtr> m_queuedaudiopackets;
        priv::AudioPacketPtr m_activepacket;

        bool onGetData(Chunk& data);
        void onSeek(sf::Time timeOffset);
        void StateChanged(State PreviousState, State NewState);
    public:
        AudioPlayback(DataSource& DataSource, sf::Time AudioOffsetCorrection = sf::milliseconds(300));
        ~AudioPlayback();
        void Update(sf::Time DeltaTime);
    };
}

#endif
