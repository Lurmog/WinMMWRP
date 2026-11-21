// WinMMWRP - Keppy's Direct MIDI API (OmniMIDI) bindings.

#include "kdmapi.hpp"

namespace kdmapi {

Api api = {};

namespace {

HMODULE g_lib = nullptr;

template <typename T>
bool bind(T& fn, const char* name) {
    fn = reinterpret_cast<T>(reinterpret_cast<void*>(GetProcAddress(g_lib, name)));
    return fn != nullptr;
}

} // namespace

bool load() {
    if (g_lib)
        return true;

    g_lib = LoadLibraryW(L"OmniMIDI.dll");
    if (!g_lib)
        return false;

    bool ok = bind(api.IsKDMAPIAvailable, "IsKDMAPIAvailable")
           && bind(api.InitializeKDMAPIStream, "InitializeKDMAPIStream")
           && bind(api.TerminateKDMAPIStream, "TerminateKDMAPIStream")
           && bind(api.ResetKDMAPIStream, "ResetKDMAPIStream")
           && bind(api.SendDirectData, "SendDirectData")
           && bind(api.SendDirectLongData, "SendDirectLongData")
           && bind(api.PrepareLongData, "PrepareLongData")
           && bind(api.UnprepareLongData, "UnprepareLongData")
           && bind(api.InitializeCallbackFeatures, "InitializeCallbackFeatures")
           && bind(api.RunCallbackFunction, "RunCallbackFunction")
           && bind(api.DriverSettings, "DriverSettings")
           && bind(api.modMessage, "modMessage");

    if (!ok || !api.IsKDMAPIAvailable()) {
        unload();
        return false;
    }

    return true;
}

void unload() {
    if (g_lib)
        FreeLibrary(g_lib);

    g_lib = nullptr;
    api = {};
}

bool loaded() {
    return g_lib != nullptr;
}

} // namespace kdmapi
