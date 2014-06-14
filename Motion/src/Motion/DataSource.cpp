#ifndef MOTION_DATASOURCE_CPP
#define MOTION_DATASOURCE_CPP

#include <Motion/DataSource.h>
#include <Motion/DataSource.hpp>

#include <Motion/DataSourceStruct.h>

#define MAX_AUDIO_SAMPLES 192000
#define PACKET_QUEUE_AMOUNT 5

namespace mt
{
    DataSource::DataSource() :
        m_videostreamid(-1),
        m_audiostreamid(-1),
        m_updateclock(),
        m_playingoffset(),
        m_filelength(sf::seconds(-1)),
        m_videosize(-1, -1),
        m_audiochannelcount(-1),
        m_playbackspeed(1),
        m_formatcontext(nullptr),
        m_videocontext(nullptr),
        m_audiocontext(nullptr),
        m_videocodec(nullptr),
        m_audiocodec(nullptr),
        m_videorawframe(nullptr),
        m_videorgbaframe(nullptr),
        m_audiorawbuffer(nullptr),
        m_videorawbuffer(nullptr),
        m_videorgbabuffer(nullptr),
        m_audiopcmbuffer(nullptr),
        m_videoswcontext(nullptr),
        m_audioswcontext(nullptr),
        m_state(State::Stopped),
        m_decodethread(nullptr),
        m_shouldthreadrun(false),
        m_playingtoeof(false),
        m_playbacklock(),
        m_videoplaybacks(),
        m_audioplaybacks()
    {
        av_register_all();
    }

    DataSource::~DataSource()
    {
        Cleanup();
        sf::Lock lock(m_playbacklock);
        while (m_videoplaybacks.size() > 0)
        {
            m_videoplaybacks.back()->m_datasource = nullptr;
            m_videoplaybacks.pop_back();
        }
        while (m_audioplaybacks.size() > 0)
        {
            m_audioplaybacks.back()->m_datasource = nullptr;
            m_audioplaybacks.back()->stop();
            m_audioplaybacks.pop_back();
        }
    }

    void DataSource::Cleanup()
    {
        StopDecodeThread();
        Stop();
        m_videostreamid = -1;
        m_audiostreamid = -1;
        m_playingoffset = sf::Time::Zero;
        m_filelength = sf::seconds(-1);
        m_videosize = { -1, -1 };
        m_audiochannelcount = -1;
        if (m_videocontext)
        {
            avcodec_close(m_videocontext);
            m_videocontext = nullptr;
        }
        m_videocodec = nullptr;
        if (m_audiocontext)
        {
            avcodec_close(m_audiocontext);
            m_audiocontext = nullptr;
        }
        m_audiocodec = nullptr;
        if (m_videorawframe) DestroyPictureFrame(m_videorawframe, m_videorawbuffer);
        if (m_videorgbaframe) DestroyPictureFrame(m_videorgbaframe, m_videorgbabuffer);
        if (m_audiorawbuffer)
        {
            avcodec_free_frame(&m_audiorawbuffer);
            m_audiorawbuffer = nullptr;
        }
        if (m_audiopcmbuffer)
        {
            av_free(m_audiopcmbuffer);
            m_audiopcmbuffer = nullptr;
        }
        if (m_videoswcontext)
        {
            sws_freeContext(m_videoswcontext);
            m_videoswcontext = nullptr;
        }
        if (m_audioswcontext)
        {
            swr_free(&m_audioswcontext);
            m_audioswcontext = nullptr;
        }
        if (m_formatcontext)
        {
            av_close_input_file(m_formatcontext);
            m_formatcontext = nullptr;
        }
    }

