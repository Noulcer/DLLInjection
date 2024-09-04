#include "stdio.h"
#include "Windows.h"
#include "pch.h"

#define DEF_PROCESS_NAME "notepad.exe"

// HINSTANCE和HMODULE几乎没什么区别
HINSTANCE g_hInstance = NULL;
HHOOK g_hHook = NULL;
// 窗口的句柄
HWND g_hWnd = NULL;

// 加载DLL时如果附着一些其他的初始化动作，就需要DLLmain
// WINAPI是一个宏，定义函数调用约定 __stdcall
// DllMain的参数由系统传递
// dwReason表明是在DLL有什么动作时执行
// LPVOID代表Long Pointer to VOID，指向任何类型数据的指针，typedef void* LPVOID
// lpvReversed表明dll加载或卸载的信息，不用太关注。
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD dwReason, LPVOID lpvReversed) {
	switch ( dwReason ) {
	// dll加载时执行
	case DLL_PROCESS_ATTACH:
		g_hInstance = hinstDLL;
		break;

	case DLL_PROCESS_DETACH:
		break;

	default:
		break;
	}
	return TRUE;
}


// LRESULT通常用来表示回调函数返回值
// CALLBACK同样是规定函数调用约定
// nCode表示键盘消息的处理状态（键盘消息在传递/键盘无动作）
// wParam按键的虚拟键值
// lParam附加的关于键盘事件的详细信息，不用太关注
// wParam与lParam一般组合使用，w传递简单数据，l传递更详细的数据
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
	// C语言中显示初始化某些元素，其他元素会被设置为0
	char szPath[MAX_PATH] = { 0, };
	char *p = NULL;

	// nCode>=0，表示有键盘消息传递
	if (nCode >= 0) {
		// 判断lparam最高位是否为1，如果为1说明是扩展键（shift、ctrl等）
		if (!(lParam & 0x8000000)) {
			// 获取指定模块的路径名，如果为NULL代表获取当前可执行文件
			GetModuleFileNameA(NULL, szPath, MAX_PATH);
			// string right character 寻找最后一个\（转义）
			p = strrchr(szPath, '\\');

			// 忽略大小写对比
			if (!_stricmp(p + 1, DEF_PROCESS_NAME)) {
				return 1;
			}
		}
	}
	// 调用后续过程，Ex代表Extend，扩展版函数
	return CallNextHookEx(g_hHook, nCode, wParam, lParam);
}

// if define 判断某个宏是否得到定义
// 根据操作系统和编译器的不同进行选择性编译
#ifdef __cplusplus
// 标记用C的方式处理函数名
extern "C" {
#endif
	// declaration specifier （dllexport）标记导出
	__declspec(dllexport) void HookStart() {
		// 键盘钩子
		// 处理钩子过程的函数
		// 处理钩子过程的函数的模块的句柄
		// 全局钩子
		g_hHook = SetWindowsHookEx(WH_KEYBOARD, KeyboardProc, g_hInstance, 0);
	}
	__declspec(dllexport) void HookStop() {
		if (g_hHook) {
			UnhookWindowsHookEx(g_hHook);
			g_hHook = NULL;
		}
	}
#ifdef __cplusplus
}
#endif
