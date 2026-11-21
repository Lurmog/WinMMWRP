cmake_minimum_required(VERSION 3.15)
project(WinMMWRP LANGUAGES CXX)

if(NOT WIN32)
    message(FATAL_ERROR "WinMMWRP targets Windows only. Use a MinGW-w64 toolchain file to cross-compile.")
endif()

option(WRP_PURE_WRAPPER     "Plain pass-through, no KDMAPI device"           OFF)
option(WRP_CODPATCH         "Cap mixer/waveIn device count at 15"             OFF)
option(WRP_FAST_SHORTMSG    "Bypass midiOutShortMsg overhead on Windows 8+"   ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(winmm SHARED
    src/dllmain.cpp
    src/kdmapi.cpp
    src/midiout.cpp
    src/winmm.cpp
)

set_target_properties(winmm PROPERTIES PREFIX "" OUTPUT_NAME "winmm")

if(CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(WRP_DEF "${CMAKE_CURRENT_SOURCE_DIR}/def/winmm-x86.def")
    set(WRP_OS_VER 5.1)   # Windows XP
else()
    set(WRP_DEF "${CMAKE_CURRENT_SOURCE_DIR}/def/winmm.def")
    set(WRP_OS_VER 5.2)   # Windows XP x64 / Server 2003
endif()
target_sources(winmm PRIVATE ${WRP_DEF})

# Windows XP is the minimum target
target_compile_definitions(winmm PRIVATE
    WINVER=0x0501 _WIN32_WINNT=0x0501 UNICODE _UNICODE
    $<$<BOOL:${WRP_PURE_WRAPPER}>:WRP_PURE_WRAPPER>
    $<$<BOOL:${WRP_CODPATCH}>:WRP_CODPATCH>
    $<$<NOT:$<BOOL:${WRP_FAST_SHORTMSG}>>:WRP_NO_FAST_SHORTMSG>
)

if(MINGW)
    string(REPLACE "." ";" _ver ${WRP_OS_VER})
    list(GET _ver 0 _major)
    list(GET _ver 1 _minor)

    target_compile_options(winmm PRIVATE -Wall -Wextra -fno-exceptions -fno-rtti)
    target_link_options(winmm PRIVATE
        -static -static-libgcc -static-libstdc++
        -Wl,--enable-stdcall-fixup
        -Wl,--major-os-version,${_major} -Wl,--minor-os-version,${_minor}
        -Wl,--major-subsystem-version,${_major} -Wl,--minor-subsystem-version,${_minor}
        $<$<CONFIG:Release,MinSizeRel>:-s>
    )
elseif(MSVC)
    # Requires the v141_xp toolset (or newer with XP support) for XP builds
    target_compile_options(winmm PRIVATE /W4 /GR- /EHs-c-)
    set_property(TARGET winmm PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    target_link_options(winmm PRIVATE /SUBSYSTEM:WINDOWS,${WRP_OS_VER})
endif()
