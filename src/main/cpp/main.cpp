#include <Windows.h>
#include "at_o2xfs_xfs_service_std_win32_Win32MessageLoop.h"
#include "o2xfs-common.h"
#include "o2xfs-logger.h"

static TCHAR szAppName[] = TEXT("XFSWrapper");
o2xfs::Logger LOG;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HWND DoCreateWindow();

// JavaVM *g_vm = 0;
HINSTANCE hInstance;
JNIEnv* g_pEnv = NULL;
jobject g_obj = NULL;
jmethodID g_methodID = NULL;

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
	switch (fdwReason) {
	case DLL_PROCESS_ATTACH:
		hInstance = hinstDLL;
		LOG.setFileName(TEXT("C:\\Temp\\o2xfs-xfs-service-std.log"));
		break;
	}
	return TRUE;
}

//JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
//	LOG.info(TEXT("JNI_OnLoad"));
//	g_vm = vm;
//	return JNI_VERSION_10;
//}

HWND DoCreateWindow() {
	WNDCLASS wndclass;
	wndclass.style = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = NULL;
	wndclass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;

	if (!RegisterClass(&wndclass)) {
		MessageBox(NULL, TEXT("Program requires Windows NT!"), szAppName, MB_ICONERROR);
		return NULL;
	}

	HWND hWnd = CreateWindow(szAppName, TEXT("O2Xfs"), WS_DISABLED, -1, -1, 0, 0, NULL, NULL, hInstance, NULL);
	// HWND hWnd = CreateWindow(szAppName, TEXT("O2Xfs"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
	LOG.info(TEXT("CreateWindow: hWnd: %p"), hWnd);
	// ShowWindow(hWnd, SW_SHOW);
	// UpdateWindow(hWnd);
	return hWnd;
}

JNIEXPORT void JNICALL Java_at_o2xfs_xfs_service_std_win32_Win32MessageLoop_close(JNIEnv* env, jobject obj, jbyteArray hWndObj) {
	HWND hWnd = (HWND) o2xfs::ToPointer(env, hWndObj);
	LOG.info(TEXT("SendMessage: hWnd: %p"), hWnd);
	SendMessage(hWnd, WM_CLOSE, 0, 0);
}

JNIEXPORT void JNICALL Java_at_o2xfs_xfs_service_std_win32_Win32MessageLoop_runLoop(JNIEnv* env, jobject obj, jobject callbackObj) {
	g_pEnv = env;
	g_obj = env->NewGlobalRef(callbackObj);
	g_methodID = env->GetMethodID(env->GetObjectClass(callbackObj), "windowProc", "(I[B)Z");
	jmethodID methodID = env->GetMethodID(env->GetObjectClass(obj), "onWindowCreated", "([B)V");
	if (methodID == NULL || g_methodID == NULL) {
		return;
	}
	HWND hWnd = DoCreateWindow();
	if (hWnd == NULL) {
		return;
	}
	jbyteArray hWndObj = o2xfs::ToArray(env, (LPVOID) hWnd);
	env->CallVoidMethod(obj, methodID, hWndObj);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	if (UnregisterClass(szAppName, hInstance)) {
		LOG.debug(TEXT("UnregisterClass: szAppName=%s"), szAppName);
	} else {
		LOG.error(TEXT("UnregisterClass: szAppName=%s,lastError=%d"), szAppName, GetLastError());
	}
	env->DeleteGlobalRef(g_obj);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	LOG.debug(TEXT("WndProc: hWnd=%p,msg=%d,lParam=%p"), hWnd, msg, lParam);
	if (msg >= WM_USER) {
		jbyteArray lParamObj = o2xfs::ToArray(g_pEnv, (LPVOID) lParam);
		if (g_pEnv->CallBooleanMethod(g_obj, g_methodID, msg, lParamObj)) {
			return 0;
		}
	}
	switch (msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}	
}