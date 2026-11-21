// WinMMWRP - Keppy's Direct MIDI API (OmniMIDI) bindings.

#pragma once

#include "common.hpp"

namespace kdmapi {

struct Api {
    BOOL(WINAPI* IsKDMAPIAvailable)();
    BOOL(WINAPI* InitializeKDMAPIStream)();
    BOOL(WINAPI* TerminateKDMAPIStream)();
    void(WINAPI* ResetKDMAPIStream)();
    void(WINAPI* SendDirectData)(DWORD);
    UINT(WINAPI* SendDirectLongData)(MIDIHDR*, UINT);
    UINT(WINAPI* PrepareLongData)(MIDIHDR*, UINT);
    UINT(WINAPI* UnprepareLongData)(MIDIHDR*, UINT);
    BOOL(WINAPI* InitializeCallbackFeatures)(HMIDI, DWORD_PTR, DWORD_PTR, DWORD_PTR, DWORD);
    void(WINAPI* RunCallbackFunction)(DWORD, DWORD_PTR, DWORD_PTR);
    BOOL(WINAPI* DriverSettings)(DWORD, DWORD, LPVOID, UINT);
    MMRESULT(WINAPI* modMessage)(UINT, UINT, DWORD_PTR, DWORD_PTR, DWORD_PTR);
};

// Resolved API; only valid between a successful load() and unload().
extern Api api;

bool load();
void unload();
bool loaded();

} // namespace kdmapi
