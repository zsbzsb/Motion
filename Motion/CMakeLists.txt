cmake_minimum_required(VERSION 3.0.2)

# define a macro that helps defining an option
macro(motion_set_option var default type docstring)
    if(NOT DEFINED ${var})
        set(${var} ${default})
    endif()
    set(${var} ${${var}} CACHE ${type} ${docstring} FORCE)
endmacro()

# set a default build type if none was provided
# this has to be done before the project() instruction!
motion_set_option(CMAKE_BUILD_TYPE Release STRING "Choose the type of build (Debug or Release)")

# project name
project(Motion)

# include the configuration file
include(${CMAKE_SOURCE_DIR}/cmake/Config.cmake)

# setup version numbers
set(VERSION_MAJOR 1)
set(VERSION_MINOR 0)
set(VERSION_PATCH 0)

# add the Motion header path
include_directories(${CMAKE_SOURCE_DIR}/include)

# add an option for choosing the build type (shared or static)
motion_set_option(BUILD_SHARED_LIBS TRUE BOOL "TRUE to build Motion as a shared library, FALSE to build it as a static library")

# add an option for linking to sfml either statically or dynamically
motion_set_option(MOTION_LINK_SFML_STATICALLY FALSE BOOL "TRUE to link to a static version of SFML, FALSE to link dynamically")

# disable the rpath stuff
set(CMAKE_SKIP_BUILD_RPATH TRUE)

# define an option for choosing between static and dynamic C runtime (Windows only)
if(MOTION_OS_WINDOWS)
    set(STATIC_STD_LIBS FALSE CACHE BOOL "TRUE to statically link to the standard libraries, FALSE to use them as DLLs")

    # for VC++, we can apply it globally by modifying the compiler flags
    if(MOTION_COMPILER_MSVC AND STATIC_STD_LIBS)
        foreach(flag
                CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
                CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
            if(${flag} MATCHES "/MD")
                string(REGEX REPLACE "/MD" "/MT" ${flag} "${${flag}}")
            endif()
        endforeach()
    endif()
endif()

if(NOT MSVC)
    set(CMAKE_CXX_FLAGS -std=c++11)
endif()

# add the subdirectories
add_subdirectory(src/Motion)

# setup the install rules
install(DIRECTORY include
        DESTINATION .
        COMPONENT devel
        PATTERN ".svn" EXCLUDE)