    bool DataSource::LoadFromFile(const std::string& Filename, bool EnableVideo, bool EnableAudio)
    {
        Cleanup();
        if (avformat_open_input(&m_formatcontext, Filename.c_str(), nullptr, nullptr) != 0)
        {
            return false;
        }
        if (avformat_find_stream_info(m_formatcontext, nullptr) < 0)
        {
            return false;
        }
        for (unsigned int i = 0; i < m_formatcontext->nb_streams; i++)
        {
            switch (m_formatcontext->streams[i]->codec->codec_type)
            {
                case AVMEDIA_TYPE_VIDEO:
                    if (m_videostreamid == -1 && EnableVideo) m_videostreamid = i;
                    break;
                case AVMEDIA_TYPE_AUDIO:
                    if (m_audiostreamid == -1 && EnableAudio) m_audiostreamid = i;
                    break;
                default:
                    break;
            }
        }
        if (HasVideo())
        {
            m_videocontext = m_formatcontext->streams[m_videostreamid]->codec;
            if (!m_videocontext) m_videostreamid = -1;
            else
            {
                m_videocodec = avcodec_find_decoder(m_videocontext->codec_id);
                if (!m_videocodec) m_videostreamid = -1;
                else
                {
                    if (avcodec_open2(m_videocontext, m_videocodec, nullptr) != 0) m_videostreamid = -1;
                    else
                    {
                        m_videosize = { m_videocontext->width, m_videocontext->height };
                        m_videorawframe = CreatePictureFrame(m_videocontext->pix_fmt, m_videosize.x, m_videosize.y, m_videorawbuffer);
                        m_videorgbaframe = CreatePictureFrame(PIX_FMT_RGBA, m_videosize.x, m_videosize.y, m_videorgbabuffer);
                        if (!m_videorawframe || !m_videorgbaframe) m_videostreamid = -1;
                        else
                        {
                            int swapmode = SWS_FAST_BILINEAR;
                            if (m_videosize.x * m_videosize.y <= 500000 && m_videosize.x % 8 != 0) swapmode |= SWS_ACCURATE_RND;
                            m_videoswcontext = sws_getCachedContext(nullptr, m_videosize.x, m_videosize.y, m_videocontext->pix_fmt, m_videosize.x, m_videosize.y, PIX_FMT_RGBA, swapmode, nullptr, nullptr, nullptr);
                        }
                    }
                }
            }
        }
        if (HasAudio())
        {
            m_audiocontext = m_formatcontext->streams[m_audiostreamid]->codec;
            if (!m_audiocontext) m_audiostreamid = -1;
            else
            {
                m_audiocodec = avcodec_find_decoder(m_audiocontext->codec_id);
                if (!m_audiocodec) m_audiostreamid = -1;
                else
                {
                    if (avcodec_open2(m_audiocontext, m_audiocodec, nullptr) != 0) m_audiostreamid = -1;
                    else
                    {
                        m_audiorawbuffer = avcodec_alloc_frame();
                        if (!m_audiorawbuffer) m_audiostreamid = -1;
                        else
                        {
                            if (av_samples_alloc(&m_audiopcmbuffer, nullptr, m_audiocontext->channels, av_samples_get_buffer_size(nullptr, m_audiocontext->channels, MAX_AUDIO_SAMPLES, AV_SAMPLE_FMT_S16, 0), AV_SAMPLE_FMT_S16, 0) < 0) m_audiostreamid = -1;
                            else
                            {
                                avcodec_get_frame_defaults(m_audiorawbuffer);
                                m_audioswcontext = swr_alloc();
                                uint64_t inchanlayout = m_audiocontext->channel_layout;
                                if (inchanlayout == 0) inchanlayout = av_get_default_channel_layout(m_audiocontext->channels);
                                uint64_t outchanlayout = inchanlayout;
                                if (outchanlayout != AV_CH_LAYOUT_MONO) outchanlayout = AV_CH_LAYOUT_STEREO;
                                av_opt_set_int(m_audioswcontext, "in_channel_layout", inchanlayout, 0);
                                av_opt_set_int(m_audioswcontext, "out_channel_layout", outchanlayout, 0);
                                av_opt_set_int(m_audioswcontext, "in_sample_rate", m_audiocontext->sample_rate, 0);
                                av_opt_set_int(m_audioswcontext, "out_sample_rate", m_audiocontext->sample_rate, 0);
                                av_opt_set_sample_fmt(m_audioswcontext, "in_sample_fmt", m_audiocontext->sample_fmt, 0);
                                av_opt_set_sample_fmt(m_audioswcontext, "out_sample_fmt", AV_SAMPLE_FMT_S16, 0);
                                swr_init(m_audioswcontext);
                                m_audiochannelcount = av_get_channel_layout_nb_channels(outchanlayout);
                            }
                        }
                    }
                }
            }
        }
        if (m_formatcontext->duration != AV_NOPTS_VALUE) m_filelength = sf::milliseconds(static_cast<int>(m_formatcontext->duration) / 1000);
        if (HasVideo() || HasAudio())
        {
            StartDecodeThread();
            return true;
        }
        else
        {
            Cleanup();
            return false;
        }
    }

