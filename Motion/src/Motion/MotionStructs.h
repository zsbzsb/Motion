#ifndef MOTION_MOTIONSTRUCTS_H
#define MOTION_MOTIONSTRUCTS_H

#include <Motion/DataSource.hpp>
#include <Motion/AudioPlayback.hpp>
#include <Motion/VideoPlayback.hpp>

struct mtDataSource
{
    mt::DataSource* Value;
};

struct mtAudioPlayback
{
    mt::AudioPlayback* Value;
};

struct mtVideoPlayback
{
    mt::VideoPlayback* Value;
};

#endif
