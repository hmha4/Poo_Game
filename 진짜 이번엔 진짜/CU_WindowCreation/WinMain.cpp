/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
dxerr9.lib¿Ã∞≈ ∂ßπÆø° æ»µ≈º≠ ª≠
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
            // Render a frame
        } 
    }
    
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
