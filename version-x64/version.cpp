#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include "MinHook.h"

#pragma comment(lib, "libMinHook-x64-v141-md.lib")

HINSTANCE mHinst = 0, mHinstDLL = 0;
extern "C" UINT_PTR mProcs[17] = { 0 };

void LoadOriginalDll();
void InitMinHook();
void UninitMinHook();

LPCSTR mImportNames[] = { "GetFileVersionInfoA", "GetFileVersionInfoByHandle", "GetFileVersionInfoExA", "GetFileVersionInfoExW", "GetFileVersionInfoSizeA", "GetFileVersionInfoSizeExA", "GetFileVersionInfoSizeExW", "GetFileVersionInfoSizeW", "GetFileVersionInfoW", "VerFindFileA", "VerFindFileW", "VerInstallFileA", "VerInstallFileW", "VerLanguageNameA", "VerLanguageNameW", "VerQueryValueA", "VerQueryValueW" };
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
	mHinst = hinstDLL;
	if (fdwReason == DLL_PROCESS_ATTACH) {
		LoadOriginalDll();
		for (int i = 0; i < 17; i++) {
			mProcs[i] = (UINT_PTR)GetProcAddress(mHinstDLL, mImportNames[i]);
		}

		InitMinHook();
	}
	else if (fdwReason == DLL_PROCESS_DETACH) {
		FreeLibrary(mHinstDLL);
		UninitMinHook();
	}
	return (TRUE);
}

BOOL WINAPI DetourGetICMProfileW(HDC hdc, LPDWORD pBufSize, LPWSTR szFilename) {
	// we return false so that the monitor's color profile won't be used
	return false;
}

void InitMinHook() {
	// Initialize MinHook.
	if (MH_Initialize() != MH_OK) {
		MessageBox(NULL, _T("Unable to init MinHook"), _T(""), NULL);
		return;
	}

	// Create a hook for GetICMProfileW, in disabled state.
	if (MH_CreateHook(&GetICMProfileW, &DetourGetICMProfileW, NULL) != MH_OK) {
		MessageBox(NULL, _T("Unable to hook GetICMProfileW"), _T(""), NULL);
		return;
	}

	// Enable the hook for GetICMProfileW.
	if (MH_EnableHook(&GetICMProfileW) != MH_OK) {
		MessageBox(NULL, _T("Unable to enable the GetICMProfileW hook"), _T(""), NULL);
		return;
	}
}

void UninitMinHook() {
	// Uninitialize MinHook.
	if (MH_Uninitialize() != MH_OK) {
		return;
	}
}

extern "C" void GetFileVersionInfoA_wrapper();
extern "C" void GetFileVersionInfoByHandle_wrapper();
extern "C" void GetFileVersionInfoExA_wrapper();
extern "C" void GetFileVersionInfoExW_wrapper();
extern "C" void GetFileVersionInfoSizeA_wrapper();
extern "C" void GetFileVersionInfoSizeExA_wrapper();
extern "C" void GetFileVersionInfoSizeExW_wrapper();
extern "C" void GetFileVersionInfoSizeW_wrapper();
extern "C" void GetFileVersionInfoW_wrapper();
extern "C" void VerFindFileA_wrapper();
extern "C" void VerFindFileW_wrapper();
extern "C" void VerInstallFileA_wrapper();
extern "C" void VerInstallFileW_wrapper();
extern "C" void VerLanguageNameA_wrapper();
extern "C" void VerLanguageNameW_wrapper();
extern "C" void VerQueryValueA_wrapper();
extern "C" void VerQueryValueW_wrapper();

// Loads the original DLL from the default system directory
// Function originally written by Michael Koch
void LoadOriginalDll() {
	char buffer[MAX_PATH];

	// Get path to system dir and to version.dll
	GetSystemDirectory(buffer, MAX_PATH);

	// Append DLL name
	strcat_s(buffer, "\\version.dll");

	// Try to load the system's version.dll, if pointer empty
	if (!mHinstDLL) mHinstDLL = LoadLibrary(buffer);

	// Debug
	if (!mHinstDLL) {
		OutputDebugString("PROXYDLL: Original version.dll not loaded ERROR ****\r\n");
		ExitProcess(0); // Exit the hard way
	}
}

