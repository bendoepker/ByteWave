set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_C_COMPILER clang)

cmake_minimum_required(VERSION 3.29)
project(ByteWave VERSION 0.1.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD 23)

# Avoid function argument expansion
cmake_policy(SET CMP0054 NEW)

# Generates a compile_commands.json in the build directory, used by clangd to resolve errors
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Extra tools for cmake in the root/cmake_tools directory
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake_tools)
include(gitignore)
ignore_directory(${CMAKE_BINARY_DIR})

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(STATUS "Using 64 Bit Architecture")
else()
    message(FATAL_ERROR "Only 64 Bit Architectures are Supported")
endif()

# Suppress windows warnings
if(WIN32)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif(WIN32)

# Build GLFW
if(LINUX)
    set(GLFW_BUILD_X11 ON)
    set(GLFW_BUILD_WAYLAND OFF)
endif()
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/ui/glfw)
set_target_properties(glfw PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

# ByteWave Source Files
set(bw_src
    # Entry Point
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp

    # Generic Audio Backend Wrapper
    ${CMAKE_CURRENT_SOURCE_DIR}/src/hostapi/hostapi.cpp

    # Utility Code
    ${CMAKE_CURRENT_SOURCE_DIR}/src/util/log.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/util/threads.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/util/serializer.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/util/types.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/util/conversions.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/util/schedule.c

    # Data Structure Variants
    ${CMAKE_CURRENT_SOURCE_DIR}/src/util/structs/dynamic-array.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/util/structs/circ-buf.c

    # ByteWave UI Files
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/style.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/ui-comps.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/application.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/render-frame.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/prompts.cpp

    # Library UI Files
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/ui/
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/ui/imgui.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/ui/imgui_draw.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/ui/imgui_demo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/ui/imgui_tables.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/ui/imgui_widgets.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/ui/glad/src/gl.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/ui/backend/imgui_impl_glfw.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/ui/backend/imgui_impl_opengl3.cpp
)

# ByteWave Include Directories
set(bw_inc
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src/hostapi
    ${CMAKE_CURRENT_SOURCE_DIR}/src/util/structs # TODO: Move these into the source root include directory
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/ui/
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/ui/glad/include
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/ui/glfw/include
)

# Detect Platform
if(WIN32)
    # TODO: Detect audio backends to use and define BW_<BACKEND> and find corresponding system libraries
    add_definitions(
        -DBW_ASIO
        # -DBW_WASAPI
        # -DBW_DSOUND
    )
    set(USE_ASIO TRUE)
    # set(USE_WASAPI TRUE)
    # set(USE_DSOUND TRUE)
endif(WIN32)

if(LINUX)
    add_definitions(
        -DBW_JACK
    )
    set(USE_JACK TRUE)
endif(LINUX)

# ByteWave Audio Backends
# ASIO
set(asio_src
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/ASIOSDK/common/asio.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/ASIOSDK/host/pc/asiolist.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/ASIOSDK/host/asiodrivers.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/hostapi/asio/bw-asio.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/hostapi/asio/bw-asio-il.cpp
)
set(asio_inc
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/ASIOSDK/common
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/ASIOSDK/host/pc
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/ASIOSDK/host
)
if(USE_ASIO STREQUAL TRUE)
    set(bw_src ${bw_src} ${asio_src})
    set(bw_inc ${bw_inc} ${asio_inc})
endif()

# WASAPI
set(wasapi_src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/hostapi/wasapi/bw-wasapi.c
)
set(wasapi_inc
    # TODO: Add any necessary include paths for wasapi here
)
if(USE_WASAPI STREQUAL TRUE)
    set(bw_src ${bw_src} ${wasapi_src})
    set(bw_inc ${bw_inc} ${wasapi_inc})
endif()

set(deps glfw)

# Direct Sound
set(dsound_src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/hostapi/dsound/bw-dsound.c
)
set(dsound_inc
    # TODO: Add any necessary include paths for direct sound here
)
if(USE_DSOUND STREQUAL TRUE)
    set(bw_src ${bw_src} ${dsound_src})
    set(bw_inc ${bw_inc} ${dsound_inc})
endif()

#Jack
set(jack_src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/hostapi/jack/bw-jack.cpp
)
set(jack_inc
    #TODO: Add any necessary jack include paths here
)
if(USE_JACK STREQUAL TRUE)
    set(bw_src ${bw_src} ${jack_src})
    set(bw_inc ${bw_inc} ${jack_inc})
    set(deps ${deps} jack)
endif()

# Debug Tools
if(CMAKE_BUILD_TYPE STREQUAL Debug)
    message("Build type is set to Debug")
    add_definitions(-DBW_LOG)
else()
    message("Build type is set to '${CMAKE_BUILD_TYPE}'")
    if(WIN32)
        set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} -mwindows)
        set(CMAKE_C_FLAGS ${CMAKE_CXX_FLAGS} -mwindows)
    endif()
endif()

# Create ByteWave Target
add_executable(ByteWave ${bw_src})

target_include_directories(ByteWave PUBLIC ${bw_inc})

target_compile_options(ByteWave PUBLIC
    # -Werror
    -Wimplicit-fallthrough
    -Wall
)

# Build ByteWave
target_link_libraries(ByteWave ${deps})