    const bool DataSource::HasVideo()
    {
        return m_videostreamid != -1;
    }

    const bool DataSource::HasAudio()
    {
        return m_audiostreamid != -1;
    }

    const sf::Vector2i DataSource::GetVideoSize()
    {
        return m_videosize;
    }

    const State DataSource::GetState()
    {
        return m_state;
    }

    const sf::Time DataSource::GetVideoFrameTime()
    {
        if (!HasVideo()) return sf::Time::Zero;
        AVRational r1 = m_formatcontext->streams[m_videostreamid]->avg_frame_rate;
        AVRational r2 = m_formatcontext->streams[m_videostreamid]->r_frame_rate;
        if ((!r1.num || !r1.den) && (!r2.num || !r2.den))
        {
            return sf::seconds(1.0f / 29.97f);
        }
        else
        {
            if (r2.num && r1.den)
            {
                return sf::seconds(1.0f / ((float)r1.num / (float)r1.den));
            }
            else
            {
                return sf::seconds(1.0f / ((float)r2.num / (float)r2.den));
            }
        }
    }

    const int DataSource::GetAudioChannelCount()
    {
        return m_audiochannelcount;
    }

    const int DataSource::GetAudioSampleRate()
    {
        if (!HasAudio()) return -1;
        return m_audiocontext->sample_rate;
    }

    void DataSource::Play()
    {
        if ((HasVideo() || HasAudio()) && m_state != State::Playing)
        {
            if (m_state == State::Stopped)
            {
                m_playingtoeof = false;
            }
            m_updateclock.restart();
            NotifyStateChanged(State::Playing);
            m_state = State::Playing;
        }
    }

    void DataSource::Pause()
    {
        if (m_state == State::Playing)
        {
            NotifyStateChanged(State::Paused);
            m_state = State::Paused;
        }
    }

    void DataSource::Stop()
    {
        if (m_state != State::Stopped)
        {
            NotifyStateChanged(State::Stopped);
            m_state = State::Stopped;
            m_playingtoeof = false;
            SetPlayingOffset(sf::Time::Zero);
        }
    }

    const sf::Time DataSource::GetFileLength()
    {
        return m_filelength;
    }

    const sf::Time DataSource::GetPlayingOffset()
    {
        return m_playingoffset;
    }

    void DataSource::SetPlayingOffset(sf::Time PlayingOffset)
    {
        m_playingoffset = PlayingOffset;
        bool startplaying = m_state == State::Playing;
        if (m_state != State::Stopped)
        {
            NotifyStateChanged(State::Stopped);
            m_state = State::Stopped;
            m_playingtoeof = false;
        }
        if (HasVideo())
        {
            AVRational timebase = m_formatcontext->streams[m_videostreamid]->time_base;
            float ftb = (float)timebase.den / (float)timebase.num;
            int64_t pos = (int64_t)(PlayingOffset.asSeconds() * ftb);
            av_seek_frame(m_formatcontext, m_videostreamid, pos, AVSEEK_FLAG_ANY);
        }
        if (HasAudio())
        {
            AVRational timebase = m_formatcontext->streams[m_audiostreamid]->time_base;
            float ftb = (float)timebase.den / (float)timebase.num;
            int64_t pos = (int64_t)(PlayingOffset.asSeconds() * ftb);
            av_seek_frame(m_formatcontext, m_audiostreamid, pos, AVSEEK_FLAG_ANY);
        }
        if (startplaying) Play();
    }

    void DataSource::NotifyStateChanged(State NewState)
    {
        sf::Lock lock(m_playbacklock);
        for (auto& videoplayback : m_videoplaybacks)
        {
            videoplayback->StateChanged(m_state, NewState);
        }
        for (auto& audioplayback : m_videoplaybacks)
        {
            audioplayback->StateChanged(m_state, NewState);
        }
    }

