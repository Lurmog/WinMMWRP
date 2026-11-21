// WinMMWRP - Windows Multimedia Wrapper
// Shared includes and helpers. Targets Windows XP and later.

#pragma once

#ifndef WINVER
#define WINVER 0x0501
#endif
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <windows.h>
#include <mmsystem.h>

// winmm exports an ANSI "PlaySound" too; don't let the A/W macro rename it
#undef PlaySound

#include <cstddef>
#include <cstdint>

#if defined(_M_IX86) || defined(__i386__)
#define WRP_X86 1
#else
#define WRP_X86 0
#endif

// Driver message IDs (normally from mmddk.h, which MSVC only ships in the WDK)
#ifndef MODM_DATA
#define MODM_DATA       7
#define MODM_GETVOLUME  10
#define MODM_SETVOLUME  11
#define MODM_STRMDATA   14
#define MODM_GETPOS     17
#define MODM_PAUSE      18
#define MODM_RESTART    19
#define MODM_STOP       20
#define MODM_PROPERTIES 21
#endif

#ifndef MIDI_IO_COOKED
#define MIDI_IO_COOKED 0x00000002L
#endif

// Legacy MMSYSTEM task callback (not declared by every SDK)
typedef void(CALLBACK* WRP_TASKCALLBACK)(DWORD_PTR);

#define WRP_EXPORT extern "C"

namespace wrp {

// True when the running OS is at least major.minor (uses RtlGetVersion,
// so the result isn't affected by manifest-based version lies).
bool osAtLeast(DWORD major, DWORD minor);

void initMidiOut();
void shutdownMidiOut();

} // namespace wrp
