#pragma once

#include "Tab.h"
#include "settings.h"

namespace GUI
{
	extern Menu* cyan;
	extern ComboBox* TGloveSkinOptions;
	extern ComboBox* CTGloveSkinOptions;
	extern ComboBox* SkinOptions;

	bool KeyPressed(unsigned int key);
	bool KeyHeld(unsigned int key);

	void PressKey(unsigned int key);
	void LiftKey(unsigned int key);

	int MouseWheelDelta();

	void Initialize();
	void PaintTraverse();

	bool __declspec(dllexport) __stdcall SetHook();
	bool __declspec(dllexport) __stdcall RemoveHook();
	bool IsHookSet();
	void UpdateHook();

	void Save(std::string cfgName = cyan->GetConfigName());
	void Load(std::string cfgName = cyan->GetConfigName());
}