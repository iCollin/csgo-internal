#pragma once

#include <Windows.h>
#include <string>
#include <iostream>

enum SPotifyActions
{
	PlayPause = 917504,
	PreviousTrack = 786432,
	NextTrack = 720896,
};

const std::wstring g_SpotifyPrefix = L"Spotify - ";
const std::wstring g_SpotifyDelimiter = L" \x2013 ";

class Spotify
{
public:
	Spotify();
	
	inline void Next() { SendMessage(procWindow, WM_APPCOMMAND, 0, NextTrack); }
	inline void Pause() { SendMessage(procWindow, WM_APPCOMMAND, 0, PlayPause); }
	inline void Previous() { SendMessage(procWindow, WM_APPCOMMAND, 0, PreviousTrack); }
	inline bool _IsAttached() { return procWindow != NULL; }
	std::wstring GetCurrentSong();

private:
	const LPCTSTR windowClassName = "SpotifyMainWindow";
	HWND procWindow;
};