#ifndef MOTION_SFMLVIDEOPLAYBACK_CPP
#define MOTION_SFMLVIDEOPLAYBACK_CPP

#include <Motion/VideoPlayback.h>
#include <Motion/SFMLVideoPlayback.hpp>

namespace mt
{
    SFMLVideoPlayback::SFMLVideoPlayback(DataSource& DataSource, sf::Color BufferColor) :
        VideoPlaybackBase(DataSource),
        m_videotexture(),
        m_videosprite(),
        m_buffercolor(BufferColor)
    { }

    void SFMLVideoPlayback::SetInitialBuffer()
    {
        sf::Image initialbuffer;

        initialbuffer.create(m_videotexture.getSize().x, m_videotexture.getSize().y, m_buffercolor);

        m_videotexture.update(initialbuffer);
    }

    const sf::Color SFMLVideoPlayback::GetBufferColor() const
    {
        return m_buffercolor;
    }

    void SFMLVideoPlayback::SetBufferColor(sf::Color BufferColor)
    {
        m_buffercolor = BufferColor;
        DataSource* datasource = GetDataSource();

        if (datasource && datasource->GetState() == State::Stopped)
            SetInitialBuffer();
    }

    sf::Image SFMLVideoPlayback::GetLastFrame() const
    {
        return m_videotexture.copyToImage();
    }

    void SFMLVideoPlayback::CreateTexture(int Width, int Height)
    {
        m_videotexture.create(Width, Height);
        m_videosprite.setTexture(m_videotexture, true);
    }

    void SFMLVideoPlayback::UpdateTexture(const uint8_t* RGBABuffer)
    {
        m_videotexture.update(RGBABuffer);
    }

    void SFMLVideoPlayback::ClearTexture()
    {
        SetInitialBuffer();
    }

    void SFMLVideoPlayback::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform *= getTransform();
        target.draw(m_videosprite, states);
    }
}

mtSFMLVideoPlayback* mtSFMLVideoPlayback_Create(mtDataSource* DataSource, sfColor BufferColor)
{
    mtSFMLVideoPlayback* videoplayback = new mtSFMLVideoPlayback();
    videoplayback->Value = new mt::SFMLVideoPlayback(*DataSource->Value, sf::Color(BufferColor.r, BufferColor.g, BufferColor.b, BufferColor.a));
    return videoplayback;
}

void mtSFMLVideoPlayback_Destroy(mtSFMLVideoPlayback* VideoPlayback)
{
    delete VideoPlayback->Value;
    delete VideoPlayback;
}

sfColor mtSFMLVideoPlayback_GetBufferColor(mtSFMLVideoPlayback* VideoPlayback)
{
    sf::Color color = VideoPlayback->Value->GetBufferColor();
    sfColor retval;
    retval.r = color.r;
    retval.g = color.g;
    retval.b = color.b;
    retval.a = color.a;
    return retval;
}

void mtSFMLVideoPlayback_SetBufferColor(mtSFMLVideoPlayback* VideoPlayback, sfColor BufferColor)
{
    VideoPlayback->Value->SetBufferColor(sf::Color(BufferColor.r, BufferColor.g, BufferColor.b, BufferColor.a));
}

void mtSFMLVideoPlayback_DrawRenderWindow(mtSFMLVideoPlayback* VideoPlayback, sfRenderWindow* RenderWindow, sfRenderStates* RenderStates)
{
    RenderWindow->This.draw(*VideoPlayback->Value, convertRenderStates(RenderStates));
}

void mtSFMLVideoPlayback_DrawRenderTexture(mtSFMLVideoPlayback* VideoPlayback, sfRenderTexture* RenderTexture, sfRenderStates* RenderStates)
{
    RenderTexture->This.draw(*VideoPlayback->Value, convertRenderStates(RenderStates));
}

void mtSFMLVideoPlayback_GetLastFrame(mtSFMLVideoPlayback* VideoPlayback, sfImage* Image)
{
    Image->This = VideoPlayback->Value->GetLastFrame();
}

unsigned int mtSFMLVideoPlayback_GetPlayedFrameCount(mtSFMLVideoPlayback* VideoPlayback)
{
    return VideoPlayback->Value->GetPlayedFrameCount();
}

#endif
