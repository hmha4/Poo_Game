/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
dxerr9.libÀÌ°Å ¶§¹®¿¡ ¾ÈµÅ¼­ »­
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "stdafx.h"
#include "WinMain.h"

#define WINDOW_TITLE "Improved DirectX Initializtion"
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 760
#define WINDOWED TRUE

LPDIRECT3D9 g_pD3D9 = NULL;
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;
D3DDISPLAYMODE g_displayMode;
D3DPRESENT_PARAMETERS g_D3Dpp;
HWND g_hWnd;

BOOL BuildPresentationParameters();
void RenderFrame();

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR cmdLine, int cmdShow )
{ 
	// Define the window
    WNDCLASSEX wcex;
    wcex.cbSize         = sizeof( WNDCLASSEX ); 
    wcex.style          = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon( hInstance, MAKEINTRESOURCE( IDI_CUNIT ) );
    wcex.hCursor        = LoadCursor( hInstance, IDC_ARROW );
    wcex.hbrBackground  = (HBRUSH)GetStockObject( WHITE_BRUSH );
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = WINDOW_TITLE;
    wcex.hIconSm        = LoadIcon( hInstance, MAKEINTRESOURCE( IDI_CUNIT ) );

	// Register the window
    RegisterClassEx( &wcex );

	// Create the window
    g_hWnd = CreateWindow( WINDOW_TITLE, WINDOW_TITLE, 
		                      (WINDOWED) ? WS_OVERLAPPEDWINDOW 
		                                 : WS_EX_TOPMOST, 
		                      CW_USEDEFAULT, CW_USEDEFAULT, 
		                      WINDOW_WIDTH, WINDOW_HEIGHT, 
		                      0, 0, hInstance, 0 );
    
    // Adjust to actual desired size
    RECT rect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
    AdjustWindowRect( &rect, GetWindowLong( g_hWnd, GWL_STYLE ), FALSE );
    SetWindowPos( g_hWnd, 0, 0, 0, rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER | SWP_NOMOVE  );

    ShowWindow( g_hWnd, SW_SHOW );
    UpdateWindow( g_hWnd );

	//-------------------Create Direct3D Object----------------------
	//The Direct3D object lets us configure the application based on the 
	//capabilities of the user¡¯s video card.
	//------------------------------------------------------------------
	g_pD3D9 = Direct3DCreate9(D3D_SDK_VERSION);
	if (!g_pD3D9)
	{
		MessageBox(0, "Direct3DCreate9() - Failed", 0, 0);
		return 0;
	}

	//------------------------Get display mode---------------------------
	g_pD3D9->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &g_displayMode);

	//-------------------Check for hardware T&L---------------------------
	D3DCAPS9 D3DCaps;
	g_pD3D9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &D3DCaps);
	DWORD vertexProcessing = 0;
	if (D3DCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		vertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
		//Check for pure device
		if (D3DCaps.DevCaps & D3DDEVCAPS_PUREDEVICE)
		{
			vertexProcessing |= D3DCREATE_PUREDEVICE;
		}
	}
	else
	{
		vertexProcessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	//---------------Fill out the presentation parameters----------------
	//This structure is used to specify how DirectX is going to behave.
	//---------------------------------------------------------------------
	if (!BuildPresentationParameters())
	{
		MessageBox(0, "Unable to find a valid DepthStencil Format", 0, 0);
		SAFE_RELEASE(g_pD3D9);
		return 0;
	}

	//------------------Create the Direct3D Device-----------------------
	//If the creation was a success, DirectX Graphics is now initialized 
	//and there is much rejoicing. If device creation fails, we alert the 
	//user and quit the program.
	//------------------------------------------------------------------
	if (FAILED(g_pD3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hWnd,
		vertexProcessing, &g_D3Dpp, &g_pD3DDevice)))
	{
		SAFE_RELEASE(g_pD3D9);
		MessageBox(0, "CreateDevice() - Failed", 0, 0);
		
		return 0;
	}

	

    // Main loop
    MSG msg;
    while ( 1 ) 
    {
        // Only render when there are no messages
        if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) 
        {
            if ( msg.message == WM_QUIT)
            {
                break;
            }
            TranslateMessage( &msg );
            DispatchMessage ( &msg );
        } 
        else 
        {
			//To Render the scene, we first clear the surface we are drawing
			//on, called the back buffer
			RenderFrame();
        } 
    }

	//Give back resources
	SAFE_RELEASE(g_pD3DDevice);
	SAFE_RELEASE(g_pD3D9);
    
	return 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Application event handler.
Parameters:
    [in] hWnd - Unique handle to the window.
    [in] message - Incoming message.
    [in] wParam - Parameter of the message (unsigned int).
    [in] lParam - Parameter of the message (long).
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    switch ( message ) 
    {
    case WM_DESTROY:
        PostQuitMessage( 0 );
        return 0;
    case WM_PAINT:
        // Render a frame then validate the client area
		if (g_pD3DDevice)
		{
			RenderFrame();
		}
        ValidateRect( hWnd, 0 );
        return 0;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}
		return 0;
    }

    return DefWindowProc( hWnd, message, wParam, lParam );
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Purpose:
Builds the D3DPRESENT_PARAMETERS structure using the current window size.
Returns: TRUE on success. FALSE if a valid depth format cannot be found.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
BOOL BuildPresentationParameters() {
	ZeroMemory(&g_D3Dpp, sizeof(g_D3Dpp));
	D3DFORMAT adapterFormat = (WINDOWED) ? g_displayMode.Format : D3DFMT_X8R8G8B8;
	if (SUCCEEDED(g_pD3D9->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, adapterFormat,
		D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24S8))) {
		g_D3Dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	}
	else if (SUCCEEDED(g_pD3D9->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, adapterFormat,
		D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24X8))) {
		g_D3Dpp.AutoDepthStencilFormat = D3DFMT_D24X8;
	}
	else if (SUCCEEDED(g_pD3D9->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, adapterFormat,
		D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D16))) {
		g_D3Dpp.AutoDepthStencilFormat = D3DFMT_D16;
	}
	else {
		return false;
	}

	g_D3Dpp.BackBufferWidth = (WINDOWED) ? 0 : g_displayMode.Width;
	g_D3Dpp.BackBufferHeight = (WINDOWED) ? 0 : g_displayMode.Height;
	g_D3Dpp.BackBufferFormat = adapterFormat;
	g_D3Dpp.BackBufferCount = 1;
	g_D3Dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	g_D3Dpp.MultiSampleQuality = 0;
	g_D3Dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	g_D3Dpp.hDeviceWindow = g_hWnd;
	g_D3Dpp.Windowed = WINDOWED;
	g_D3Dpp.EnableAutoDepthStencil = TRUE;
	g_D3Dpp.FullScreen_RefreshRateInHz = (WINDOWED) ? 0 : g_displayMode.RefreshRate;
	g_D3Dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	return TRUE;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Renders the current frame.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void RenderFrame()
{
	g_pD3DDevice->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 100), 1.0f, 0);
	g_pD3DDevice->BeginScene();

	// Render the frame here

	g_pD3DDevice->EndScene();
	g_pD3DDevice->Present(0, 0, 0, 0);
}
