set_project("WinMMWRP")
set_xmakever("2.8.0")

set_allowedplats("windows", "mingw")
set_allowedarchs("x86", "i386", "x64", "x86_64", "arm64")
add_rules("mode.release", "mode.debug")
set_languages("cxx17")

option("purewrapper", {default = false, showmenu = true, description = "Plain pass-through, no KDMAPI device"})
option("codpatch", {default = false, showmenu = true, description = "Cap mixer/waveIn device count at 15"})
option("fastshortmsg", {default = true, showmenu = true, description = "Bypass midiOutShortMsg overhead on Windows 8+"})

target("winmm")
    set_kind("shared")
    set_prefixname("")
    set_basename("winmm")
    add_files("src/*.cpp")

    -- Windows XP is the minimum target
    add_defines("WINVER=0x0501", "_WIN32_WINNT=0x0501", "UNICODE", "_UNICODE")

    if has_config("purewrapper") then add_defines("WRP_PURE_WRAPPER") end
    if has_config("codpatch") then add_defines("WRP_CODPATCH") end
    if not has_config("fastshortmsg") then add_defines("WRP_NO_FAST_SHORTMSG") end

    local x86 = is_arch("x86", "i386")
    local def = path.join(os.scriptdir(), "def", x86 and "winmm-x86.def" or "winmm.def")

    if is_plat("mingw") then
        local ver = x86 and {"5", "1"} or {"5", "2"}
        add_cxflags("-Wall", "-Wextra", "-fno-exceptions", "-fno-rtti")
        add_shflags(def, "-static", "-static-libgcc", "-static-libstdc++",
                    "-Wl,--enable-stdcall-fixup",
                    "-Wl,--major-os-version," .. ver[1], "-Wl,--minor-os-version," .. ver[2],
                    "-Wl,--major-subsystem-version," .. ver[1], "-Wl,--minor-subsystem-version," .. ver[2],
                    {force = true})
    else
        set_runtimes("MT")
        add_cxflags("/W4", "/GR-")
        add_shflags("/DEF:" .. def, "/SUBSYSTEM:WINDOWS," .. (x86 and "5.01" or "5.02"), {force = true})
    end

    if is_mode("release") then
        set_symbols("hidden")
        set_strip("all")
        set_optimize("fastest")
    end
target_end()
