#ifndef MOTION_AUDIOPLAYBACK_HPP
#define MOTION_AUDIOPLAYBACK_HPP

#include <memory>
#include <queue>

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>

#include <Motion/Export.h>
#include <Motion/DataSource.hpp>
#include <Motion/priv/AudioPacket.hpp>

namespace mt
{
    class MOTION_CXX_API AudioPlayback
    {
        friend class DataSource;

    private:

        mt::DataSource* m_datasource;
        sf::Mutex m_queuelock;
        std::queue<priv::AudioPacketPtr> m_queuedaudiopackets;
    };
}

#endif
