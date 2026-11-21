// WinMMWRP - MIDI out overrides.
//
// * Exposes OmniMIDI's KDMAPI as MIDI out device 0 (real devices are
//   shifted up by one). Disabled when built with WRP_PURE_WRAPPER.
// * Bypasses the slow midiOutShortMsg path on Windows 8+ by calling the
//   driver's modMessage directly.

#include "kdmapi.hpp"
#include "winmm.hpp"

#include <type_traits>

namespace {

#ifdef WRP_PURE_WRAPPER
constexpr bool kKdmapi = false;
#else
constexpr bool kKdmapi = true;
#endif

constexpr UINT kKdmapiId = 0;
constexpr UINT kDevOffset = kKdmapi ? 1 : 0;
const HMIDIOUT kKdmapiOut = reinterpret_cast<HMIDIOUT>(static_cast<UINT_PTR>(0x10001));
const HMIDISTRM kKdmapiStrm = reinterpret_cast<HMIDISTRM>(kKdmapiOut);
const wchar_t kKdmapiName[] = L"KDMAPI";

DWORD_PTR g_kdmapiUser = 0;
bool g_kdmapiOpen = false;

// Internal winmm handle layout on Windows 8 and later.
struct DeviceVtbl {
#if WRP_X86
    char gap[0x3C];
#else
    char gap[0x50];
#endif
    MMRESULT(WINAPI* modMessage)(UINT, UINT, DWORD_PTR, DWORD_PTR, DWORD_PTR);
};

struct DeviceHandle {
    DeviceVtbl* vtbl;
    UINT id;
#if WRP_X86
    char gap[4];
#endif
    DWORD_PTR user;
};

bool g_fastShortMsg = false;

bool isKdmapi(HMIDIOUT h) { return kKdmapi && g_kdmapiOpen && h == kKdmapiOut; }
bool isKdmapi(HMIDISTRM h) { return kKdmapi && g_kdmapiOpen && h == kKdmapiStrm; }

bool isMapper(UINT_PTR id) {
    return id == MIDI_MAPPER || id == static_cast<UINT>(MIDI_MAPPER);
}

// Device IDs above the device count are handles (allowed by GetDevCaps)
bool isDeviceId(UINT_PTR id) {
    return id < static_cast<UINT_PTR>(real::midiOutGetNumDevs ? real::midiOutGetNumDevs() : 0) + kDevOffset;
}

UINT_PTR toRealId(UINT_PTR id) {
    if (!kKdmapi || isMapper(id) || !isDeviceId(id))
        return id;
    return id - kDevOffset;
}

template <typename Caps>
MMRESULT fillKdmapiCaps(Caps* caps, UINT size) {
    if (!caps || size == 0)
        return MMSYSERR_INVALPARAM;

    Caps c = {};
    c.wMid = 0xFFFF;
    c.wPid = 0xFFFF;
    c.vDriverVersion = 0x0100;
    for (size_t i = 0; i < sizeof(kKdmapiName) / sizeof(kKdmapiName[0]); ++i)
        c.szPname[i] = static_cast<std::remove_reference_t<decltype(c.szPname[0])>>(kKdmapiName[i]);
    c.wTechnology = MOD_SWSYNTH;
    c.wVoices = 0xFFFF;
    c.wNotes = 0xFFFF;
    c.wChannelMask = 0xFFFF;
    c.dwSupport = MIDICAPS_VOLUME | MIDICAPS_STREAM;

    CopyMemory(caps, &c, size < sizeof(c) ? size : sizeof(c));
    return MMSYSERR_NOERROR;
}

MMRESULT openKdmapi(HMIDI handle, DWORD_PTR cb, DWORD_PTR inst, DWORD flags) {
    if (g_kdmapiOpen)
        return MMSYSERR_ALLOCATED;

    if (!kdmapi::load())
        return MMSYSERR_NODRIVER;

    const kdmapi::Api& k = kdmapi::api;
    if (!k.InitializeCallbackFeatures(handle, cb, inst, reinterpret_cast<DWORD_PTR>(&g_kdmapiUser), flags)) {
        kdmapi::unload();
        return MMSYSERR_INVALPARAM;
    }

    // Close any leftover stream, just to be safe
    k.TerminateKDMAPIStream();
    if (!k.InitializeKDMAPIStream()) {
        kdmapi::unload();
        return MMSYSERR_ERROR;
    }

    k.DriverSettings(0xFFFFF, 0, nullptr, 0);
    k.RunCallbackFunction(MOM_OPEN, 0, 0);
    g_kdmapiOpen = true;
    return MMSYSERR_NOERROR;
}

MMRESULT closeKdmapi() {
    if (!g_kdmapiOpen)
        return MMSYSERR_INVALHANDLE;

    const kdmapi::Api& k = kdmapi::api;
    if (!k.TerminateKDMAPIStream())
        return MMSYSERR_ERROR;

    k.DriverSettings(0xFFFFE, 0, nullptr, 0);
    k.RunCallbackFunction(MOM_CLOSE, 0, 0);

    g_kdmapiOpen = false;
    kdmapi::unload();
    return MMSYSERR_NOERROR;
}

MMRESULT kdmapiMessage(UINT msg, DWORD_PTR a, DWORD_PTR b) {
    return kdmapi::api.modMessage(kKdmapiId, msg, g_kdmapiUser, a, b);
}

} // namespace

