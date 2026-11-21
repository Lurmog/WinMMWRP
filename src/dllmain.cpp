// WinMMWRP - DLL entry point.

#include "winmm.hpp"

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID reserved) {
    switch (reason) {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(module);
        if (!real::load())
            return FALSE;
        wrp::initMidiOut();
        break;

    case DLL_PROCESS_DETACH:
        // Skip cleanup when the process is exiting; the OS reclaims everything
        if (!reserved) {
            wrp::shutdownMidiOut();
            real::unload();
        }
        break;
    }

    return TRUE;
}
