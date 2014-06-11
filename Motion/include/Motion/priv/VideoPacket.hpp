#ifndef MOTION_VIDEOPACKET_HPP
#define MOTION_VIDEOPACKET_HPP

#include <cstdint>
#include <cstddef>
#include <memory>

extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
#include <libavutil/opt.h>
}

namespace mt
{
    namespace priv
    {
        class VideoPacket
        {
        private:
            uint8_t* m_rgbabuffer;
        public:
            VideoPacket(uint8_t* RGBABufferSource, int Width, int Height);
            ~VideoPacket();
        };

        typedef std::shared_ptr<mt::priv::VideoPacket> VideoPacketPtr;
    }
}

#endif
