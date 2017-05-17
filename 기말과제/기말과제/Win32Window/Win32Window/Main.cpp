#include <Windows.h>
#include "GraphicsDevice.h"

//Prototypes for the GenerateWindow function, otherwise WinMain cannot call GenerateWindow
bool GenerateWindow(HINSTANCE hInstance, int nCmdShow, LPCSTR className, LPCSTR windowTitle, int width, int height, HWND& hWnd);
bool GenerateWindow(HINSTANCE hInstance, int nCmdShow, LPCSTR className, LPCSTR windowTitle, int x, int y, int width, int height, HWND& hWnd);

//Another prototype
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

//UPDATE and DRAW functions
void Update(float gameTime);
void Draw(GraphicsDevice *gDevice, float gameTime);

//WinMain function, set up window and message loop
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd;

	if (GenerateWindow(hInstance, nCmdShow, "Direct3D", "Initializing Direct3D Example", 1280, 720, hWnd))
	{
		MSG msg;
		GraphicsDevice *gDevice = new GraphicsDevice();
		gDevice->Initialize(hWnd, true);

		while (true)
		{
			while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);

				DispatchMessage(&msg);
			}

			if (msg.message == WM_QUIT) break;
			else
			{
				Update(0.0f);

				Draw(gDevice, 0.0f);
			}
		}

		delete gDevice;

		return msg.wParam;
	}

	return 0;
}

bool GenerateWindow(HINSTANCE hInstance, int nCmdShow, LPCSTR className, LPCSTR windowTitle, int width, int height, HWND& hWnd)
{
	return GenerateWindow(hInstance, nCmdShow, className, windowTitle, (GetSystemMetrics(SM_CXSCREEN) - width) / 2,
		(GetSystemMetrics(SM_CYSCREEN) - height) / 2, width, height, hWnd);
}

//Generic GenerateWindow function
bool GenerateWindow(HINSTANCE hInstance, int nCmdShow, LPCSTR className, LPCSTR windowTitle, int x, int y, int width, int height, HWND& hWnd)
{
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
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wcex))
	{
		return false;
	}

	//Now we are using Direct3D, we need to make sure the window will not be adjusted by the user
	hWnd = CreateWindowEx(NULL, className, windowTitle, WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX, x, y, width, height, NULL, NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);

	return true;
}

//WindowProc - Handles input sent to the window
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	} break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

void Update(float gameTime)
{
	//Update our sprites and other game logic
}

void Draw(GraphicsDevice *gDevice, float gameTime)
{
	//Simple RGB value for the background so use XRGB instead of ARGB
	gDevice->Clear(D3DCOLOR_XRGB(0, 100, 100));
	gDevice->Begin();

	//Draw logic here.

	gDevice->End();
	gDevice->Present();
}