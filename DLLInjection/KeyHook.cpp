#include "stdio.h"
#include "Windows.h"
#include "pch.h"

#define DEF_PROCESS_NAME "notepad.exe"

// HINSTANCE��HMODULE����ûʲô����
HINSTANCE g_hInstance = NULL;
HHOOK g_hHook = NULL;
// ���ڵľ��
HWND g_hWnd = NULL;

// ����DLLʱ�������һЩ�����ĳ�ʼ������������ҪDLLmain
// WINAPI��һ���꣬���庯������Լ�� __stdcall
// DllMain�Ĳ�����ϵͳ����
// dwReason��������DLL��ʲô����ʱִ��
// LPVOID����Long Pointer to VOID��ָ���κ��������ݵ�ָ�룬typedef void* LPVOID
// lpvReversed����dll���ػ�ж�ص���Ϣ������̫��ע��
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD dwReason, LPVOID lpvReversed) {
	switch ( dwReason ) {
	// dll����ʱִ��
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


// LRESULTͨ��������ʾ�ص���������ֵ
// CALLBACKͬ���ǹ涨��������Լ��
// nCode��ʾ������Ϣ�Ĵ���״̬��������Ϣ�ڴ���/�����޶�����
// wParam�����������ֵ
// lParam���ӵĹ��ڼ����¼�����ϸ��Ϣ������̫��ע
// wParam��lParamһ�����ʹ�ã�w���ݼ����ݣ�l���ݸ���ϸ������
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
	// C��������ʾ��ʼ��ĳЩԪ�أ�����Ԫ�ػᱻ����Ϊ0
	char szPath[MAX_PATH] = { 0, };
	char *p = NULL;

	// nCode>=0����ʾ�м�����Ϣ����
	if (nCode >= 0) {
		// �ж�lparam���λ�Ƿ�Ϊ1�����Ϊ1˵������չ����shift��ctrl�ȣ�
		if (!(lParam & 0x8000000)) {
			// ��ȡָ��ģ���·���������ΪNULL�����ȡ��ǰ��ִ���ļ�
			GetModuleFileNameA(NULL, szPath, MAX_PATH);
			// string right character Ѱ�����һ��\��ת�壩
			p = strrchr(szPath, '\\');

			// ���Դ�Сд�Ա�
			if (!_stricmp(p + 1, DEF_PROCESS_NAME)) {
				return 1;
			}
		}
	}
	// ���ú������̣�Ex����Extend����չ�溯��
	return CallNextHookEx(g_hHook, nCode, wParam, lParam);
}

// if define �ж�ĳ�����Ƿ�õ�����
// ���ݲ���ϵͳ�ͱ������Ĳ�ͬ����ѡ���Ա���
#ifdef __cplusplus
// �����C�ķ�ʽ��������
extern "C" {
#endif
	// declaration specifier ��dllexport����ǵ���
	__declspec(dllexport) void HookStart() {
		// ���̹���
		// �����ӹ��̵ĺ���
		// �����ӹ��̵ĺ�����ģ��ľ��
		// ȫ�ֹ���
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
