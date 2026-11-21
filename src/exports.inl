// WinMMWRP - list of every winmm.dll function that gets forwarded.
//
// X-macro table, included several times with different definitions of:
//   WMM_FN(ret, name, (params), (args), fallback)
//   WMM_VOID(name, (params), (args))
//
// "fallback" is returned when the real function couldn't be resolved.
// Do not add include guards to this file.

#ifndef WMM_VOID
#define WMM_VOID(name, params, args) WMM_FN(void, name, params, args, )
#define WMM_VOID_DEFAULTED
#endif

#define E_NS MMSYSERR_NOTSUPPORTED
#define E_MCI MCIERR_UNSUPPORTED_FUNCTION

// Driver
WMM_FN(LRESULT, CloseDriver, (HDRVR h, LPARAM a, LPARAM b), (h, a, b), 0)
WMM_FN(HDRVR, OpenDriver, (LPCWSTR drv, LPCWSTR sec, LPARAM p), (drv, sec, p), nullptr)
WMM_FN(LRESULT, SendDriverMessage, (HDRVR h, UINT m, LPARAM a, LPARAM b), (h, m, a, b), 0)
WMM_FN(LRESULT, DefDriverProc, (DWORD_PTR id, HDRVR h, UINT m, LPARAM a, LPARAM b), (id, h, m, a, b), 0)
WMM_FN(BOOL, DriverCallback, (DWORD_PTR cb, DWORD f, HDRVR h, DWORD m, DWORD_PTR u, DWORD_PTR a, DWORD_PTR b), (cb, f, h, m, u, a, b), FALSE)
WMM_FN(HMODULE, DrvGetModuleHandle, (HDRVR h), (h), nullptr)
WMM_FN(HMODULE, GetDriverModuleHandle, (HDRVR h), (h), nullptr)

// Tasks / misc
WMM_FN(DWORD, mmGetCurrentTask, (), (), 0)
WMM_VOID(mmTaskBlock, (DWORD t), (t))
WMM_FN(UINT, mmTaskCreate, (WRP_TASKCALLBACK cb, HANDLE* h, DWORD_PTR i), (cb, h, i), E_NS)
WMM_FN(BOOL, mmTaskSignal, (DWORD t), (t), FALSE)
WMM_VOID(mmTaskYield, (), ())
WMM_FN(UINT, mmsystemGetVersion, (), (), 0x030A)

// PlaySound
WMM_FN(BOOL, PlaySound, (LPCSTR s, HMODULE m, DWORD f), (s, m, f), FALSE)
WMM_FN(BOOL, PlaySoundA, (LPCSTR s, HMODULE m, DWORD f), (s, m, f), FALSE)
WMM_FN(BOOL, PlaySoundW, (LPCWSTR s, HMODULE m, DWORD f), (s, m, f), FALSE)
WMM_FN(BOOL, sndPlaySoundA, (LPCSTR s, UINT f), (s, f), FALSE)
WMM_FN(BOOL, sndPlaySoundW, (LPCWSTR s, UINT f), (s, f), FALSE)

// Aux
WMM_FN(UINT, auxGetNumDevs, (), (), 0)
WMM_FN(MMRESULT, auxGetDevCapsA, (UINT_PTR id, LPAUXCAPSA c, UINT s), (id, c, s), E_NS)
WMM_FN(MMRESULT, auxGetDevCapsW, (UINT_PTR id, LPAUXCAPSW c, UINT s), (id, c, s), E_NS)
WMM_FN(MMRESULT, auxGetVolume, (UINT id, LPDWORD v), (id, v), E_NS)
WMM_FN(MMRESULT, auxSetVolume, (UINT id, DWORD v), (id, v), E_NS)
WMM_FN(MMRESULT, auxOutMessage, (UINT id, UINT m, DWORD_PTR a, DWORD_PTR b), (id, m, a, b), E_NS)