namespace wrp {

bool osAtLeast(DWORD major, DWORD minor) {
    typedef LONG(WINAPI * RtlGetVersionFn)(OSVERSIONINFOW*);
    HMODULE ntdll = GetModuleHandleW(L"ntdll.dll");
    auto fn = ntdll ? reinterpret_cast<RtlGetVersionFn>(
                          reinterpret_cast<void*>(GetProcAddress(ntdll, "RtlGetVersion")))
                    : nullptr;

    OSVERSIONINFOW vi = {};
    vi.dwOSVersionInfoSize = sizeof(vi);
    if (!fn || fn(&vi) != 0)
        return false;

    return vi.dwMajorVersion > major || (vi.dwMajorVersion == major && vi.dwMinorVersion >= minor);
}

void initMidiOut() {
#ifndef WRP_NO_FAST_SHORTMSG
    // The handle layout used by the fast path only exists on Windows 8+
    g_fastShortMsg = osAtLeast(6, 2);
#endif
}

void shutdownMidiOut() {
    if (g_kdmapiOpen)
        closeKdmapi();
    kdmapi::unload();
}

} // namespace wrp

WRP_EXPORT UINT WINAPI WRP_midiOutGetNumDevs() {
    return WINMM_midiOutGetNumDevs() + kDevOffset;
}

WRP_EXPORT MMRESULT WINAPI WRP_midiOutGetDevCapsW(UINT_PTR id, LPMIDIOUTCAPSW caps, UINT size) {
    if (kKdmapi && (id == kKdmapiId || isKdmapi(reinterpret_cast<HMIDIOUT>(id))))
        return fillKdmapiCaps(caps, size);
    return WINMM_midiOutGetDevCapsW(toRealId(id), caps, size);
}

WRP_EXPORT MMRESULT WINAPI WRP_midiOutGetDevCapsA(UINT_PTR id, LPMIDIOUTCAPSA caps, UINT size) {
    if (kKdmapi && (id == kKdmapiId || isKdmapi(reinterpret_cast<HMIDIOUT>(id))))
        return fillKdmapiCaps(caps, size);
    return WINMM_midiOutGetDevCapsA(toRealId(id), caps, size);
}

WRP_EXPORT MMRESULT WINAPI WRP_midiOutOpen(LPHMIDIOUT out, UINT id, DWORD_PTR cb, DWORD_PTR inst, DWORD flags) {
    if (kKdmapi && id == kKdmapiId) {
        if (!out)
            return MMSYSERR_INVALPARAM;

        MMRESULT r = openKdmapi(reinterpret_cast<HMIDI>(kKdmapiOut), cb, inst, flags);
        if (r == MMSYSERR_NOERROR)
            *out = kKdmapiOut;
        return r;
    }
    return WINMM_midiOutOpen(out, static_cast<UINT>(toRealId(id)), cb, inst, flags);
}

