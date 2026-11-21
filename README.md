<p align="center">
  <img src="https://i.imgur.com/iFLDs6C.png">
  <br />
  A group of functions, that allows you to bypass the Windows Multimedia API, thus getting rid of the lag caused by its slow buffer system.
</p>

# F.A.Q.

## What does this do?
Windows Multimedia Wrapper (as the name says), wraps around the default multimedia library in Windows, **winmm.dll**, and allows users to circumvent the extremely slow buffer that Microsoft has set in Windows 8.1 and Windows 10, when using the **midiOutShortMsg** call.

## Why did they make the function slower?
I have no idea really.<br/>
Aside from making it slower, the output latency *(MIDI app -> MIDI driver)* also got worse, since the function seems to make some useless checks on the message, which adds overhead.<br/>
I don't know if it's for safety purposes, or they just did something wrong in the function itself.

## Was the patch really necessary?
For me and other people, it was.<br/>
I really like Black MIDIs, which is a genre of music characterised by its extreme note arts and melodies.<br/>
Performance when playing them were okay, when using Windows XP/Vista/7, but Microsoft decided to **ruin** them on 8.x/10, so we had to find a way around it.<br/>
The first iteration of the *"wrapper"* was literally just **copying winmm.dll from Windows 7**, and using it on Windows 8.x. That worked fine on everything, until Microsoft decided to make that *impossible* on Windows 10. Everytime an app tries to load 7's winmm.dll on 10, it'd just throw an error, complaining about a missing ordinal.<br/>
For Windows 10, the second iteration was using **ReactOS' winmm.dll** on Windows 10. That also worked fine, aside from minor issues with the *timeGetTime* function (Which I fixed in later releases of the file).<br/>
Then, I decided to work on a *wrapper*, which would import the functions from the **real** Windows Multimedia library *(Allowing users to use both Microsoft GS and MIDI input devices)*, while also getting rid of the lag caused by some weird overhead in **midiOutShortMsg** *(When using my MIDI driver, that is ;P)*.<br/>
Long story short, it was a success. Everything works like it should, and people are happy.

# Building
WinMMWRP runs on **Windows XP and later** (x86 and x64). It only depends on `kernel32.dll` and `msvcrt.dll`.

### MinGW-w64 (recommended, works from Linux too)
```sh
# 32-bit
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=cmake/mingw-i686.cmake
cmake --build build

# 64-bit
cmake -S . -B build64 -DCMAKE_TOOLCHAIN_FILE=cmake/mingw-x86_64.cmake
cmake --build build64
```
On Windows with MSYS2, use the **mingw32/mingw64** environments, not ucrt64: UCRT doesn't exist on XP.

### xmake
```sh
xmake f -p mingw -a i386     # or x86_64
xmake
```

### MSVC
Configure with CMake using a toolset that still supports XP (e.g. `-T v141_xp`).

### Options
| CMake | xmake | Default | Description |
|---|---|---|---|
| `WRP_PURE_WRAPPER` | `purewrapper` | OFF | Plain pass-through, no KDMAPI device |
| `WRP_FAST_SHORTMSG` | `fastshortmsg` | ON | Bypass `midiOutShortMsg` overhead (only active on Windows 8+) |
| `WRP_CODPATCH` | `codpatch` | OFF | Cap mixer/waveIn device count at 15 for old games |

# Usage
Drop the `winmm.dll` matching the app's architecture next to its executable.
If OmniMIDI is installed, a **KDMAPI** device shows up as MIDI out device 0.

# Layout
| Path | Purpose |
|---|---|
| `src/exports.inl` | Single table of every forwarded winmm function |
| `src/winmm.cpp` | Loads the system winmm.dll and generates the pass-through exports |
| `src/midiout.cpp` | MIDI out overrides (KDMAPI device, fast short messages) |
| `src/kdmapi.cpp` | OmniMIDI KDMAPI bindings |
| `def/` | Export definitions (x86 adds the WOW32 `*32Message` thunks) |

# Known bugs
## Windows Multimedia Wrapper stock patch
- None, as far as I know.

## Windows Multimedia Wrapper DAW patch
- None, as far as I know.

# License
See [LICENSE](LICENSE).
