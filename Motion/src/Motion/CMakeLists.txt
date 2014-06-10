
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
    ${SRCROOT}/RenderTextureStruct.h
    ${SRCROOT}/RenderWindowStruct.h
    ${INCROOT}/Types.h
)

# define the motion target
motion_add_library(Motion
                  SOURCES ${SRC}
                  DEPENDS ${SFML_GRAPHICS_LIBRARY} ${SFML_WINDOW_LIBRARY} ${SFML_SYSTEM_LIBRARY} ${SFML_GRAPHICS_DEPENDENCIES} ${SFML_WINDOW_DEPENDENCIES} ${SFML_SYSTEM_DEPENDENCIES})
