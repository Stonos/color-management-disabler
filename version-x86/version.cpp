#include <windows.h>
#include <stdio.h>
HINSTANCE mHinst = 0, mHinstDLL = 0;
UINT_PTR mProcs[17] = {0};

void LoadOriginalDll();

LPCSTR mImportNames[] = {"GetFileVersionInfoA", "GetFileVersionInfoByHandle", "GetFileVersionInfoExA", "GetFileVersionInfoExW", "GetFileVersionInfoSizeA", "GetFileVersionInfoSizeExA", "GetFileVersionInfoSizeExW", "GetFileVersionInfoSizeW", "GetFileVersionInfoW", "VerFindFileA", "VerFindFileW", "VerInstallFileA", "VerInstallFileW", "VerLanguageNameA", "VerLanguageNameW", "VerQueryValueA", "VerQueryValueW"};
BOOL WINAPI DllMain( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved ) {
	mHinst = hinstDLL;
	if ( fdwReason == DLL_PROCESS_ATTACH ) {
		LoadOriginalDll();
		for ( int i = 0; i < 17; i++ )
			mProcs[ i ] = (UINT_PTR)GetProcAddress( mHinstDLL, mImportNames[ i ] );
	} else if ( fdwReason == DLL_PROCESS_DETACH ) {
		FreeLibrary( mHinstDLL );
	}
	return ( TRUE );
}

extern "C" __declspec(naked) void __stdcall GetFileVersionInfoA_wrapper(){__asm{jmp mProcs[0*4]}}
extern "C" __declspec(naked) void __stdcall GetFileVersionInfoByHandle_wrapper(){__asm{jmp mProcs[1*4]}}
extern "C" __declspec(naked) void __stdcall GetFileVersionInfoExA_wrapper(){__asm{jmp mProcs[2*4]}}
extern "C" __declspec(naked) void __stdcall GetFileVersionInfoExW_wrapper(){__asm{jmp mProcs[3*4]}}
extern "C" __declspec(naked) void __stdcall GetFileVersionInfoSizeA_wrapper(){__asm{jmp mProcs[4*4]}}
extern "C" __declspec(naked) void __stdcall GetFileVersionInfoSizeExA_wrapper(){__asm{jmp mProcs[5*4]}}
extern "C" __declspec(naked) void __stdcall GetFileVersionInfoSizeExW_wrapper(){__asm{jmp mProcs[6*4]}}
extern "C" __declspec(naked) void __stdcall GetFileVersionInfoSizeW_wrapper(){__asm{jmp mProcs[7*4]}}
extern "C" __declspec(naked) void __stdcall GetFileVersionInfoW_wrapper(){__asm{jmp mProcs[8*4]}}
extern "C" __declspec(naked) void __stdcall VerFindFileA_wrapper(){__asm{jmp mProcs[9*4]}}
extern "C" __declspec(naked) void __stdcall VerFindFileW_wrapper(){__asm{jmp mProcs[10*4]}}
extern "C" __declspec(naked) void __stdcall VerInstallFileA_wrapper(){__asm{jmp mProcs[11*4]}}
extern "C" __declspec(naked) void __stdcall VerInstallFileW_wrapper(){__asm{jmp mProcs[12*4]}}
extern "C" __declspec(naked) void __stdcall VerLanguageNameA_wrapper(){__asm{jmp mProcs[13*4]}}
extern "C" __declspec(naked) void __stdcall VerLanguageNameW_wrapper(){__asm{jmp mProcs[14*4]}}
extern "C" __declspec(naked) void __stdcall VerQueryValueA_wrapper(){__asm{jmp mProcs[15*4]}}
extern "C" __declspec(naked) void __stdcall VerQueryValueW_wrapper(){__asm{jmp mProcs[16*4]}}


// Loads the original DLL from the default system directory
//	Function originally written by Michael Koch
void LoadOriginalDll()
{
	char buffer[MAX_PATH];

	// Get path to system dir and to version.dll
	GetSystemDirectory(buffer, MAX_PATH);

	// Append DLL name
	strcat_s(buffer, "\\version.dll");

	// Try to load the system's version.dll, if pointer empty
	if (!mHinstDLL) mHinstDLL = LoadLibrary(buffer);

	// Debug
	if (!mHinstDLL)
	{
		OutputDebugString("PROXYDLL: Original version.dll not loaded ERROR ****\r\n");
		ExitProcess(0); // Exit the hard way
	}
}

