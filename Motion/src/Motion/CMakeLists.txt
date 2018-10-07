# include the Motion specific macros
include(${CMAKE_SOURCE_DIR}/cmake/Macros.cmake)

# add the Motion sources path
include_directories(${CMAKE_SOURCE_DIR}/src)

# set the output directory for Motion libraries
set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/lib")

# define the export symbol
add_definitions(-DMOTION_EXPORTS)

# disable stupid warnings with Visual C++
if(MOTION_COMPILER_MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

# find SFML libraries (C++)
if(MOTION_OS_WINDOWS)
    if (MOTION_LINK_SFML_STATICALLY)
        set(SFML_STATIC_LIBRARIES TRUE)
        add_definitions(-DSFML_STATIC)
    endif()
endif()

find_package(SFML 2.5 COMPONENTS system window graphics audio REQUIRED)
include_directories(${SFML_INCLUDE_DIR})

#find FFMPEG
motion_set_option(FFMPEG_ROOT "" PATH "FFMPEG Root Directory")

find_path(AVUTIL_INCLUDE_DIR libavutil/avutil.h PATHS ${FFMPEG_ROOT}/include)
find_library(AVUTIL_LIBRARY avutil PATHS ${FFMPEG_ROOT}/lib)

find_path(AVFORMAT_INCLUDE_DIR libavformat/avformat.h PATHS ${FFMPEG_ROOT}/include)
find_library(AVFORMAT_LIBRARY avformat PATHS ${FFMPEG_ROOT}/lib)

find_path(AVCODEC_INCLUDE_DIR libavcodec/avcodec.h PATHS ${FFMPEG_ROOT}/include)
find_library(AVCODEC_LIBRARY avcodec PATHS ${FFMPEG_ROOT}/lib)

find_path(SWSCALE_INCLUDE_DIR libswscale/swscale.h PATHS ${FFMPEG_ROOT}/include)
find_library(SWSCALE_LIBRARY swscale PATHS ${FFMPEG_ROOT}/lib)

find_path(SWRESAMPLE_INCLUDE_DIR libswresample/swresample.h PATHS ${FFMPEG_ROOT}/include)
find_library(SWRESAMPLE_LIBRARY swresample PATHS ${FFMPEG_ROOT}/lib)

include_directories(${AVUTIL_INCLUDE_DIR} ${AVFORMAT_INCLUDE_DIR} ${AVCODEC_INCLUDE_DIR} ${SWSCALE_INCLUDE_DIR} ${SWRESAMPLE_INCLUDE_DIR})

set(INCROOT ${CMAKE_SOURCE_DIR}/include/Motion)
set(SRCROOT ${CMAKE_SOURCE_DIR}/src/Motion)

# all source files
set(SRC
    ${INCROOT}/Export.h
    ${INCROOT}/priv/AudioPacket.hpp
    ${SRCROOT}/AudioPacket.cpp
    ${INCROOT}/AudioPlayback.h
    ${INCROOT}/AudioPlaybackBase.hpp
    ${SRCROOT}/AudioPlaybackBase.cpp
    ${INCROOT}/DataSource.h
    ${INCROOT}/DataSource.hpp
    ${SRCROOT}/DataSource.cpp
    ${INCROOT}/Motion.hpp
    ${SRCROOT}/MotionStructs.h
    ${INCROOT}/State.hpp
    ${INCROOT}/SFMLAudioPlayback.hpp
    ${SRCROOT}/SFMLAudioPlayback.cpp
    ${INCROOT}/SFMLVideoPlayback.hpp
    ${SRCROOT}/SFMLVideoPlayback.cpp
    ${INCROOT}/priv/VideoPacket.hpp
    ${SRCROOT}/VideoPacket.cpp
    ${INCROOT}/VideoPlayback.h
    ${INCROOT}/VideoPlaybackBase.hpp
    ${SRCROOT}/VideoPlaybackBase.cpp
    ${INCROOT}/Types.h
    ${SRCROOT}/CSFMLStructs.h
)

# define the motion target
motion_add_library(Motion
                  SOURCES ${SRC}
                  DEPENDS sfml-graphics sfml-window sfml-system sfml-audio ${AVUTIL_LIBRARY} ${AVFORMAT_LIBRARY} ${AVCODEC_LIBRARY} ${SWSCALE_LIBRARY} ${SWRESAMPLE_LIBRARY})

if(MOTION_OS_WINDOWS)
    target_link_libraries(Motion legacy_stdio_definitions.lib)
endif()
