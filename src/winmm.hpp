// WinMMWRP - pointers to the real system winmm.dll functions.

#pragma once

#include "common.hpp"

namespace real {

#define WMM_FN(ret, name, params, args, fallback) extern ret(WINAPI* name) params;
#include "exports.inl"
#undef WMM_FN

// Loads the system winmm.dll (and winmmbase.dll on Windows 8+)
// from the system directory and resolves every function pointer.
bool load();
void unload();

} // namespace real

// Pass-through exports (defined in winmm.cpp)
#define WMM_FN(ret, name, params, args, fallback) WRP_EXPORT ret WINAPI WINMM_##name params;
#include "exports.inl"
#undef WMM_FN
