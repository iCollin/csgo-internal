/*
      ___           ___           ___           ___     
     /\  \         |\__\         /\  \         /\__\    
    /::\  \        |:|  |       /::\  \       /::|  |   
   /:/\:\  \       |:|  |      /:/\:\  \     /:|:|  |   
  /:/  \:\  \      |:|__|__   /::\~\:\  \   /:/|:|  |__ 
 /:/__/ \:\__\     /::::\__\ /:/\:\ \:\__\ /:/ |:| /\__\
 \:\  \  \/__/    /:/~~/~~   \/__\:\/:/  / \/__|:|/:/  /
  \:\  \         /:/  /           \::/  /      |:/:/  / 
   \:\  \        \/__/            /:/  /       |::/  /  
    \:\__\                       /:/  /        /:/  /   
     \/__/                       \/__/         \/__/    

revolt (4/2017)
Synraw @ unknowncheats.me
*/

#pragma once

#ifndef UTILITIES_H
#define UTILITIES_H

#ifdef _DEBUG
#define __cyandbg
#define __cyanlog
#endif

#include "Windows.h"
#include <vector>
#include <string>
#include <memory>

#define INRANGE(x,a,b)    (x >= a && x <= b) 
#define getBits( x )    (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define getByte( x )    (getBits(x[0]) << 4 | getBits(x[1]))

#define DEBUG_RETURN(str) { Utilities::Debug(str); return false; }
#define DEBUG_RETURN(str, ret) { Utilities::Debug(str); return ret; }

namespace Utilities
{
	namespace Memory
	{
		// Waits for a module to be available, before returning it's base address
		DWORD WaitOnModuleHandle(std::string moduleName);

		// Attempts to locate the given signature and mask in a memory range
		// Returns the address at which it has been found
		DWORD FindPatternMask(std::string moduleName, BYTE* Mask, char* szMask);

		DWORD FindPattern(std::string moduleName, std::string pattern);

		// Attempts to locate the given text in a memory range
		// Returns the address at which it has been found
		DWORD FindTextPattern(std::string moduleName, char* string);

		class VMTManager
		{
		private:
			DWORD	*CustomTable;
			bool	initComplete;
			DWORD	*OriginalTable;
			uintptr_t	*Instance;

			int		MethodCount(uintptr_t* InstancePointer);

		public:
			bool	Initialize(uintptr_t* InstancePointer);

			DWORD	HookMethod(DWORD NewFunction, int Index);
			void	UnhookMethod(int Index);

			void	RestoreOriginal();
			void	RestoreCustom();

			template<typename T>
			T GetMethod(size_t nIndex)
			{
				return (T)OriginalTable[nIndex];
			}

			DWORD	GetOriginalFunction(int Index);
		};
	};

	extern HINSTANCE hModule;
	extern int LocalSteamID;

#ifdef __cyandbg
	extern bool unload;
#endif

	extern std::string VersionString;

	template <typename... Values>
	std::string Stringf(const char* sz, Values... args)
	{
		size_t size = snprintf(nullptr, 0, sz, args ...) + 1;
		std::unique_ptr<char[]> buf(new char[size]);
		snprintf(buf.get(), size, sz, args ...);
		return std::string(buf.get(), buf.get() + size - 1);
	}

	void Log(std::string s);
	void Warn(std::string s);
	void Error(std::string s);
	FORCEINLINE void Debug(std::string s)
	{
#ifdef __cyandbg
		Log(s);
#endif
	}

	/*template <typename... Values>
	void Logf(const char* sz, Values... args) { Log(Stringf(sz, args...)); }
	template <typename... Values>
	void Warnf(const char* sz, Values... args) { Warn(Stringf(sz, args...)); }
	template <typename... Values>
	void Errorf(const char* sz, Values... args) { Error(Stringf(sz, args...)); }*/
	template <typename... Values>
	void Debugf(const char* sz, Values... args) { Debug(Stringf(sz, args...)); }

	void Log(std::wstring s);
	//void Warn(std::wstring s);
	//void Error(std::wstring s);

	const std::string GetTime();

	void GetLastCompetitiveMatchID();

	void csMessageBox(std::string title, std::string msg, bool ok, bool cancel);

	float RandomFloat(float, float);
	int RandomInt(int, int);

	std::string ToString(const std::wstring& wstr);

	bool cmpstr(const char*, std::vector< std::string > );
	bool cmpstr(std::string, std::vector< std::string >);
}

#endif