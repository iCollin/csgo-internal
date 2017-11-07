#include "SpotifyControls.h"

Spotify::Spotify()
{
	procWindow = FindWindow(windowClassName, NULL);
}

std::wstring Spotify::GetCurrentSong()
{
	int len = GetWindowTextLengthW(procWindow);
	if (len > 0) {
		std::wstring tmp(len + 1, 0);
		GetWindowTextW(procWindow, const_cast<wchar_t*>(tmp.c_str()), tmp.length());

		if (wcscmp(tmp.c_str(), L"Spotify\0"))
			return tmp;
	}

	return L"Paused";
}