// WinMMWRP - system winmm.dll loader and plain pass-through exports.

#include "winmm.hpp"

namespace real {

#define WMM_FN(ret, name, params, args, fallback) ret(WINAPI* name) params = nullptr;
#include "exports.inl"
#undef WMM_FN

namespace {

struct Import {
    void** slot;
    const char* name;
};

const Import kImports[] = {
#define WMM_FN(ret, name, params, args, fallback) {reinterpret_cast<void**>(&name), #name},
#include "exports.inl"
#undef WMM_FN
};

HMODULE g_winmm = nullptr;
HMODULE g_winmmbase = nullptr;

HMODULE loadSystemLibrary(const wchar_t* file) {
    wchar_t path[MAX_PATH];
    UINT len = GetSystemDirectoryW(path, MAX_PATH);
    if (len == 0 || len >= MAX_PATH)
        return nullptr;

    if (lstrlenW(file) + len + 2 > MAX_PATH)
        return nullptr;

    path[len++] = L'\\';
    lstrcpyW(path + len, file);

    HMODULE mod = LoadLibraryW(path);

    // Never bind to ourselves (e.g. if the wrapper got copied into System32)
    HMODULE self = nullptr;
    GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                       reinterpret_cast<LPCWSTR>(&loadSystemLibrary), &self);
    if (mod && mod == self) {
        FreeLibrary(mod);
        return nullptr;
    }

    return mod;
}

void* resolve(const char* name) {
    FARPROC p = nullptr;
    if (g_winmm)
        p = GetProcAddress(g_winmm, name);
    if (!p && g_winmmbase)
        p = GetProcAddress(g_winmmbase, name);
    return reinterpret_cast<void*>(p);
}

} // namespace

bool load() {
    if (g_winmm)
        return true;

    g_winmm = loadSystemLibrary(L"winmm.dll");
    // Only present on Windows 8 and later; optional
    g_winmmbase = loadSystemLibrary(L"winmmbase.dll");

    if (!g_winmm && !g_winmmbase)
        return false;

    for (const Import& imp : kImports)
        *imp.slot = resolve(imp.name);

    return true;
}

void unload() {
    for (const Import& imp : kImports)
        *imp.slot = nullptr;

    if (g_winmmbase)
        FreeLibrary(g_winmmbase);
    if (g_winmm)
        FreeLibrary(g_winmm);

    g_winmmbase = nullptr;
    g_winmm = nullptr;
}

} // namespace real

// Pass-through exports, named WINMM_<function>. The .def files map
// the public names to these (or to the WRP_ overrides in midiout.cpp).
#define WMM_FN(ret, name, params, args, fallback) \
    WRP_EXPORT ret WINAPI WINMM_##name params { return real::name ? real::name args : (ret)(fallback); }
#define WMM_VOID(name, params, args) \
    WRP_EXPORT void WINAPI WINMM_##name params { if (real::name) real::name args; }
#include "exports.inl"
#undef WMM_FN
#undef WMM_VOID