// Joystick
WMM_FN(UINT, joyGetNumDevs, (), (), 0)
WMM_FN(MMRESULT, joyConfigChanged, (DWORD f), (f), E_NS)
WMM_FN(MMRESULT, joyGetDevCapsA, (UINT_PTR id, LPJOYCAPSA c, UINT s), (id, c, s), E_NS)
WMM_FN(MMRESULT, joyGetDevCapsW, (UINT_PTR id, LPJOYCAPSW c, UINT s), (id, c, s), E_NS)
WMM_FN(MMRESULT, joyGetPos, (UINT id, LPJOYINFO i), (id, i), E_NS)
WMM_FN(MMRESULT, joyGetPosEx, (UINT id, LPJOYINFOEX i), (id, i), E_NS)
WMM_FN(MMRESULT, joyGetThreshold, (UINT id, LPUINT t), (id, t), E_NS)
WMM_FN(MMRESULT, joySetThreshold, (UINT id, UINT t), (id, t), E_NS)
WMM_FN(MMRESULT, joyReleaseCapture, (UINT id), (id), E_NS)
WMM_FN(MMRESULT, joySetCapture, (HWND w, UINT id, UINT p, BOOL c), (w, id, p, c), E_NS)

// MCI
WMM_FN(MCIERROR, mciSendCommandA, (MCIDEVICEID id, UINT m, DWORD_PTR a, DWORD_PTR b), (id, m, a, b), E_MCI)
WMM_FN(MCIERROR, mciSendCommandW, (MCIDEVICEID id, UINT m, DWORD_PTR a, DWORD_PTR b), (id, m, a, b), E_MCI)
WMM_FN(MCIERROR, mciSendStringA, (LPCSTR c, LPSTR r, UINT n, HWND w), (c, r, n, w), E_MCI)
WMM_FN(MCIERROR, mciSendStringW, (LPCWSTR c, LPWSTR r, UINT n, HWND w), (c, r, n, w), E_MCI)
WMM_FN(MCIDEVICEID, mciGetDeviceIDA, (LPCSTR d), (d), 0)
WMM_FN(MCIDEVICEID, mciGetDeviceIDW, (LPCWSTR d), (d), 0)
WMM_FN(MCIDEVICEID, mciGetDeviceIDFromElementIDA, (DWORD e, LPCSTR t), (e, t), 0)
WMM_FN(MCIDEVICEID, mciGetDeviceIDFromElementIDW, (DWORD e, LPCWSTR t), (e, t), 0)
WMM_FN(BOOL, mciGetErrorStringA, (MCIERROR e, LPSTR t, UINT n), (e, t, n), FALSE)
WMM_FN(BOOL, mciGetErrorStringW, (MCIERROR e, LPWSTR t, UINT n), (e, t, n), FALSE)
WMM_FN(BOOL, mciSetYieldProc, (MCIDEVICEID id, YIELDPROC p, DWORD d), (id, p, d), FALSE)
WMM_FN(YIELDPROC, mciGetYieldProc, (MCIDEVICEID id, LPDWORD d), (id, d), nullptr)
WMM_FN(HTASK, mciGetCreatorTask, (MCIDEVICEID id), (id), nullptr)
WMM_FN(BOOL, mciExecute, (LPCSTR c), (c), FALSE)
WMM_FN(BOOL, mciDriverNotify, (HANDLE w, MCIDEVICEID id, UINT s), (w, id, s), FALSE)
WMM_FN(UINT, mciDriverYield, (MCIDEVICEID id), (id), 0)
WMM_FN(DWORD_PTR, mciGetDriverData, (MCIDEVICEID id), (id), 0)
WMM_FN(BOOL, mciSetDriverData, (MCIDEVICEID id, DWORD_PTR d), (id, d), FALSE)
WMM_FN(UINT, mciLoadCommandResource, (HANDLE i, LPCWSTR n, UINT t), (i, n, t), static_cast<UINT>(-1))
WMM_FN(BOOL, mciFreeCommandResource, (UINT t), (t), FALSE)

