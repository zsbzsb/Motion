#ifndef MOTION_VIDEOPLAYBACK_HPP
#define MOTION_VIDEOPLAYBACK_HPP

#include <memory>
#include <queue>
#include <cmath>

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Mutex.hpp>
#include <SFML/System/Lock.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Transform.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <Motion/Export.h>
#include <Motion/DataSource.hpp>
#include <Motion/State.hpp>
#include <Motion/priv/VideoPacket.hpp>

namespace mt
{
    class DataSource;

    class MOTION_CXX_API VideoPlayback : private sf::NonCopyable, public sf::Drawable, public sf::Transformable
    {
        friend class DataSource;

    private:
        sf::Texture m_videotexture;
        sf::Sprite m_videosprite;
        sf::Color m_buffercolor;
        DataSource* m_datasource;
        sf::Mutex m_protectionlock;
        std::queue<priv::VideoPacketPtr> m_queuedvideopackets;
        sf::Time m_elapsedtime;
        sf::Time m_frametime;
        int m_framejump;

        void SetInitialBuffer();
        void StateChanged(State PreviousState, State NewState);
        void Update(sf::Time DeltaTime);

    public:
        VideoPlayback(DataSource& DataSource, sf::Color BufferColor = sf::Color::Black);
        ~VideoPlayback();
        const sf::Color GetBufferColor();
        void SetBufferColor(sf::Color BufferColor);

    protected:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    };
}

#endif
