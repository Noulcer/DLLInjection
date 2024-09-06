#include "stdio.h"
#include "conio.h"
#include "Windows.h"

#define DEF_DLL_NAME "KeyHookDll.dll"
#define DEF_HOOKSTART "HookStart"
#define DEF_HOOKSTOP "HookStop"

// 定义一个函数指针
typedef void(*PFN_HOOKSTART)();
typedef void(*PFN_HOOKSTOP)();

void main() {
	// 定义DLL的句柄
	HMODULE	hDll = NULL;

	// 定义两个函数指针
	PFN_HOOKSTART HookStart = NULL;
	PFN_HOOKSTOP HookStop = NULL;

	// 加载DLL，返回句柄
	hDll = LoadLibraryA(DEF_DLL_NAME);

	// GetProcAddress返回是一个通用的函数指针，必须根据函数定义进行函数指针类型转换
	HookStart = (PFN_HOOKSTART)GetProcAddress(hDll, DEF_HOOKSTART);
	HookStop = (PFN_HOOKSTOP)GetProcAddress(hDll, DEF_HOOKSTOP);

	HookStart();

	printf("press 'q' to quit!\n");
	while (_getch() != 'q');

	HookStop();

	FreeLibrary(hDll);

}