// MIDI out
WMM_FN(UINT, midiOutGetNumDevs, (), (), 0)
WMM_FN(MMRESULT, midiOutGetDevCapsA, (UINT_PTR id, LPMIDIOUTCAPSA c, UINT s), (id, c, s), E_NS)
WMM_FN(MMRESULT, midiOutGetDevCapsW, (UINT_PTR id, LPMIDIOUTCAPSW c, UINT s), (id, c, s), E_NS)
WMM_FN(MMRESULT, midiOutGetErrorTextA, (MMRESULT e, LPSTR t, UINT n), (e, t, n), E_NS)
WMM_FN(MMRESULT, midiOutGetErrorTextW, (MMRESULT e, LPWSTR t, UINT n), (e, t, n), E_NS)
WMM_FN(MMRESULT, midiOutOpen, (LPHMIDIOUT p, UINT id, DWORD_PTR cb, DWORD_PTR i, DWORD f), (p, id, cb, i, f), E_NS)
WMM_FN(MMRESULT, midiOutClose, (HMIDIOUT h), (h), E_NS)
WMM_FN(MMRESULT, midiOutReset, (HMIDIOUT h), (h), E_NS)
WMM_FN(MMRESULT, midiOutShortMsg, (HMIDIOUT h, DWORD m), (h, m), E_NS)
WMM_FN(MMRESULT, midiOutLongMsg, (HMIDIOUT h, LPMIDIHDR hdr, UINT s), (h, hdr, s), E_NS)
WMM_FN(MMRESULT, midiOutPrepareHeader, (HMIDIOUT h, LPMIDIHDR hdr, UINT s), (h, hdr, s), E_NS)
WMM_FN(MMRESULT, midiOutUnprepareHeader, (HMIDIOUT h, LPMIDIHDR hdr, UINT s), (h, hdr, s), E_NS)
WMM_FN(MMRESULT, midiOutMessage, (HMIDIOUT h, UINT m, DWORD_PTR a, DWORD_PTR b), (h, m, a, b), E_NS)
WMM_FN(MMRESULT, midiOutGetVolume, (HMIDIOUT h, LPDWORD v), (h, v), E_NS)
WMM_FN(MMRESULT, midiOutSetVolume, (HMIDIOUT h, DWORD v), (h, v), E_NS)
WMM_FN(MMRESULT, midiOutGetID, (HMIDIOUT h, LPUINT id), (h, id), E_NS)
WMM_FN(MMRESULT, midiOutCachePatches, (HMIDIOUT h, UINT b, LPWORD p, UINT f), (h, b, p, f), E_NS)
WMM_FN(MMRESULT, midiOutCacheDrumPatches, (HMIDIOUT h, UINT p, LPWORD k, UINT f), (h, p, k, f), E_NS)

// MIDI stream
WMM_FN(MMRESULT, midiStreamOpen, (LPHMIDISTRM p, LPUINT id, DWORD n, DWORD_PTR cb, DWORD_PTR i, DWORD f), (p, id, n, cb, i, f), E_NS)
WMM_FN(MMRESULT, midiStreamClose, (HMIDISTRM h), (h), E_NS)
WMM_FN(MMRESULT, midiStreamOut, (HMIDISTRM h, LPMIDIHDR hdr, UINT s), (h, hdr, s), E_NS)
WMM_FN(MMRESULT, midiStreamPause, (HMIDISTRM h), (h), E_NS)
WMM_FN(MMRESULT, midiStreamRestart, (HMIDISTRM h), (h), E_NS)
WMM_FN(MMRESULT, midiStreamStop, (HMIDISTRM h), (h), E_NS)
WMM_FN(MMRESULT, midiStreamProperty, (HMIDISTRM h, LPBYTE d, DWORD p), (h, d, p), E_NS)
WMM_FN(MMRESULT, midiStreamPosition, (HMIDISTRM h, LPMMTIME t, UINT s), (h, t, s), E_NS)