    void DataSource::Update()
    {
        sf::Time deltatime = m_updateclock.restart() * m_playbackspeed;
        sf::Lock lock(m_playbacklock);
        m_playingoffset += deltatime;
        for (auto& videoplayback : m_videoplaybacks)
        {
            videoplayback->Update(deltatime);
        }
        if (m_playingoffset > m_filelength)
        {
            Stop();
        }
    }

    const float DataSource::GetPlaybackSpeed()
    {
        return m_playbackspeed;
    }

    void DataSource::SetPlaybackSpeed(float PlaybackSpeed)
    {
        m_playbackspeed = PlaybackSpeed;
        sf::Lock lock(m_playbacklock);
        for (auto& audioplayback : m_audioplaybacks)
        {
            audioplayback->setPitch(PlaybackSpeed);
        }
    }

    void DataSource::StartDecodeThread()
    {
        if (m_shouldthreadrun) return;
        m_shouldthreadrun = true;
        m_decodethread.reset(new std::thread(&DataSource::DecodeThreadRun, this));
    }

    void DataSource::StopDecodeThread()
    {
        if (!m_shouldthreadrun) return;
        m_shouldthreadrun = false;
        if (m_decodethread->joinable()) m_decodethread->join();
        m_decodethread.reset(nullptr);
    }

