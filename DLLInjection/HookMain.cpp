#include "stdio.h"
#include "conio.h"
#include "Windows.h"

#define DEF_DLL_NAME "KeyHookDll.dll"
#define DEF_HOOKSTART "HookStart"
#define DEF_HOOKSTOP "HookStop"

// ����һ������ָ��
typedef void(*PFN_HOOKSTART)();
typedef void(*PFN_HOOKSTOP)();

void main() {
	// ����DLL�ľ��
	HMODULE	hDll = NULL;

	// ������������ָ��
	PFN_HOOKSTART HookStart = NULL;
	PFN_HOOKSTOP HookStop = NULL;

	// ����DLL�����ؾ��
	hDll = LoadLibraryA(DEF_DLL_NAME);

	// GetProcAddress������һ��ͨ�õĺ���ָ�룬������ݺ���������к���ָ������ת��
	HookStart = (PFN_HOOKSTART)GetProcAddress(hDll, DEF_HOOKSTART);
	HookStop = (PFN_HOOKSTOP)GetProcAddress(hDll, DEF_HOOKSTOP);

	HookStart();

	printf("press 'q' to quit!\n");
	while (_getch() != 'q');

	HookStop();

	FreeLibrary(hDll);

}