#ifndef MOTION_MOTIONSTRUCTS_H
#define MOTION_MOTIONSTRUCTS_H

#include <Motion/DataSource.hpp>
#include <Motion/AudioPlaybackBase.hpp>
#include <Motion/VideoPlaybackBase.hpp>
#include <Motion/SFMLAudioPlayback.hpp>
#include <Motion/SFMLVideoPlayback.hpp>

class DummyVideoPlayback : public mt::VideoPlaybackBase
{
private:
    mtCreateTextureCB m_createcb;
    mtUpdateTextureCB m_updatecb;
    mtClearTextureCB m_clearcb;

public:
    DummyVideoPlayback(mt::DataSource& DataSource, mtCreateTextureCB CreateCB, mtUpdateTextureCB UpdateCB, mtClearTextureCB ClearCB);

protected:
    void CreateTexture(int Width, int Height);
    void UpdateTexture(const uint8_t* RGBABuffer);
    void ClearTexture();
};

class DummyAudioPlayback : public mt::AudioPlaybackBase
{
private:
    mtSetupStreamCB m_setupcb;
    mtSetPlaybackSpeedCB m_setspeedcb;
    mtChangeStateCB m_startcb;
    mtChangeStateCB m_pausecb;
    mtChangeStateCB m_stopcb;

public:
    DummyAudioPlayback(mt::DataSource& DataSource, sf::Time OffsetCorrection, mtSetupStreamCB SetupCB, mtSetPlaybackSpeedCB SetSpeedCB, mtChangeStateCB StartCB, mtChangeStateCB PauseCB, mtChangeStateCB StopCB);
    bool OnGetNextBuffer(const int16_t*& Samples, std::size_t& SampleCount);

protected:
    void SetupStream(unsigned int ChannelCount, int SampleRate);
    void SetPlaybackSpeed(float PlaybackSpeed);
    void StartStream();
    void PauseStream();
    void StopStream();
};

struct mtDataSource
{
    mt::DataSource* Value;
};

struct mtAudioPlaybackBase
{
    DummyAudioPlayback* Value;
};

struct mtVideoPlaybackBase
{
    DummyVideoPlayback* Value;
};

struct mtSFMLAudioPlayback
{
    mt::SFMLAudioPlayback* Value;
};

struct mtSFMLVideoPlayback
{
    mt::SFMLVideoPlayback* Value;
};

#endif
