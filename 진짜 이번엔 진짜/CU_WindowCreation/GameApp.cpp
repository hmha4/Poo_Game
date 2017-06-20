#include "stdafx.h"
#include "GameApp.h"

char * g_sometext = "Look how lovely all this text is. I think I could just writhe text all day. Did you know that if you cross a bull dog and a shih tzu, you get a bull shiht? Ok, that wasn't very good. What can I say? I've been writing tutorials all day.\nLook!\nA\nline\nbreak\nis\nused\nhere!"; 
char* g_instructions = "Esc: Quit\r\nF5: Toggle fullscreen\r\nF6: Toggle wireframe";

CGameApp::CGameApp()
{
	m_pFramework = NULL;
	m_pTexture = NULL;
	m_pTextMesh = NULL;
	m_showInstructions = FALSE;
}

//Clean up resources
void CGameApp::Release()
{
	SAFE_RELEASE(m_pFramework);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Sets the CFramework instnace of the application.
Parameters:
[in] pFramework – Pointer to a CFramework instance
* * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CGameApp::SetFramework(CFramework* pFramework)
{
	m_pFramework = pFramework;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
Summary: Initialize application-specific resources and states here.
Returns: TRUE on success, FALSE on failure
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * ** */
BOOL CGameApp::Initialize()
{
	//Create the light
	ZeroMemory(&m_light, sizeof(D3DLIGHT9));
	m_light.Type = D3DLIGHT_DIRECTIONAL;;
	m_light.Diffuse.r = 1.0f;
	m_light.Diffuse.g = 1.0f;
	m_light.Diffuse.b = 1.0f;
	m_light.Direction.x = -1.0f;
	m_light.Direction.y = -1.0f;
	m_light.Direction.z = 1.0f;
	return TRUE;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary:
This callback function will be called immediately after the Direct3D 
device has been created. This is
the best location to create D3DPOOL_MANAGED resources. Resources 
created here should be released in
the OnDestroyDevice callback.
Parameters:
[in] pDevice – Pointer to a DIRECT3DDEVICE9 instance
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
void CGameApp::OnCreateDevice(LPDIRECT3DDEVICE9 pDevice)
{
	//Create sprite for batching text calls
	D3DXCreateSprite(pDevice, &m_pTextSprite);

	//Create 2D text
	m_font.Initialize(pDevice, "Arial", 12);

	//Create 3D text
	SAFE_RELEASE(m_pTextMesh);
	HFONT hFont = CreateFont(0, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial");

	HDC hdc = CreateCompatibleDC(NULL);

	//Save the old font
	HFONT hFontOld = (HFONT)SelectObject(hdc, hFont);

	//Create the text mesh
	if (FAILED(D3DXCreateText(pDevice, hdc, "C_Unit", 0.01f, 0.4f, &m_pTextMesh, NULL, NULL)))
	{
		SHOWERROR("D3DXCreateText() - Failed.", __FILE__, __LINE__);
	}

	//Restore the old font and clean up
	SelectObject(hdc, hFontOld);
	DeleteObject(hFont);
	DeleteDC(hdc);

	
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
Summary:
This callback function will be called immediately after the Direct3D
device has been created. This is
the best location to create D3DPOOL_DEFAULT resources. Resources 
created here should be released in
the OnLostDevice callback.
Parameters:
[in] pDevice – Pointer to a DIRECT3DDEVICE9 instance
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CGameApp::OnResetDevice(LPDIRECT3DDEVICE9 pDevice)
{
	m_pTextSprite->OnResetDevice();
	m_font.OnResetDevice();

	// Set transforms
	D3DXVECTOR3 cameraPosition(0.0f, 2.0f, -5.0f);
	D3DXVECTOR3 cameraTarget(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 cameraUp(0.0f, 1.0f, 0.0f);
	D3DXMATRIX viewMatrix;
	D3DXMatrixLookAtLH(&viewMatrix, &cameraPosition, &cameraTarget, &cameraUp);
	pDevice->SetTransform(D3DTS_VIEW, &viewMatrix);
	D3DXMATRIX projection;
	float aspect = (float)m_pFramework->GetWidth() / (float)m_pFramework->GetHeight();
	D3DXMatrixPerspectiveFovLH(&projection, D3DX_PI / 3.0f, aspect, 0.1f, 1000.0f);
	pDevice->SetTransform(D3DTS_PROJECTION, &projection);

	//Set up the render states
	pDevice->SetRenderState(D3DRS_FILLMODE, m_pFramework->GetFillMode());
	pDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	pDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(80, 80, 80));
	pDevice->LightEnable(0, TRUE);
	pDevice->SetLight(0, &m_light);

	//Set a material
	D3DMATERIAL9 material;
	ZeroMemory(&material, sizeof(D3DMATERIAL9));
	material.Diffuse.r = material.Ambient.r = 0.4f;
	material.Diffuse.g = material.Ambient.g = 0.7f;
	material.Diffuse.b = material.Ambient.b = 1.0f;
	material.Diffuse.a = material.Ambient.a = 1.0f;
	pDevice->SetMaterial(&material);

	pDevice->SetTexture(0, m_pTexture);
}

/* * * * * * * * * * * * * * * ** * * * * * * * * * * * * * * * * *
Summary:
This callback function will be called immediately after the Direct3D
device has entered a lost state
and before IDirect3DDevice9::Reset is called. Resources created in
the OnResetDevice callback should
be released here, which generally includes all D3DPOOL_DEFAULT resources.
* * * * * * * * * * * * * * * * * *  * * * * * * * * * * * * * * * */
void CGameApp::OnLostDevice()
{
	m_pTextSprite->OnLostDevice();
	m_font.OnLostDevice();
}

/* * * * * * * * * *  * * * * * * * * * * * * * * * * * * * * * * * *
Summary:
This callback function will be called immediately after the Direct3D
device has been destroyed.
Resources created in the OnCreateDevice callback should be released 
here, which generally includes
all D3DPOOL_MANAGED resources.
* * * * * * * * * * ** * * * * * * * * * * * * * * * * * * * * * * */
void CGameApp::OnDestroyDevice()
{
	SAFE_RELEASE(m_pTextSprite);
	SAFE_RELEASE(m_pTexture);
	m_font.Release();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Updates the current frame.
Parameters:
[in] pDevice – Pointer to a DIRECT3DDEVICE9 instance
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CGameApp::OnUpdateFrame(LPDIRECT3DDEVICE9 pDevice, float elapsedTime)
{
	//Move the light back and forth
	static float xVelocity = 3.0f;
	float x = m_light.Position.x;
	x += xVelocity * elapsedTime;
	if (x < -4.0f || x > 4.0f)
	{
		x -= xVelocity * elapsedTime;
		xVelocity = -xVelocity;
	}
	m_light.Position.x = x;
	m_light.Position.y = 3.0f;
	m_light.Position.z = -5.0f;

		//Set the light to index 0 on the device
	pDevice->SetLight(0, &m_light);
	
	m_transform.SetXPosition(-1.0f);
	m_transform.RotateRel(D3DXToRadian(-90.0f) * elapsedTime, 0.0f, 0.0f);
	pDevice->SetTransform(D3DTS_WORLD, m_transform.GetTransform());
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * *
Summary: Renders the current frame.
Parameters:
[in] pDevice – Pointer to a DIRECT3DDEVICE9 instance
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CGameApp::OnRenderFrame(LPDIRECT3DDEVICE9 pDevice, float elapsedTime)
{
	sprintf(m_fps, "%.2f fps", m_pFramework->GetFPS());

	pDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	pDevice->BeginScene();

	//Render scene here
	m_font.Print("Printing text without a sprite like\r\nthis slows things down a bit.",
		m_pFramework->GetWidth() - 300, m_pFramework->GetHeight() - 100, D3DCOLOR_XRGB(0, 200, 0));

	m_pTextSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
	m_font.Print(g_sometext, 50, 100, D3DCOLOR_XRGB(255, 0, 0), m_pTextSprite, 200, 0, FA_CENTER);
	m_font.Print("We can even align a bunch of text along the right side. Isn't that neat?",
		m_pFramework->GetWidth() - 110, m_pFramework->GetHeight() - (m_pFramework->GetHeight() / 2) - 30,
		D3DCOLOR_XRGB(200, 200, 255), m_pTextSprite, 100, 0, FA_RIGHT);

	// Display framerate and instructions
	m_font.Print(m_fps, 5, 5, D3DCOLOR_XRGB(255, 0, 0), m_pTextSprite);
	if (m_showInstructions)
	{
		m_font.Print(g_instructions, 5, 20, D3DCOLOR_XRGB(255, 255, 255), m_pTextSprite);
	}
	else
	{
		m_font.Print("Hit F1 to view the instructions.", 5, 20, D3DCOLOR_XRGB(255, 255, 255), m_pTextSprite);
	}

	m_pTextSprite->End();

	// Render 3D text
	if (m_pTextMesh)
	{
		m_pTextMesh->DrawSubset(0);
	}

	
	

	pDevice->EndScene();
	pDevice->Present(0, 0, 0, 0);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Responds to key presses
Parameters:
[in] wParam – Key down argument
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CGameApp::OnKeyDown(WPARAM wParam)
{
	switch (wParam)
	{
	case VK_ESCAPE:
		PostQuitMessage(0);
		break;
	case VK_F1:
		m_showInstructions = !m_showInstructions;
		break;
	case VK_F5:
		if (m_pFramework != NULL)
		{
			m_pFramework->ToggleFullscreen();
		}
		break;
	case VK_F6:
		if (m_pFramework != NULL)
		{
			m_pFramework->ToggleWireframe();
		}
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * *
Summary: Application entry point
Parameters:
[in] hInstance – Application instance
[in] hPrevInstance – Junk
[in] lpCmdLine – Command line arguments
[in] nCmdShow – Window display flags
* * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	CGameApp* pApplication = new CGameApp();
	CFramework* pFramework = new CFramework((CBaseApp*)pApplication);

	pApplication->SetFramework(pFramework);

	//Initalize any application resources
	if (!pApplication->Initialize())
	{
		return 0;
	}

	//Initialize the Framework
	if (!pFramework->Initialize("Lighting", hInstance, 640, 480, TRUE))
	{
		return 0;
	}

	//Rock and rool
	pFramework->Run();

	//Clean up resources
	SAFE_RELEASE(pApplication);

	return 0;
}