// MIDI in
WMM_FN(UINT, midiInGetNumDevs, (), (), 0)
WMM_FN(MMRESULT, midiInGetDevCapsA, (UINT_PTR id, LPMIDIINCAPSA c, UINT s), (id, c, s), E_NS)
WMM_FN(MMRESULT, midiInGetDevCapsW, (UINT_PTR id, LPMIDIINCAPSW c, UINT s), (id, c, s), E_NS)
WMM_FN(MMRESULT, midiInGetErrorTextA, (MMRESULT e, LPSTR t, UINT n), (e, t, n), E_NS)
WMM_FN(MMRESULT, midiInGetErrorTextW, (MMRESULT e, LPWSTR t, UINT n), (e, t, n), E_NS)
WMM_FN(MMRESULT, midiInOpen, (LPHMIDIIN p, UINT id, DWORD_PTR cb, DWORD_PTR i, DWORD f), (p, id, cb, i, f), E_NS)
WMM_FN(MMRESULT, midiInClose, (HMIDIIN h), (h), E_NS)
WMM_FN(MMRESULT, midiInPrepareHeader, (HMIDIIN h, LPMIDIHDR hdr, UINT s), (h, hdr, s), E_NS)
WMM_FN(MMRESULT, midiInUnprepareHeader, (HMIDIIN h, LPMIDIHDR hdr, UINT s), (h, hdr, s), E_NS)
WMM_FN(MMRESULT, midiInAddBuffer, (HMIDIIN h, LPMIDIHDR hdr, UINT s), (h, hdr, s), E_NS)
WMM_FN(MMRESULT, midiInStart, (HMIDIIN h), (h), E_NS)
WMM_FN(MMRESULT, midiInStop, (HMIDIIN h), (h), E_NS)
WMM_FN(MMRESULT, midiInReset, (HMIDIIN h), (h), E_NS)
WMM_FN(MMRESULT, midiInGetID, (HMIDIIN h, LPUINT id), (h, id), E_NS)
WMM_FN(MMRESULT, midiInMessage, (HMIDIIN h, UINT m, DWORD_PTR a, DWORD_PTR b), (h, m, a, b), E_NS)
WMM_FN(MMRESULT, midiConnect, (HMIDI h, HMIDIOUT o, LPVOID r), (h, o, r), E_NS)
WMM_FN(MMRESULT, midiDisconnect, (HMIDI h, HMIDIOUT o, LPVOID r), (h, o, r), E_NS)

// Mixer
WMM_FN(UINT, mixerGetNumDevs, (), (), 0)
WMM_FN(MMRESULT, mixerGetDevCapsA, (UINT_PTR id, LPMIXERCAPSA c, UINT s), (id, c, s), E_NS)
WMM_FN(MMRESULT, mixerGetDevCapsW, (UINT_PTR id, LPMIXERCAPSW c, UINT s), (id, c, s), E_NS)
WMM_FN(MMRESULT, mixerOpen, (LPHMIXER p, UINT id, DWORD_PTR cb, DWORD_PTR i, DWORD f), (p, id, cb, i, f), E_NS)
WMM_FN(MMRESULT, mixerClose, (HMIXER h), (h), E_NS)
WMM_FN(DWORD, mixerMessage, (HMIXER h, UINT m, DWORD_PTR a, DWORD_PTR b), (h, m, a, b), E_NS)
WMM_FN(MMRESULT, mixerGetID, (HMIXEROBJ h, UINT* id, DWORD f), (h, id, f), E_NS)
WMM_FN(MMRESULT, mixerGetLineInfoA, (HMIXEROBJ h, LPMIXERLINEA l, DWORD f), (h, l, f), E_NS)
WMM_FN(MMRESULT, mixerGetLineInfoW, (HMIXEROBJ h, LPMIXERLINEW l, DWORD f), (h, l, f), E_NS)
WMM_FN(MMRESULT, mixerGetLineControlsA, (HMIXEROBJ h, LPMIXERLINECONTROLSA l, DWORD f), (h, l, f), E_NS)
WMM_FN(MMRESULT, mixerGetLineControlsW, (HMIXEROBJ h, LPMIXERLINECONTROLSW l, DWORD f), (h, l, f), E_NS)
WMM_FN(MMRESULT, mixerGetControlDetailsA, (HMIXEROBJ h, LPMIXERCONTROLDETAILS d, DWORD f), (h, d, f), E_NS)
WMM_FN(MMRESULT, mixerGetControlDetailsW, (HMIXEROBJ h, LPMIXERCONTROLDETAILS d, DWORD f), (h, d, f), E_NS)
WMM_FN(MMRESULT, mixerSetControlDetails, (HMIXEROBJ h, LPMIXERCONTROLDETAILS d, DWORD f), (h, d, f), E_NS)

