/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
dxerr9.libÀÌ°Å ¶§¹®¿¡ ¾ÈµÅ¼­ »­
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "stdafx.h"
#include "WinMain.h"

#define WINDOW_TITLE "Creating a Window"
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 760

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
    HWND hWnd = CreateWindow( WINDOW_TITLE, WINDOW_TITLE, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, hInstance, 0 );
    
    // Adjust to actual desired size
    RECT rect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
    AdjustWindowRect( &rect, GetWindowLong( hWnd, GWL_STYLE ), FALSE );
    SetWindowPos( hWnd, 0, 0, 0, rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER | SWP_NOMOVE  );

    ShowWindow( hWnd, SW_SHOW );
    UpdateWindow( hWnd );

	//-------------------Create Direct3D9 Object----------------------
	//The Direct3D object lets us configure the application based on the 
	//capabilities of the user¡¯s video card.
	//------------------------------------------------------------------
	LPDIRECT3D9 pD3D9 = Direct3DCreate9(D3D_SDK_VERSION);
	if (!pD3D9)
	{
		MessageBox(0, "Direct3DCreate9() - Failed", 0, 0);
		return 0;
	}

	//---------------Fill out the presentation parameters----------------
	//This structure is used to specify how DirectX is going to behave.
	//---------------------------------------------------------------------
	D3DPRESENT_PARAMETERS D3Dpp;
	ZeroMemory(&D3Dpp, sizeof(D3Dpp));
	D3Dpp.Windowed = TRUE;
	D3Dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

	//------------------Create the Direct3D Device-----------------------
	//If the creation was a success, DirectX Graphics is now initialized 
	//and there is much rejoicing. If device creation fails, we alert the 
	//user and quit the program.
	//------------------------------------------------------------------
	LPDIRECT3DDEVICE9 pD3DDevice = NULL;
	if (FAILED(pD3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING, &D3Dpp,
		&pD3DDevice)))
	{
		MessageBox(0, "CreateDevice() - Failed", 0, 0);
		SAFE_RELEASE(pD3D9);
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
			pD3DDevice->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 100), 1.0f, 0);
			pD3DDevice->BeginScene();
            // Render a frame

			//When done rendering, we need to call EndScene
			pD3DDevice->EndScene();
			pD3DDevice->Present(0, 0, 0, 0);
        } 
    }

	//Give back resources
	SAFE_RELEASE(pD3DDevice);
	SAFE_RELEASE(pD3D9);
    
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
        ValidateRect( hWnd, 0 );
        return 0;
    }

    return DefWindowProc( hWnd, message, wParam, lParam );
}
