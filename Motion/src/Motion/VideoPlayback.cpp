#ifndef MOTION_VIDEOPLAYBACK_CPP
#define MOTION_VIDEOPLAYBACK_CPP

#include <Motion/VideoPlayback.hpp>

namespace mt
{
    VideoPlayback::VideoPlayback(DataSource& DataSource, sf::Color BufferColor) :
        m_videotexture(),
        m_videosprite(),
        m_buffercolor(BufferColor),
        m_datasource(&DataSource),
        m_queuedvideopackets(),
        m_elapsedtime(),
        m_frametime(DataSource.GetVideoFrameTime()),
        m_framejump(0)
    {
        if (m_datasource->HasVideo())
        {
            m_datasource->m_videoplaybacks.push_back(this);
            m_videotexture.create(m_datasource->GetVideoSize().x, m_datasource->GetVideoSize().y);
            m_videosprite.setTexture(m_videotexture);
            SetInitialBuffer();
        }
    }

    VideoPlayback::~VideoPlayback()
    {
        if (m_datasource)
        {
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
        if (m_datasource)
        {
            if (m_datasource->GetState() == DataSource::Playing)
            {
                m_elapsedtime += DeltaTime;
                int jumpcount = static_cast<int>(std::floor(m_elapsedtime.asMicroseconds() / m_frametime.asMicroseconds()));
                m_elapsedtime -= m_frametime * static_cast<float>(jumpcount);
                m_framejump += jumpcount;
            }
            while (m_queuedvideopackets.size() > 0)
            {
                if (m_framejump > 1)
                {
                    m_framejump -= 1;
                    m_queuedvideopackets.pop();
                }
                else if (m_framejump == 1)
                {
                    m_framejump -= 1;
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
}

#endif
