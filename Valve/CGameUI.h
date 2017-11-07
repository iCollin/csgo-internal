#pragma once

class ECommandMsgBoxSlot;
class KeyValues;
class IPanel;

class CGameUI {
public:
	virtual void Initialize(void * (*)(char const*, int *)) = 0;
	virtual void PostInit(void) = 0;
	virtual void Connect(void * (*)(char const*, int *)) = 0;
	virtual void Start(void) = 0;
	virtual void Shutdown(void) = 0;
	virtual void RunFrame(void) = 0;
	virtual void OnGameUIActivated(void) = 0;
	virtual void OnGameUIHidden(void) = 0;
	virtual void OLD_OnConnectToServer(char const*, int, int) = 0;
	virtual void OnDisconnectFromServer_OLD(unsigned char, char const*) = 0;
	virtual void OnLevelLoadingStarted(char const*, bool) = 0;
	virtual void OnLevelLoadingFinished(bool, char const*, char const*) = 0;
	virtual void StartLoadingScreenForCommand(char const*) = 0;
	virtual void StartLoadingScreenForKeyValues(KeyValues *) = 0;
	virtual void UpdateProgressBar(float, char const*, bool) = 0;
	virtual void SetShowProgressText(bool) = 0;
	virtual void UpdateSecondaryProgressBar(float, wchar_t const*) = 0;
	virtual void SetProgressLevelName(char const*) = 0;
	virtual void ShowMessageDialog(unsigned int, IPanel *) = 0;
	virtual void ShowMessageDialog(char const*, char const*) = 0;
	virtual void CreateCommandMsgBox(char const*, char const*, bool, bool, char const*, char const*, char const*, char const*) = 0;
	virtual void CreateCommandMsgBoxInSlot(ECommandMsgBoxSlot, char const*, char const*, bool, bool, char const*, char const*, char const*, char const*) = 0;
	virtual void SetLoadingBackgroundDialog(unsigned long long) = 0;
	virtual void OnConnectToServer2(char const*, int, int, int) = 0;
	virtual void SetProgressOnStart(void) = 0;
	virtual void OnDisconnectFromServer(unsigned char) = 0;
	virtual void NeedConnectionProblemWaitScreen(void) = 0;
	virtual void ShowPasswordUI(char const*) = 0;
	virtual void LoadingProgressWantsIsolatedRender(bool) = 0;
	virtual bool IsPlayingFullScreenVideo(void) = 0;
	virtual bool IsTransitionEffectEnabled(void) = 0;
	virtual bool IsInLevel(void) = 0;
	virtual void RestoreTopLevelMenu(void) = 0;
	virtual void StartProgressBar(void) = 0;
	virtual void ContinueProgressBar(float, bool) = 0;
	virtual void StopProgressBar(bool, char const*, char const*) = 0;
	virtual void SetProgressBarStatusText(char const*, bool) = 0;
	virtual void SetSecondaryProgressBar(float) = 0;
	virtual void SetSecondaryProgressBarText(wchar_t const*) = 0;
};