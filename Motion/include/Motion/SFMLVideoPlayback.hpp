#ifndef MOTION_SFMLVIDEOPLAYBACK_HPP
#define MOTION_SFMLVIDEOPLAYBACK_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Transform.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Image.hpp>

#include <Motion/Export.h>
#include <Motion/VideoPlaybackBase.hpp>

namespace mt
{
    class MOTION_CXX_API SFMLVideoPlayback : public VideoPlaybackBase, public sf::Drawable, public sf::Transformable
    {
    private:
        sf::Texture m_videotexture;
        sf::Sprite m_videosprite;
        sf::Color m_buffercolor;

        void SetInitialBuffer();
        void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    public:
        SFMLVideoPlayback(DataSource& DataSource, sf::Color BufferColor = sf::Color::Black);
        const sf::Color GetBufferColor() const;
        void SetBufferColor(sf::Color BufferColor);
        sf::Image GetLastFrame() const;

    protected:
        void CreateTexture(int Width, int Height);
        void UpdateTexture(const uint8_t* RGBABuffer);
        void ClearTexture();
    };
}

#endif