// MMIO
WMM_FN(FOURCC, mmioStringToFOURCCA, (LPCSTR s, UINT f), (s, f), 0)
WMM_FN(FOURCC, mmioStringToFOURCCW, (LPCWSTR s, UINT f), (s, f), 0)
WMM_FN(LPMMIOPROC, mmioInstallIOProcA, (FOURCC c, LPMMIOPROC p, DWORD f), (c, p, f), nullptr)
WMM_FN(LPMMIOPROC, mmioInstallIOProcW, (FOURCC c, LPMMIOPROC p, DWORD f), (c, p, f), nullptr)
WMM_FN(HMMIO, mmioOpenA, (LPSTR n, LPMMIOINFO i, DWORD f), (n, i, f), nullptr)
WMM_FN(HMMIO, mmioOpenW, (LPWSTR n, LPMMIOINFO i, DWORD f), (n, i, f), nullptr)
WMM_FN(MMRESULT, mmioRenameA, (LPCSTR o, LPCSTR n, const MMIOINFO* i, DWORD f), (o, n, i, f), E_NS)
WMM_FN(MMRESULT, mmioRenameW, (LPCWSTR o, LPCWSTR n, const MMIOINFO* i, DWORD f), (o, n, i, f), E_NS)
WMM_FN(MMRESULT, mmioClose, (HMMIO h, UINT f), (h, f), E_NS)
WMM_FN(LONG, mmioRead, (HMMIO h, HPSTR p, LONG n), (h, p, n), -1)
WMM_FN(LONG, mmioWrite, (HMMIO h, const char* p, LONG n), (h, p, n), -1)
WMM_FN(LONG, mmioSeek, (HMMIO h, LONG o, int w), (h, o, w), -1)
WMM_FN(MMRESULT, mmioGetInfo, (HMMIO h, LPMMIOINFO i, UINT f), (h, i, f), E_NS)
WMM_FN(MMRESULT, mmioSetInfo, (HMMIO h, const MMIOINFO* i, UINT f), (h, i, f), E_NS)
WMM_FN(MMRESULT, mmioSetBuffer, (HMMIO h, LPSTR b, LONG n, UINT f), (h, b, n, f), E_NS)
WMM_FN(MMRESULT, mmioFlush, (HMMIO h, UINT f), (h, f), E_NS)
WMM_FN(MMRESULT, mmioAdvance, (HMMIO h, LPMMIOINFO i, UINT f), (h, i, f), E_NS)
WMM_FN(LRESULT, mmioSendMessage, (HMMIO h, UINT m, LPARAM a, LPARAM b), (h, m, a, b), 0)
WMM_FN(MMRESULT, mmioDescend, (HMMIO h, LPMMCKINFO c, const MMCKINFO* p, UINT f), (h, c, p, f), E_NS)
WMM_FN(MMRESULT, mmioAscend, (HMMIO h, LPMMCKINFO c, UINT f), (h, c, f), E_NS)
WMM_FN(MMRESULT, mmioCreateChunk, (HMMIO h, LPMMCKINFO c, UINT f), (h, c, f), E_NS)

// Timers
WMM_FN(DWORD, timeGetTime, (), (), GetTickCount())
WMM_FN(MMRESULT, timeGetSystemTime, (LPMMTIME t, UINT s), (t, s), TIMERR_NOCANDO)
WMM_FN(MMRESULT, timeGetDevCaps, (LPTIMECAPS c, UINT s), (c, s), TIMERR_NOCANDO)
WMM_FN(MMRESULT, timeBeginPeriod, (UINT p), (p), TIMERR_NOCANDO)
WMM_FN(MMRESULT, timeEndPeriod, (UINT p), (p), TIMERR_NOCANDO)
WMM_FN(MMRESULT, timeSetEvent, (UINT d, UINT r, LPTIMECALLBACK cb, DWORD_PTR u, UINT f), (d, r, cb, u, f), 0)
WMM_FN(MMRESULT, timeKillEvent, (UINT id), (id), TIMERR_NOCANDO)

