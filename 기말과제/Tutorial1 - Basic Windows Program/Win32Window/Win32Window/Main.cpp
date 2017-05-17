//Windows.h include necessary functions for Windows API
#include <Windows.h>

//Prototypes for the GenerateWindow function, otherwise WinMain cannot call GenerateWindow.
bool GenerateWindow(HINSTANCE hInstance, int nCmdShow, LPCSTR className, LPCSTR windowTitle, int width, int height);
bool GenerateWindow(HINSTANCE hInstance, int nCmdShow, LPCSTR className, LPCSTR windowTitle, int x, int y, int width, int height);

//Another prototype
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

//WinMain function, set up window and message loop.
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if(GenerateWindow(hInstance, nCmdShow, "Win32Window", "Window Title", 1280, 720))
	{
		MSG msg;

		while(true)
		{
			while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);

				DispatchMessage(&msg);
			}

			if(msg.message == WM_QUIT) break;
			else
			{
				//UPDATE and DRAW our game
			}
		}

		return msg.wParam;
	}

	return 0;
}

//Generic GenerateWindow function.
bool GenerateWindow(HINSTANCE hInstance, int nCmdShow, LPCSTR className, LPCSTR windowTitle, int x, int y, int width, int height)
{
	HWND hWnd;

	WNDCLASSEX wcex;

	ZeroMemory(&wcex, sizeof(WNDCLASSEX));
	wcex.cbSize = sizeof(wcex);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WindowProc;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wcex.lpszClassName = className;
	wcex.hIconSm = LoadIcon(NULL, IDI_WINLOGO);

	if(!RegisterClassEx(&wcex))
	{
		return false;
	}

	hWnd = CreateWindowEx(NULL, className, windowTitle, WS_OVERLAPPEDWINDOW, x, y, width, height, NULL, NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);

	return true;
}

bool GenerateWindow(HINSTANCE hInstance, int nCmdShow, LPCSTR className, LPCSTR windowTitle, int width, int height)
{
	return GenerateWindow(hInstance, nCmdShow, className, windowTitle, (GetSystemMetrics(SM_CXSCREEN) - width)/2, (GetSystemMetrics(SM_CYSCREEN) - height)/2, width, height);
}

//WindowProc - Handles input sent to the window.
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		} break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}