    void DataSource::DecodeThreadRun()
    {
        while (m_shouldthreadrun)
        {
            bool notfilled = false;
            {
                sf::Lock lock(m_playbacklock);
                if (HasVideo())
                {
                    for (auto& videoplayback : m_videoplaybacks)
                    {
                        sf::Lock playbacklock(videoplayback->m_protectionlock);
                        if (videoplayback->m_queuedvideopackets.size() < PACKET_QUEUE_AMOUNT)
                        {
                            notfilled = true;
                            break;
                        }
                    }
                }
                if (!notfilled && HasAudio())
                {
                    for (auto& audioplayback : m_audioplaybacks)
                    {
                        sf::Lock playbacklock(audioplayback->m_protectionlock);
                        if (audioplayback->m_queuedaudiopackets.size() < PACKET_QUEUE_AMOUNT)
                        {
                            notfilled = true;
                            break;
                        }
                    }
                }
            }
            while (!m_playingtoeof && notfilled && m_shouldthreadrun)
            {
                bool validpacket = false;
                while (!validpacket && m_shouldthreadrun)
                {
                    AVPacket* packet;
                    packet = (AVPacket*)av_malloc(sizeof(*packet));
                    av_init_packet(packet);
                    if (av_read_frame(m_formatcontext, packet) == 0)
                    {
                        if (packet->stream_index == m_videostreamid)
                        {
                            int decoderesult = 0;
                            if (avcodec_decode_video2(m_videocontext, m_videorawframe, &decoderesult, packet) >= 0)
                            {
                                if (decoderesult)
                                {
                                    sws_scale(m_videoswcontext, m_videorawframe->data, m_videorawframe->linesize, 0, m_videocontext->height, m_videorgbaframe->data, m_videorgbaframe->linesize);
                                    validpacket = true;
                                    priv::VideoPacketPtr packet(std::make_shared<priv::VideoPacket>(m_videorgbaframe->data[0], m_videosize.x, m_videosize.y));
                                    notfilled = false;
                                    sf::Lock lock(m_playbacklock);
                                    for (auto& videoplayback : m_videoplaybacks)
                                    {
                                        sf::Lock playbacklock(videoplayback->m_protectionlock);
                                        videoplayback->m_queuedvideopackets.push(packet);
                                        if (!notfilled && videoplayback->m_queuedvideopackets.size() < PACKET_QUEUE_AMOUNT)
                                        {
                                            notfilled = true;
                                        }
                                    }
                                    if (!notfilled && HasAudio())
                                    {
                                        for (auto& audioplayback : m_audioplaybacks)
                                        {
                                            sf::Lock playbacklock(audioplayback->m_protectionlock);
                                            if (audioplayback->m_queuedaudiopackets.size() < PACKET_QUEUE_AMOUNT)
                                            {
                                                notfilled = true;
                                                break;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        else if (packet->stream_index == m_audiostreamid)
                        {
                            int decoderesult = 0;
                            if (avcodec_decode_audio4(m_audiocontext, m_audiorawbuffer, &decoderesult, packet) > 0)
                            {
                                if (decoderesult)
                                {
                                    int convertlength = swr_convert(m_audioswcontext, &m_audiopcmbuffer, m_audiorawbuffer->nb_samples, (const uint8_t**)m_audiorawbuffer->extended_data, m_audiorawbuffer->nb_samples);
                                    if (convertlength > 0)
                                    {
                                        validpacket = true;
                                        priv::AudioPacketPtr packet(std::make_shared<priv::AudioPacket>(m_audiopcmbuffer, convertlength, m_audiochannelcount));
                                        notfilled = false;
                                        sf::Lock lock(m_playbacklock);
                                        for (auto& audioplayback : m_audioplaybacks)
                                        {
                                            sf::Lock playbacklock(audioplayback->m_protectionlock);
                                            audioplayback->m_queuedaudiopackets.push(packet);
                                            if (!notfilled && audioplayback->m_queuedaudiopackets.size() < PACKET_QUEUE_AMOUNT)
                                            {
                                                notfilled = true;
                                            }
                                        }
                                        if (!notfilled && HasVideo())
                                        {
                                            for (auto& videoplayback : m_videoplaybacks)
                                            {
                                                sf::Lock playbacklock(videoplayback->m_protectionlock);
                                                if (videoplayback->m_queuedvideopackets.size() < PACKET_QUEUE_AMOUNT)
                                                {
                                                    notfilled = true;
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        av_free_packet(packet);
                        av_free(packet);
                    }
                    else
                    {
                        m_playingtoeof = true;
                        validpacket = true;
                        av_free_packet(packet);
                        av_free(packet);
                    }
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
    }

    AVFrame* DataSource::CreatePictureFrame(enum PixelFormat SelectedPixelFormat, int Width, int Height, uint8_t*& PictureBuffer)
    {
        AVFrame *picture;
        picture = avcodec_alloc_frame();
        if (!picture) return nullptr;
        int size = avpicture_get_size(SelectedPixelFormat, Width, Height);
        PictureBuffer = (uint8_t*)av_malloc(size);
        if (!PictureBuffer)
        {
            avcodec_free_frame(&picture);
            return nullptr;
        }
        avpicture_fill((AVPicture*)picture, PictureBuffer, SelectedPixelFormat, Width, Height);
        return picture;
    }

    void DataSource::DestroyPictureFrame(AVFrame*& PictureFrame, uint8_t*& PictureBuffer)
    {
        av_free(PictureBuffer);
        avcodec_free_frame(&PictureFrame);
        PictureBuffer = nullptr;
        PictureFrame = nullptr;
    }
}

mtDataSource* mtDataSource_Create(void)
{
    mtDataSource* datasource = new mtDataSource();
    datasource->Value = new mt::DataSource();
    return datasource;
}

void mtDataSource_Destroy(mtDataSource* DataSource)
{
    delete DataSource->Value;
    delete DataSource;
}

sfBool mtDataSource_LoadFromFile(mtDataSource* DataSource, const char* Filename, sfBool EnableVideo, sfBool EnableAudio)
{
    return DataSource->Value->LoadFromFile(Filename, EnableVideo == sfTrue, EnableAudio == sfTrue);
}

void mtDataSource_Play(mtDataSource* DataSource)
{
    DataSource->Value->Play();
}

void mtDataSource_Pause(mtDataSource* DataSource)
{
    DataSource->Value->Pause();
}

void mtDataSource_Stop(mtDataSource* DataSource)
{
    DataSource->Value->Stop();
}

sfBool mtDataSource_HasVideo(mtDataSource* DataSource)
{
    return DataSource->Value->HasVideo();
}

sfBool mtDataSource_HasAudio(mtDataSource* DataSource)
{
    return DataSource->Value->HasAudio();
}

#endif
