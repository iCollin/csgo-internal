#include "KeyValues.h"

#include "interfaces.h"

void *KeyValues::operator new(size_t iAllocSize)
{
	static void* pKeyValuesSystem;
	if (!pKeyValuesSystem)
	{
		typedef PVOID(__cdecl* oKeyValuesSystem)();
		oKeyValuesSystem KeyValuesSystem = (oKeyValuesSystem)GetProcAddress(GetModuleHandleA("vstdlib.dll"), "KeyValuesSystem");

		pKeyValuesSystem = KeyValuesSystem();
	}

	typedef void*(__thiscall* oAllocKeyValuesMemory)(PVOID, size_t);
	return getvfunc<oAllocKeyValuesMemory>(pKeyValuesSystem, 1)(pKeyValuesSystem, iAllocSize);
}

void KeyValues::operator delete(PVOID pMem)
{
	static PVOID pKeyValuesSystem;
	if (!pKeyValuesSystem)
	{
		typedef PVOID(__cdecl* oKeyValuesSystem)();
		oKeyValuesSystem KeyValuesSystem = (oKeyValuesSystem)GetProcAddress(GetModuleHandleA("vstdlib.dll"), "KeyValuesSystem");

		pKeyValuesSystem = KeyValuesSystem();
	}

	typedef void(__thiscall* oFreeKeyValuesMemory)(PVOID, PVOID);
	getvfunc<oFreeKeyValuesMemory>(pKeyValuesSystem, 2)(pKeyValuesSystem, pMem);
}