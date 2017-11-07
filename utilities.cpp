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
credit: Synraw @ unknowncheats.me
*/

#include "offsets.h"
#include "Path.h"

#include "Psapi.h"
#include "menu.h"
#include "interfaces.h"

#include "tinyxml.h"

#include <locale>
#include <codecvt>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <sstream>

#define __cyanver "0.8.002"

#ifdef __cyandbg
bool Utilities::unload = false;
#endif

std::string Utilities::VersionString = "v" + std::string(__cyanver);

/* ---------- UTILITIES MEMORY ---------- */
DWORD Utilities::Memory::WaitOnModuleHandle(std::string moduleName)
{
	DWORD ModuleHandle = NULL;

	while (!ModuleHandle)
	{
		ModuleHandle = (DWORD)GetModuleHandle(moduleName.c_str());
		if (!ModuleHandle)
			Sleep(50);
	}

	return ModuleHandle;
}

bool bCompare(const BYTE* Data, const BYTE* Mask, const char* szMask)
{
	for (; *szMask; ++szMask, ++Mask, ++Data)
		if (*szMask == 'x' && *Mask != *Data)
			return false;

	return (*szMask) == 0;
}

DWORD Utilities::Memory::FindPattern(std::string moduleName, std::string pattern)
{
	const char* pat = pattern.c_str();
	DWORD firstMatch = 0;
	DWORD rangeStart = (DWORD)GetModuleHandleA(moduleName.c_str());
	MODULEINFO miModInfo; GetModuleInformation(GetCurrentProcess(), (HMODULE)rangeStart, &miModInfo, sizeof(MODULEINFO));
	DWORD rangeEnd = rangeStart + miModInfo.SizeOfImage;
	for (DWORD pCur = rangeStart; pCur < rangeEnd; pCur++)
	{
		if (!*pat)
			return firstMatch;

		if (*(PBYTE)pat == '\?' || *(BYTE*)pCur == getByte(pat))
		{
			if (!firstMatch)
				firstMatch = pCur;

			if (!pat[2])
				return firstMatch;

			if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?')
				pat += 3;

			else
				pat += 2;    //one ?
		}
		else
		{
			pat = pattern.c_str();
			firstMatch = 0;
		}
	}

	Utilities::Error("Failed to find pattern in " + moduleName + ". (" + pattern + ")");
	return NULL;
}

DWORD Utilities::Memory::FindPatternMask(std::string moduleName, BYTE* Mask, char* szMask)
{
	DWORD Address = WaitOnModuleHandle(moduleName.c_str());
	MODULEINFO ModInfo; GetModuleInformation(GetCurrentProcess(), (HMODULE)Address, &ModInfo, sizeof(MODULEINFO));
	DWORD Length = ModInfo.SizeOfImage;

	for (DWORD c = 0; c < Length; c += 1)
		if (bCompare((BYTE*)(Address + c), Mask, szMask))
			return (DWORD)(Address + c);

	Utilities::Error("Failed to find pattern in " + moduleName + ". (" + std::string(szMask) + ", " + std::string((char*)Mask) + ")");
	return 0;
}

DWORD Utilities::Memory::FindTextPattern(std::string moduleName, char* string)
{
	DWORD Address = WaitOnModuleHandle(moduleName.c_str());
	MODULEINFO ModInfo; GetModuleInformation(GetCurrentProcess(), (HMODULE)Address, &ModInfo, sizeof(MODULEINFO));
	DWORD Length = ModInfo.SizeOfImage;

	int len = strlen(string);
	char* szMask = new char[len + 1];

	for (int i = 0; i < len; i++)
		szMask[i] = 'x';

	szMask[len] = '\0';

	for (DWORD c = 0; c < Length; c += 1)
		if (bCompare((BYTE*)(Address + c), (BYTE*)string, szMask))
			return (DWORD)(Address + c);

	Utilities::Error("Failed to find pattern in " + moduleName + ". (" + std::string(string) + ")");
	return 0;
}

/* ---------- UTILITIES MEMORY VMT MANAGER ---------- */
bool Utilities::Memory::VMTManager::Initialize(uintptr_t* InstancePointer)
{
	// Store the instance pointers and such, and work out how big the table is
	Instance = InstancePointer;
	OriginalTable = (DWORD*)*InstancePointer;
	int VMTSize = MethodCount(InstancePointer);
	size_t TableBytes = VMTSize * 4;

	// Allocate some memory and copy the table
	CustomTable = (DWORD*)malloc(TableBytes + 8);
	if (!CustomTable) return false;
	memcpy((void*)CustomTable, (void*)OriginalTable, VMTSize * 4);

	// Change the pointer
	*InstancePointer = (DWORD)CustomTable;

	initComplete = true;
	return true;
}

