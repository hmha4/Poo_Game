#include "stdafx.h"
#include "CFramework.h"


CFramework::CFramework(CBaseApp* pGameApp)
{
	m_pGameApp = pGameApp;
	m_active = TRUE;
	m_pGraphics = new CGraphics();
}

void CFramework::Release()
{
	SAFE_RELEASE(m_pGraphics);
	OnLostDevice();
	OnDestroyDevice();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Creates the window and initializes DirectX Graphics.
Parameters:
[in] szTitle – Title of the window
[in] hInstance – Application instance.
[in] iWidth – Window width.
[in] iHeight – Window height.
[in] bWindowed – Window mode (TRUE). Fullscreen mode (FALSE).
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
BOOL CFramework::Initialize(char* title, HINSTANCE hInstance, int width, int height, BOOL windowed)
{
	m_hInstance = hInstance;
	m_windowWidth = width;
	m_windowHeight = height;

	//Define the window
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_DBLCLKS;
	wcex.lpfnWndProc = (WNDPROC)CFramework::StaticWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CUNIT));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = title;
	wcex.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CUNIT));

	//Register the window
	RegisterClassEx(&wcex);

	// Create the window
	m_hWnd = CreateWindow(title, title, windowed ? WS_OVERLAPPEDWINDOW : WS_EX_TOPMOST, CW_USEDEFAULT,
		0, width, height, NULL, NULL, hInstance, this);

	// Adjust to desired size
	RECT rect = { 0, 0, width, height };
	AdjustWindowRect(&rect, GetWindowLong(m_hWnd, GWL_STYLE), FALSE);
	SetWindowPos(m_hWnd, HWND_TOP, 0, 0, rect.right - rect.left, rect.bottom - rect.top,
		SWP_NOZORDER | SWP_NOMOVE);

	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	// Save current location/size
	ZeroMemory(&m_wp, sizeof(WINDOWPLACEMENT));
	m_wp.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(m_hWnd, &m_wp);

	// Initialize Direct3D
	if (!m_pGraphics->Initialize(m_hWnd, windowed))
	{
		return FALSE;
	}


	OnCreateDevice();
	OnResetDevice();

	return TRUE;
}

void CFramework::Run()
{
	MSG msg;
	while (1)
	{
		//Did we recieve a message, or are we idling?
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//Advance Game Frame
			if (m_pGraphics->GetDevice() != NULL && m_active)
			{
				OnUpdateFrame();
				OnRenderFrame();
			}
		}
	}
}

//Called after the device is created. Create D3DPOOL_MANAGED resources here.
void CFramework::OnCreateDevice()
{
	if (m_pGameApp != NULL && m_pGraphics != NULL)
	{
		m_pGameApp->OnCreateDevice(m_pGraphics->GetDevice());
	}
}

//Called after the device is reset. Create D3DPOOL_DEFAULT resources here.
void CFramework::OnResetDevice()
{
	if (m_pGameApp != NULL && m_pGraphics != NULL)
	{
		m_pGameApp->OnResetDevice(m_pGraphics->GetDevice());
	}
}

//Called when the device is lost. Release D3DPOOL_DEFAULT resources here.
void CFramework::OnLostDevice()
{
	if (m_pGameApp != NULL)
	{
		m_pGameApp->OnLostDevice();
	}
}

//Called after the device is destroyed. Release D3DPOOL_MANAGED resources here.
void CFramework::OnDestroyDevice()
{
	if (m_pGameApp != NULL)
	{
		m_pGameApp->OnDestroyDevice();
	}
}

//Updates the current frame.
void CFramework::OnUpdateFrame()
{
	if (m_pGameApp != NULL && m_pGraphics != NULL)
	{
		m_pGameApp->OnUpdateFrame(m_pGraphics->GetDevice());
	}
}

//Renders the current frame.
void CFramework::OnRenderFrame()
{
	if (!m_active || (m_pGraphics->GetDevice() == NULL))
	{
		return;
	}

	//Check for lost device
	HRESULT result = m_pGraphics->GetDevice()->TestCooperativeLevel();
	if (FAILED(result))
	{
		if (result == D3DERR_DEVICELOST)
		{
			Sleep(50);
			return;
		}
		else
		{
			OnLostDevice();
			if (m_pGraphics->Reset() == D3DERR_DEVICELOST)
			{
				//Device is lost still
				Sleep(50);
				return;
			}
			else
			{
				OnResetDevice();
			}
		}
	}
}

//Toggles between fullscreen and windowed mode.
void CFramework::ToggleFullscreen()
{
	if (m_pGraphics == NULL || !m_active)
	{
		return;
	}

	m_pGraphics->Windowed = !m_pGraphics->Windowed;

	//Set new window style
	if (m_pGraphics->Windowed)
	{
		//Going to windowed mode
		SetWindowLongPtr(m_hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
	}
	else
	{
		//Save Curren location/size
		ZeroMemory(&m_wp, sizeof(WINDOWPLACEMENT));
		m_wp.length = sizeof(WINDOWPLACEMENT);
		GetWindowPlacement(m_hWnd, &m_wp);

		//Going to fullscreen mode
		SetWindowLongPtr(m_hWnd, GWL_STYLE, WS_EX_TOPMOST);

		//Hide the window to avoid animation of blank windows
		ShowWindow(m_hWnd, SW_HIDE);
	}

	//Reset the Device
	OnLostDevice();
	m_pGraphics->Reset();
	OnResetDevice();

	if (m_pGraphics->Windowed)
	{
		//Going to windowed mode
		//Restore the window location/size
		SetWindowPlacement(m_hWnd, &m_wp);
	}

	//Make the window visible
	if (!IsWindowVisible(m_hWnd))
	{
		ShowWindow(m_hWnd, SW_SHOW);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Event handler. Routes messages to appropriate instance.
Parameters:
[in] hWnd – Unique handle to the window.
[in] message – Incoming message.
[in] wParam – Parameter of the message (unsigned int).
[in] lParam – Parameter of the message (long).
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
LRESULT CALLBACK CFramework::StaticWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_CREATE)
	{
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG)((CREATESTRUCT*)lParam)->lpCreateParams);
	}

	CFramework *targetApp = (CFramework*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

	if (targetApp)
	{
		return targetApp->WndProc(hWnd, msg, wParam, lParam);
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Application event handler.
Parameters:
[in] hWnd – Unique handle to the window.
[in] message – Incoming message.
[in] wParam – Parameter of the message (unsigned int).
[in] lParam – Parameter of the message (long).
* * * * * * * * * * * * * * * * * * * * * * * * * */
LRESULT CALLBACK CFramework::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_PAINT:
		if (m_pGraphics->GetDevice())
		{
			OnUpdateFrame();
			OnRenderFrame();
		}
		ValidateRect(hWnd, 0);
		return 0;
		case WM_SIZE:
		if (wParam == SIZE_MINIMIZED)
		{
			//Diable application on minimized
			m_active = FALSE;
		}
		else
		{
			m_active = TRUE;
			m_windowWidth = LOWORD(lParam);
			m_windowHeight = HIWORD(lParam);
			if (m_pGraphics->GetDevice())
			{
				OnLostDevice();
				m_pGraphics->Reset();
				OnResetDevice();
				OnUpdateFrame();
				OnRenderFrame();
			}
		}
		return 0;
		case WM_KEYDOWN:
			//Send keystrokes to application to handle
			if (m_pGameApp != NULL)
			{
				m_pGameApp->OnKeyDown(wParam);
			}
			return 0;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

CFramework::~CFramework()
{
}