WRP_EXPORT MMRESULT WINAPI WRP_midiOutClose(HMIDIOUT h) {
    if (isKdmapi(h))
        return closeKdmapi();
    return WINMM_midiOutClose(h);
}

WRP_EXPORT MMRESULT WINAPI WRP_midiOutReset(HMIDIOUT h) {
    if (isKdmapi(h)) {
        kdmapi::api.ResetKDMAPIStream();
        return MMSYSERR_NOERROR;
    }
    return WINMM_midiOutReset(h);
}

WRP_EXPORT MMRESULT WINAPI WRP_midiOutPrepareHeader(HMIDIOUT h, LPMIDIHDR hdr, UINT size) {
    if (isKdmapi(h))
        return kdmapi::api.PrepareLongData(hdr, size);
    return WINMM_midiOutPrepareHeader(h, hdr, size);
}

WRP_EXPORT MMRESULT WINAPI WRP_midiOutUnprepareHeader(HMIDIOUT h, LPMIDIHDR hdr, UINT size) {
    if (isKdmapi(h))
        return kdmapi::api.UnprepareLongData(hdr, size);
    return WINMM_midiOutUnprepareHeader(h, hdr, size);
}

WRP_EXPORT MMRESULT WINAPI WRP_midiOutShortMsg(HMIDIOUT h, DWORD msg) {
    if (isKdmapi(h)) {
        kdmapi::api.SendDirectData(msg);
        return MMSYSERR_NOERROR;
    }

    if (g_fastShortMsg && h) {
        auto dev = reinterpret_cast<DeviceHandle*>(h);
        return dev->vtbl->modMessage(dev->id, MODM_DATA, dev->user, msg, 0);
    }

    return WINMM_midiOutShortMsg(h, msg);
}

WRP_EXPORT MMRESULT WINAPI WRP_midiOutLongMsg(HMIDIOUT h, LPMIDIHDR hdr, UINT size) {
    if (isKdmapi(h)) {
        MMRESULT r = kdmapi::api.SendDirectLongData(hdr, size);
        // MOM_DONE: dwParam1 = MIDIHDR*, dwParam2 = 0 (needed by vanBasco)
        kdmapi::api.RunCallbackFunction(MOM_DONE, reinterpret_cast<DWORD_PTR>(hdr), 0);
        return r;
    }
    return WINMM_midiOutLongMsg(h, hdr, size);
}

WRP_EXPORT MMRESULT WINAPI WRP_midiOutMessage(HMIDIOUT h, UINT msg, DWORD_PTR a, DWORD_PTR b) {
    if (isKdmapi(h))
        return MMSYSERR_NOERROR;
    return WINMM_midiOutMessage(h, msg, a, b);
}

WRP_EXPORT MMRESULT WINAPI WRP_midiOutSetVolume(HMIDIOUT h, DWORD vol) {
    if (isKdmapi(h))
        return kdmapiMessage(MODM_SETVOLUME, vol, 0);
    return WINMM_midiOutSetVolume(h, vol);
}

WRP_EXPORT MMRESULT WINAPI WRP_midiOutGetVolume(HMIDIOUT h, LPDWORD vol) {
    if (isKdmapi(h))
        return kdmapiMessage(MODM_GETVOLUME, reinterpret_cast<DWORD_PTR>(vol), 0);
    return WINMM_midiOutGetVolume(h, vol);
}

WRP_EXPORT MMRESULT WINAPI WRP_midiOutGetID(HMIDIOUT h, LPUINT id) {
    if (isKdmapi(h)) {
        if (!id)
            return MMSYSERR_INVALPARAM;
        *id = kKdmapiId;
        return MMSYSERR_NOERROR;
    }

    MMRESULT r = WINMM_midiOutGetID(h, id);
    if (r == MMSYSERR_NOERROR && id && !isMapper(*id))
        *id += kDevOffset;
    return r;
}

