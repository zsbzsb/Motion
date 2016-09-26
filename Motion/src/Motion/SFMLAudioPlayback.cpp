#ifndef MOTION_SFMLAUDIOPLAYBACK_CPP
#define MOTION_SFMLAUDIOPLAYBACK_CPP

#include <Motion/AudioPlayback.h>
#include <Motion/SFMLAudioPlayback.hpp>

namespace mt
{
    SFMLAudioPlayback::SFMLAudioPlayback(DataSource& DataSource, sf::Time OffsetCorrection) :
        AudioPlaybackBase(DataSource, OffsetCorrection)
    { }

    bool SFMLAudioPlayback::onGetData(Chunk& data)
    {
        const int16_t* samples = nullptr;
        std::size_t samplecount = 0;

        if (GetNextBuffer(samples, samplecount))
        {
            data.samples = samples;
            data.sampleCount = samplecount;

            return true;
        }
        else
            return false;
    }

    void SFMLAudioPlayback::onSeek(sf::Time timeOffset)
    {
        // nothing to do
    }

    const float SFMLAudioPlayback::GetVolume()
    {
        return getVolume();
    }

    void SFMLAudioPlayback::SetVolume(float Volume)
    {
        setVolume(Volume);
    }

    void SFMLAudioPlayback::SetupStream(unsigned int ChannelCount, int SampleRate)
    {
        initialize(ChannelCount, SampleRate);
    }

    void SFMLAudioPlayback::SetPlaybackSpeed(float PlaybackSpeed)
    {
        setPitch(PlaybackSpeed);
    }

    void SFMLAudioPlayback::StartStream()
    {
        play();
    }

    void SFMLAudioPlayback::PauseStream()
    {
        pause();
    }

    void SFMLAudioPlayback::StopStream()
    {
        stop();
    }
}

mtSFMLAudioPlayback* mtSFMLAudioPlayback_Create(mtDataSource* DataSource, sfTime OffsetCorrection)
{
    mtSFMLAudioPlayback* audioplayback = new mtSFMLAudioPlayback();

    audioplayback->Value = new mt::SFMLAudioPlayback(*DataSource->Value, sf::microseconds(OffsetCorrection.microseconds));

    return audioplayback;
}

void mtSFMLAudioPlayback_Destroy(mtSFMLAudioPlayback* AudioPlayback)
{
    delete AudioPlayback->Value;
    delete AudioPlayback;
}

float mtSFMLAudioPlayback_GetVolume(mtSFMLAudioPlayback* AudioPlayback)
{
    return AudioPlayback->Value->GetVolume();
}

void mtSFMLAudioPlayback_SetVolume(mtSFMLAudioPlayback* AudioPlayback, float Volume)
{
    AudioPlayback->Value->SetVolume(Volume);
}

sfTime mtSFMLAudioPlayback_GetOffsetCorrection(mtSFMLAudioPlayback* AudioPlayback)
{
    sfTime retval;

    retval.microseconds = AudioPlayback->Value->GetOffsetCorrection().asMicroseconds();

    return retval;
}

void mtSFMLAudioPlayback_SetOffsetCorrection(mtSFMLAudioPlayback* AudioPlayback, sfTime OffsetCorrection)
{
    AudioPlayback->Value->SetOffsetCorrection(sf::microseconds(OffsetCorrection.microseconds));
}

#endif
