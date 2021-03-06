#include <Windows.h>
#include <iostream>

#include "../Bameware Base Internal/Headers/Hooks/BreakpointHooks.h"
#include "../Bameware Base Shared/Headers/Cryptography/StringEncryption.h"
#include <string>

using namespace std;

#ifdef _WIN64
#pragma comment(lib, "Bameware Base External_x64.lib")
#pragma comment(lib, "Bameware Base Internal_x64.lib")
#pragma comment(lib, "Bameware Base Shared_x64.lib")
#else
#pragma comment(lib, "Bameware Base External_x86.lib")
#pragma comment(lib, "Bameware Base Internal_x86.lib")
#pragma comment(lib, "Bameware Base Shared_x86.lib")
#endif

wchar_t *mango(const char* charArray)
{
	wchar_t* wString = new wchar_t[4096];
	MultiByteToWideChar(CP_ACP, 0, charArray, -1, wString, 4096);
	return wString;
}

LPVOID callback()
{
	std::cout << "GetModuleFileNameA call detected from hook" << std::endl;
	return nullptr;
}

int main()
{
	auto debug_manager = BAMEWARE::HOOKS::BreakpointHookManager();

	const auto thread_ptr = GetProcAddress(
		GetModuleHandle(mango("kernel32.dll")),
		ENCRYPT_STRING("GetModuleFileNameA"));

	if (debug_manager.Hook(FARPROC(thread_ptr), FARPROC(callback)))
		std::cout << "Hooked GetModuleFileNameA to callback" << std::endl;
	else
		std::cout << "Failed to hook GetModuleFileNameA to callback" << std::endl;

	std::cout << "Press 'q' to quit the application, press 'enter' to call GetModuleFileNameA." << std::endl;

	int c;
	do
	{
		c = getchar();

		static char buffer[MAX_PATH];
		GetModuleFileNameA(NULL, buffer, MAX_PATH);
		std::cout << "GetModuleFileNameA called" << std::endl;
	} while (c != 'q');

	return 0;
}