// Wave out
WMM_FN(UINT, waveOutGetNumDevs, (), (), 0)
WMM_FN(MMRESULT, waveOutGetDevCapsA, (UINT_PTR id, LPWAVEOUTCAPSA c, UINT s), (id, c, s), E_NS)
WMM_FN(MMRESULT, waveOutGetDevCapsW, (UINT_PTR id, LPWAVEOUTCAPSW c, UINT s), (id, c, s), E_NS)
WMM_FN(MMRESULT, waveOutGetErrorTextA, (MMRESULT e, LPSTR t, UINT n), (e, t, n), E_NS)
WMM_FN(MMRESULT, waveOutGetErrorTextW, (MMRESULT e, LPWSTR t, UINT n), (e, t, n), E_NS)
WMM_FN(MMRESULT, waveOutOpen, (LPHWAVEOUT p, UINT id, LPCWAVEFORMATEX fmt, DWORD_PTR cb, DWORD_PTR i, DWORD f), (p, id, fmt, cb, i, f), E_NS)
WMM_FN(MMRESULT, waveOutClose, (HWAVEOUT h), (h), E_NS)
WMM_FN(MMRESULT, waveOutPrepareHeader, (HWAVEOUT h, LPWAVEHDR hdr, UINT s), (h, hdr, s), E_NS)
WMM_FN(MMRESULT, waveOutUnprepareHeader, (HWAVEOUT h, LPWAVEHDR hdr, UINT s), (h, hdr, s), E_NS)
WMM_FN(MMRESULT, waveOutWrite, (HWAVEOUT h, LPWAVEHDR hdr, UINT s), (h, hdr, s), E_NS)
WMM_FN(MMRESULT, waveOutPause, (HWAVEOUT h), (h), E_NS)
WMM_FN(MMRESULT, waveOutRestart, (HWAVEOUT h), (h), E_NS)
WMM_FN(MMRESULT, waveOutReset, (HWAVEOUT h), (h), E_NS)
WMM_FN(MMRESULT, waveOutBreakLoop, (HWAVEOUT h), (h), E_NS)
WMM_FN(MMRESULT, waveOutGetPosition, (HWAVEOUT h, LPMMTIME t, UINT s), (h, t, s), E_NS)
WMM_FN(MMRESULT, waveOutGetPitch, (HWAVEOUT h, LPDWORD p), (h, p), E_NS)
WMM_FN(MMRESULT, waveOutSetPitch, (HWAVEOUT h, DWORD p), (h, p), E_NS)
WMM_FN(MMRESULT, waveOutGetPlaybackRate, (HWAVEOUT h, LPDWORD r), (h, r), E_NS)
WMM_FN(MMRESULT, waveOutSetPlaybackRate, (HWAVEOUT h, DWORD r), (h, r), E_NS)
WMM_FN(MMRESULT, waveOutGetVolume, (HWAVEOUT h, LPDWORD v), (h, v), E_NS)
WMM_FN(MMRESULT, waveOutSetVolume, (HWAVEOUT h, DWORD v), (h, v), E_NS)
WMM_FN(MMRESULT, waveOutGetID, (HWAVEOUT h, LPUINT id), (h, id), E_NS)
WMM_FN(MMRESULT, waveOutMessage, (HWAVEOUT h, UINT m, DWORD_PTR a, DWORD_PTR b), (h, m, a, b), E_NS)