WRP_EXPORT MMRESULT WINAPI WRP_midiOutCachePatches(HMIDIOUT h, UINT bank, LPWORD patches, UINT flags) {
    if (isKdmapi(h))
        return MMSYSERR_NOERROR;
    return WINMM_midiOutCachePatches(h, bank, patches, flags);
}

WRP_EXPORT MMRESULT WINAPI WRP_midiOutCacheDrumPatches(HMIDIOUT h, UINT patch, LPWORD keys, UINT flags) {
    if (isKdmapi(h))
        return MMSYSERR_NOERROR;
    return WINMM_midiOutCacheDrumPatches(h, patch, keys, flags);
}

WRP_EXPORT MMRESULT WINAPI WRP_midiStreamOpen(LPHMIDISTRM out, LPUINT id, DWORD count, DWORD_PTR cb, DWORD_PTR inst, DWORD flags) {
    if (!out || !id)
        return MMSYSERR_INVALPARAM;

    if (kKdmapi && *id == kKdmapiId) {
        MMRESULT r = openKdmapi(reinterpret_cast<HMIDI>(kKdmapiStrm), cb, inst, flags | MIDI_IO_COOKED);
        if (r == MMSYSERR_NOERROR)
            *out = kKdmapiStrm;
        return r;
    }

    UINT realId = static_cast<UINT>(toRealId(*id));
    return WINMM_midiStreamOpen(out, &realId, count, cb, inst, flags);
}

WRP_EXPORT MMRESULT WINAPI WRP_midiStreamClose(HMIDISTRM h) {
    if (isKdmapi(h))
        return closeKdmapi();
    return WINMM_midiStreamClose(h);
}

WRP_EXPORT MMRESULT WINAPI WRP_midiStreamOut(HMIDISTRM h, LPMIDIHDR hdr, UINT size) {
    if (isKdmapi(h))
        return kdmapiMessage(MODM_STRMDATA, reinterpret_cast<DWORD_PTR>(hdr), size);
    return WINMM_midiStreamOut(h, hdr, size);
}

WRP_EXPORT MMRESULT WINAPI WRP_midiStreamPause(HMIDISTRM h) {
    if (isKdmapi(h))
        return kdmapiMessage(MODM_PAUSE, 0, 0);
    return WINMM_midiStreamPause(h);
}

WRP_EXPORT MMRESULT WINAPI WRP_midiStreamRestart(HMIDISTRM h) {
    if (isKdmapi(h))
        return kdmapiMessage(MODM_RESTART, 0, 0);
    return WINMM_midiStreamRestart(h);
}

WRP_EXPORT MMRESULT WINAPI WRP_midiStreamStop(HMIDISTRM h) {
    if (isKdmapi(h))
        return kdmapiMessage(MODM_STOP, 0, 0);
    return WINMM_midiStreamStop(h);
}

WRP_EXPORT MMRESULT WINAPI WRP_midiStreamProperty(HMIDISTRM h, LPBYTE data, DWORD prop) {
    if (isKdmapi(h))
        return kdmapiMessage(MODM_PROPERTIES, reinterpret_cast<DWORD_PTR>(data), prop);
    return WINMM_midiStreamProperty(h, data, prop);
}

WRP_EXPORT MMRESULT WINAPI WRP_midiStreamPosition(HMIDISTRM h, LPMMTIME time, UINT size) {
    if (isKdmapi(h))
        return kdmapiMessage(MODM_GETPOS, reinterpret_cast<DWORD_PTR>(time), size);
    return WINMM_midiStreamPosition(h, time, size);
}

// Some old games (e.g. Call of Duty) crash with more than 15 devices
WRP_EXPORT UINT WINAPI WRP_mixerGetNumDevs() {
    UINT n = WINMM_mixerGetNumDevs();
#ifdef WRP_CODPATCH
    if (n > 15)
        n = 15;
#endif
    return n;
}

WRP_EXPORT UINT WINAPI WRP_waveInGetNumDevs() {
    UINT n = WINMM_waveInGetNumDevs();
#ifdef WRP_CODPATCH
    if (n > 15)
        n = 15;
#endif
    return n;
}
