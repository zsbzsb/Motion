
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
find_package(SFML 2 COMPONENTS system window graphics audio REQUIRED)
include_directories(${SFML_INCLUDE_DIR})

set(INCROOT ${CMAKE_SOURCE_DIR}/include/Motion)
set(SRCROOT ${CMAKE_SOURCE_DIR}/src/Motion)

# all source files
set(SRC
    ${INCROOT}/Export.h
    ${INCROOT}/priv/AudioPacket.hpp
    ${SRCROOT}/AudioPacket.cpp
    ${INCROOT}/DataSource.h
    ${INCROOT}/DataSource.hpp
    ${SRCROOT}/DataSource.cpp
    ${SRCROOT}/DataSourceStruct.h
    ${SRCROOT}/RenderTextureStruct.h
    ${SRCROOT}/RenderWindowStruct.h
    ${INCROOT}/priv/VideoPacket.hpp
    ${SRCROOT}/VideoPacket.cpp
    ${SRCROOT}/ViewStruct.h
    ${INCROOT}/Types.h
)

# define the motion target
motion_add_library(Motion
                  SOURCES ${SRC}
                  DEPENDS ${SFML_GRAPHICS_LIBRARY} ${SFML_WINDOW_LIBRARY} ${SFML_SYSTEM_LIBRARY} ${SFML_GRAPHICS_DEPENDENCIES} ${SFML_WINDOW_DEPENDENCIES} ${SFML_SYSTEM_DEPENDENCIES})


# include FFMPEG
motion_set_option(FFMPEG_INCLUDE_DIR "FFMPEG_INCLUDE_DIR Not Found" PATH "FFMPEG Include Directory")
motion_set_option(FFMPEG_LIBRARY_DIR "FFMPEG_LIBRARY_DIR Not Found" PATH "FFMPEG Library Directory")
motion_set_option(FFMPEG_AVUTIL_LIB "avutil.lib" STRING "avutil")
motion_set_option(FFMPEG_AVFORMAT_LIB "avformat.lib" STRING "avformat")
motion_set_option(FFMPEG_AVCODEC_LIB "avcodec.lib" STRING "avcodec")
motion_set_option(FFMPEG_SWSCALE_LIB "swscale.lib" STRING "swscale")
motion_set_option(FFMPEG_SWRESAMPLE_LIB "swresample.lib" STRING "swresample")
include_directories(${FFMPEG_INCLUDE_DIR})
target_link_libraries(Motion ${FFMPEG_LIBRARY_DIR}/${FFMPEG_AVUTIL_LIB} ${FFMPEG_LIBRARY_DIR}/${FFMPEG_AVFORMAT_LIB} ${FFMPEG_LIBRARY_DIR}/${FFMPEG_AVCODEC_LIB} ${FFMPEG_LIBRARY_DIR}/${FFMPEG_SWSCALE_LIB} ${FFMPEG_LIBRARY_DIR}/${FFMPEG_SWRESAMPLE_LIB})