// Wave in
WMM_FN(UINT, waveInGetNumDevs, (), (), 0)
WMM_FN(MMRESULT, waveInGetDevCapsA, (UINT_PTR id, LPWAVEINCAPSA c, UINT s), (id, c, s), E_NS)
WMM_FN(MMRESULT, waveInGetDevCapsW, (UINT_PTR id, LPWAVEINCAPSW c, UINT s), (id, c, s), E_NS)
WMM_FN(MMRESULT, waveInGetErrorTextA, (MMRESULT e, LPSTR t, UINT n), (e, t, n), E_NS)
WMM_FN(MMRESULT, waveInGetErrorTextW, (MMRESULT e, LPWSTR t, UINT n), (e, t, n), E_NS)
WMM_FN(MMRESULT, waveInOpen, (LPHWAVEIN p, UINT id, LPCWAVEFORMATEX fmt, DWORD_PTR cb, DWORD_PTR i, DWORD f), (p, id, fmt, cb, i, f), E_NS)
WMM_FN(MMRESULT, waveInClose, (HWAVEIN h), (h), E_NS)
WMM_FN(MMRESULT, waveInPrepareHeader, (HWAVEIN h, LPWAVEHDR hdr, UINT s), (h, hdr, s), E_NS)
WMM_FN(MMRESULT, waveInUnprepareHeader, (HWAVEIN h, LPWAVEHDR hdr, UINT s), (h, hdr, s), E_NS)
WMM_FN(MMRESULT, waveInAddBuffer, (HWAVEIN h, LPWAVEHDR hdr, UINT s), (h, hdr, s), E_NS)
WMM_FN(MMRESULT, waveInStart, (HWAVEIN h), (h), E_NS)
WMM_FN(MMRESULT, waveInStop, (HWAVEIN h), (h), E_NS)
WMM_FN(MMRESULT, waveInReset, (HWAVEIN h), (h), E_NS)
WMM_FN(MMRESULT, waveInGetPosition, (HWAVEIN h, LPMMTIME t, UINT s), (h, t, s), E_NS)
WMM_FN(MMRESULT, waveInGetID, (HWAVEIN h, LPUINT id), (h, id), E_NS)
WMM_FN(MMRESULT, waveInMessage, (HWAVEIN h, UINT m, DWORD_PTR a, DWORD_PTR b), (h, m, a, b), E_NS)

#if WRP_X86
// WOW32 thunks (32-bit winmm only)
WMM_FN(DWORD, aux32Message, (UINT id, UINT m, DWORD_PTR i, DWORD_PTR a, DWORD_PTR b), (id, m, i, a, b), E_NS)
WMM_FN(DWORD, joy32Message, (UINT id, UINT m, DWORD_PTR i, DWORD_PTR a, DWORD_PTR b), (id, m, i, a, b), E_NS)
WMM_FN(DWORD, mci32Message, (UINT id, UINT m, DWORD_PTR i, DWORD_PTR a, DWORD_PTR b), (id, m, i, a, b), E_NS)
WMM_FN(DWORD, mid32Message, (UINT id, UINT m, DWORD_PTR i, DWORD_PTR a, DWORD_PTR b), (id, m, i, a, b), E_NS)
WMM_FN(DWORD, mod32Message, (UINT id, UINT m, DWORD_PTR i, DWORD_PTR a, DWORD_PTR b), (id, m, i, a, b), E_NS)
WMM_FN(DWORD, mxd32Message, (UINT id, UINT m, DWORD_PTR i, DWORD_PTR a, DWORD_PTR b), (id, m, i, a, b), E_NS)
WMM_FN(DWORD, tid32Message, (UINT id, UINT m, DWORD_PTR i, DWORD_PTR a, DWORD_PTR b), (id, m, i, a, b), E_NS)
WMM_FN(DWORD, wid32Message, (UINT id, UINT m, DWORD_PTR i, DWORD_PTR a, DWORD_PTR b), (id, m, i, a, b), E_NS)
WMM_FN(DWORD, wod32Message, (UINT id, UINT m, DWORD_PTR i, DWORD_PTR a, DWORD_PTR b), (id, m, i, a, b), E_NS)
#endif

#undef E_NS
#undef E_MCI

#ifdef WMM_VOID_DEFAULTED
#undef WMM_VOID
#undef WMM_VOID_DEFAULTED
#endif
