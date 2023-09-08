
///
/// D3D 프로젝트
/// 김형환
/// 최초 작성일 2023/09/06
/// 최종 작성일 2023/09/06
/// 
/// D3D 숙달을 위한 프로젝트의 EntryPoint
/// 

#include "framework.h"
#include "D3D.h"
#include "DemoProcess.h"

#define MAX_LOADSTRING 100

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// 프로그램의 최초 진입점(Entry Point). 멀티바이트 프로젝트
int WINAPI wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR	lpCmdLine,
	_In_ int nCmdShow)
{
	WCHAR szAppName[] = L"Game Institute of Technology 2023";
	HWND hWnd;
	MSG	msg;
	WNDCLASS wndclass;

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;

	// 윈도 클래스 등록
	RegisterClass(&wndclass);

	// 윈도 생성
	hWnd = CreateWindow(szAppName,
		szAppName,
		WS_OVERLAPPEDWINDOW,
		100, 100, 800, 600,
		NULL, NULL, hInstance, NULL);

	if (!hWnd) return FALSE;

	// 생성된 윈도를 화면에 표시
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	DemoProcess* demoProcess = new DemoProcess();
	demoProcess->Initialize(hWnd);

	// 메시지 루프
	while (TRUE)
	{
		// 기본 메세지 처리
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;
			DispatchMessage(&msg);

		}
		else
		{
			// 엔진 동작
			demoProcess->Process();
		}
	}

	// 프로그램 종료 전 엔진 반환
	delete demoProcess;

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
	WPARAM wParam, LPARAM lParam)
{
	HDC			hdc;
	PAINTSTRUCT ps;

	switch (message)
	{
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

