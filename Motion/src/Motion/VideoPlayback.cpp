#ifndef MOTION_VIDEOPLAYBACK_CPP
#define MOTION_VIDEOPLAYBACK_CPP

#include <Motion/VideoPlayback.h>
#include <Motion/VideoPlayback.hpp>

namespace mt
{
    VideoPlayback::VideoPlayback(DataSource& DataSource, sf::Color BufferColor) :
        m_videotexture(),
        m_videosprite(),
        m_buffercolor(BufferColor),
        m_datasource(&DataSource),
        m_protectionlock(),
        m_queuedvideopackets(),
        m_elapsedtime(),
        m_frametime(sf::Time::Zero),
        m_framejump(0),
        m_playedframecount(0)
    {
        SourceReloaded();
        sf::Lock lock(m_datasource->m_playbacklock);
        m_datasource->m_videoplaybacks.push_back(this);
    }

    VideoPlayback::~VideoPlayback()
    {
        if (m_datasource)
        {
            sf::Lock lock(m_datasource->m_playbacklock);
            for (auto& videoplayback : m_datasource->m_videoplaybacks)
            {
                if (videoplayback == this)
                {
                    std::swap(videoplayback, m_datasource->m_videoplaybacks.back());
                    m_datasource->m_videoplaybacks.pop_back();
                    break;
                }
            }
            m_datasource = nullptr;
        }
    }

    void VideoPlayback::SetInitialBuffer()
    {
        sf::Image initialbuffer;
        initialbuffer.create(m_videotexture.getSize().x, m_videotexture.getSize().y, m_buffercolor);
        m_videotexture.update(initialbuffer);
    }

    void VideoPlayback::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform *= getTransform();
        target.draw(m_videosprite, states);
    }

    void VideoPlayback::Update(sf::Time DeltaTime)
    {
        if (m_datasource && m_datasource->HasVideo())
        {
            if (m_datasource->GetState() == State::Playing)
            {
                m_elapsedtime += DeltaTime;
                int jumpcount = static_cast<int>(std::floor(m_elapsedtime.asMicroseconds() / m_frametime.asMicroseconds()));
                m_elapsedtime -= m_frametime * static_cast<float>(jumpcount);
                m_framejump += jumpcount;
            }
            sf::Lock lock(m_protectionlock);
            while (m_queuedvideopackets.size() > 0)
            {
                if (m_framejump > 1)
                {
                    m_framejump -= 1;
                    m_playedframecount++;
                    m_queuedvideopackets.pop();
                }
                else if (m_framejump == 1)
                {
                    m_framejump -= 1;
                    m_playedframecount++;
                    m_videotexture.update(m_queuedvideopackets.front()->GetRGBABuffer());
                    m_queuedvideopackets.pop();
                    break;
                }
                else
                {
                    break;
                }
            }
        }
    }

    void VideoPlayback::SourceReloaded()
    {
        if (m_datasource->HasVideo())
        {
            m_frametime = m_datasource->GetVideoFrameTime();
            m_videotexture.create(m_datasource->GetVideoSize().x, m_datasource->GetVideoSize().y);
            m_videosprite.setTexture(m_videotexture, true);
            SetInitialBuffer();
        }
    }

    void VideoPlayback::StateChanged(State PreviousState, State NewState)
    {
        if (NewState == State::Playing && PreviousState == State::Stopped)
        {
            m_framejump = 1;
            m_playedframecount = 0;
        }
        else if (NewState == State::Stopped)
        {
            m_framejump = 0;
            m_playedframecount = 0;
            if (m_datasource->HasVideo()) SetInitialBuffer();
            sf::Lock lock(m_protectionlock);
            while (m_queuedvideopackets.size() > 0)
            {
                m_queuedvideopackets.pop();
            }
        }
    }

    sf::Color VideoPlayback::GetBufferColor() const
    {
        return m_buffercolor;
    }

    void VideoPlayback::SetBufferColor(sf::Color BufferColor)
    {
        m_buffercolor = BufferColor;
        if (m_datasource && m_datasource->GetState() == State::Stopped) SetInitialBuffer();
    }

    sf::Image VideoPlayback::GetLastFrame() const
    {
        return m_videotexture.copyToImage();
    }

    unsigned int VideoPlayback::GetPlayedFrameCount() const
    {
        return m_playedframecount;
    }
}

mtVideoPlayback* mtVideoPlayback_Create(mtDataSource* DataSource, sfColor BufferColor)
{
    mtVideoPlayback* videoplayback = new mtVideoPlayback();
    videoplayback->Value = new mt::VideoPlayback(*DataSource->Value, sf::Color(BufferColor.r, BufferColor.g, BufferColor.b, BufferColor.a));
    return videoplayback;
}

void mtVideoPlayback_Destroy(mtVideoPlayback* VideoPlayback)
{
    delete VideoPlayback->Value;
    delete VideoPlayback;
}

sfColor mtVideoPlayback_GetBufferColor(mtVideoPlayback* VideoPlayback)
{
    sf::Color color = VideoPlayback->Value->GetBufferColor();
    sfColor retval;
    retval.r = color.r;
    retval.g = color.g;
    retval.b = color.b;
    retval.a = color.a;
    return retval;
}

void mtVideoPlayback_SetBufferColor(mtVideoPlayback* VideoPlayback, sfColor BufferColor)
{
    VideoPlayback->Value->SetBufferColor(sf::Color(BufferColor.r, BufferColor.g, BufferColor.b, BufferColor.a));
}

void mtVideoPlayback_DrawRenderWindow(mtVideoPlayback* VideoPlayback, sfRenderWindow* RenderWindow, sfRenderStates* RenderStates)
{
    RenderWindow->This.draw(*VideoPlayback->Value, convertRenderStates(RenderStates));
}

void mtVideoPlayback_DrawRenderTexture(mtVideoPlayback* VideoPlayback, sfRenderTexture* RenderTexture, sfRenderStates* RenderStates)
{
    RenderTexture->This.draw(*VideoPlayback->Value, convertRenderStates(RenderStates));
}

void mtVideoPlayback_GetLastFrame(mtVideoPlayback* VideoPlayback, sfImage* Image)
{
    Image->This = VideoPlayback->Value->GetLastFrame();
}

unsigned int mtVideoPlayback_GetPlayedFrameCount(mtVideoPlayback* VideoPlayback)
{
    return VideoPlayback->Value->GetPlayedFrameCount();
}

#endif