int Utilities::Memory::VMTManager::MethodCount(uintptr_t* InstancePointer)
{/*
	DWORD *VMT = (DWORD*)*InstancePointer;
	int Index = 0;
	int Amount = 0;
	while (!IsBadCodePtr((FARPROC)VMT[Index]))
	{
		if (!IsBadCodePtr((FARPROC)VMT[Index]))
		{
			Amount++;
			Index++;
		}
	}

	return Amount;*/

	size_t i = 0;

	while (reinterpret_cast<uintptr_t*>(*InstancePointer)[i])
		i++;

	return i;
}

DWORD Utilities::Memory::VMTManager::HookMethod(DWORD NewFunction, int Index)
{
	if (initComplete)
	{
		CustomTable[Index] = NewFunction;
		return OriginalTable[Index];
	}
	else
		return NULL;
}

void Utilities::Memory::VMTManager::UnhookMethod(int Index)
{
	if (initComplete)
		CustomTable[Index] = OriginalTable[Index];
}

void Utilities::Memory::VMTManager::RestoreOriginal()
{
	if (initComplete)
		*Instance = (DWORD)OriginalTable;
}

void Utilities::Memory::VMTManager::RestoreCustom()
{
	if (initComplete)
		*Instance = (DWORD)CustomTable;
}

DWORD Utilities::Memory::VMTManager::GetOriginalFunction(int Index)
{
	return OriginalTable[Index];
}

#pragma warning(disable:4996)
const std::string Utilities::GetTime() {
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);
	// Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
	// for more information about date/time format
	strftime(buf, sizeof(buf), "[%Y-%m-%d %X]", &tstruct);

	return buf;
}
#pragma warning(default:4996)

void Utilities::Log(std::string s)
{
	s += "\n";
	if (I::CVar)
		I::CVar->ConsoleColorPrintf(Colors::Console::LOG, s.c_str());

#ifdef __cyanlog
	std::ofstream log;
	log.open(Path::Get("errorlog.txt"), std::ios_base::app);
	log << "[" << __cyanver << "] [LOG]   " << GetTime() << ": " << s;
#endif
}

#pragma warning(disable:4996)
void Utilities::Log(std::wstring s)
{
	char cstr[256];
	s += L"\n";
	std::wcstombs(cstr, s.c_str(), 256);

	I::CVar->ConsoleColorPrintf(Colors::Console::LOG, cstr);

#ifdef __cyanlog
	std::ofstream log;
	log.open(Path::Get("errorlog.txt"), std::ios_base::app);
	log << "[" << __cyanver << "] [LOG]   " << GetTime() << ": " << cstr;
#endif
}
#pragma warning(default:4996)

void Utilities::Warn(std::string s)
{
	s += "\n";
	I::CVar->ConsoleColorPrintf(Colors::Console::WARN, s.c_str());

#ifdef __cyanlog
	std::ofstream log;
	log.open(Path::Get("errorlog.txt"), std::ios_base::app);
	log << "[" << __cyanver << "] [WARN]  " << GetTime() << ": " << s;
#endif
}

void Utilities::Error(std::string s)
{
	s += "\n";
	I::CVar->ConsoleColorPrintf(Colors::Console::ERR, s.c_str());

#ifdef __cyanlog
	std::ofstream log;
	log.open(Path::Get("errorlog.txt"), std::ios_base::app);
	log << "[" << __cyanver << "] [ERROR] " << GetTime() << ": " << s;
#endif
}

void Utilities::GetLastCompetitiveMatchID()
{
	uint64_t id = reinterpret_cast<uint64_t(__cdecl*)()>(Offsets::GetLastCompetitiveMatchID)();
	Log(std::string("Last Match ID: " + std::to_string(id) + "\n").c_str());
}

void Utilities::csMessageBox(std::string title, std::string msg, bool ok, bool cancel)
{
	I::GameUI->CreateCommandMsgBox(title.c_str(), msg.c_str(), ok, cancel, "", "", "", "");
}

float Utilities::RandomFloat(float min, float max)
{
	typedef float(*RandomFloat_t)(float, float);
	static RandomFloat_t m_RandomFloat = (RandomFloat_t)GetProcAddress(GetModuleHandle("vstdlib.dll"), "RandomFloat");
	return m_RandomFloat(min, max);
}

int Utilities::RandomInt(int min, int max)
{
	typedef int(*RandomInt_t)(int, int);
	static RandomInt_t m_RandomInt = (RandomInt_t)GetProcAddress(GetModuleHandle("vstdlib.dll"), "RandomInt");
	return m_RandomInt(min, max);
}

std::string Utilities::ToString(const std::wstring & wstr)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.to_bytes(wstr);
}

bool Utilities::cmpstr(const char * cmp, std::vector<std::string> lst)
{
	return cmpstr(std::string(cmp), lst);
}

bool Utilities::cmpstr(std::string cmp, std::vector<std::string> lst)
{
	for (std::string s : lst)
		if (cmp == s)
			return true;

	return false;
}
