#include "stdafx.h"
#include "GameApp.h"

struct CUSTOMVERTEX
{
	float x, y, z; //  Position in 3D space
	DWORD color;   //  Color
	float u, v;    //  Texture coordinates
};

CUSTOMVERTEX g_vertices[] =

{


	{ -1.0f,  1.0f, -1.0f, D3DCOLOR_XRGB(255, 255, 255), 0.0f, 0.0f },

	{ 1.0f,  1.0f, -1.0f, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0.0f },

	{ 1.0f, -1.0f, -1.0f, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 1.0f },

	{ -1.0f,  1.0f, -1.0f, D3DCOLOR_XRGB(255, 255, 255), 0.0f, 0.0f },

	{ 1.0f, -1.0f, -1.0f, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 1.0f },

	{ -1.0f, -1.0f, -1.0f, D3DCOLOR_XRGB(255, 255, 255), 0.0f, 1.0f },

	{ 1.0f,  1.0f, -1.0f, D3DCOLOR_XRGB(255, 255, 255), 0.0f, 0.0f },

	{ 1.0f,  1.0f,  1.0f, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0.0f },

	{ 1.0f, -1.0f,  1.0f, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 1.0f },

	{ 1.0f,  1.0f, -1.0f, D3DCOLOR_XRGB(255, 255, 255), 0.0f, 0.0f },

	{ 1.0f, -1.0f,  1.0f, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 1.0f },

	{ 1.0f, -1.0f, -1.0f, D3DCOLOR_XRGB(255, 255, 255), 0.0f, 1.0f },


	{ 1.0f,  1.0f,  1.0f, D3DCOLOR_XRGB(255, 255, 255), 0.0f, 0.0f },

	{ -1.0f,  1.0f,  1.0f, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0.0f },

	{ -1.0f, -1.0f,  1.0f, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 1.0f },

	{ 1.0f,  1.0f,  1.0f, D3DCOLOR_XRGB(255, 255, 255), 0.0f, 0.0f },

	{ -1.0f, -1.0f,  1.0f, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 1.0f },

	{ 1.0f, -1.0f,  1.0f, D3DCOLOR_XRGB(255, 255, 255), 0.0f, 1.0f },


	{ -1.0f,  1.0f,  1.0f, D3DCOLOR_XRGB(255, 255, 255), 0.0f, 0.0f },

	{ -1.0f,  1.0f, -1.0f, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0.0f },

	{ -1.0f, -1.0f, -1.0f, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 1.0f },

	{ -1.0f,  1.0f,  1.0f, D3DCOLOR_XRGB(255, 255, 255), 0.0f, 0.0f },

	{ -1.0f, -1.0f, -1.0f, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 1.0f },

	{ -1.0f, -1.0f,  1.0f, D3DCOLOR_XRGB(255, 255, 255), 0.0f, 1.0f },


	{ -1.0f,  1.0f,  1.0f, D3DCOLOR_XRGB(0,   0, 255), 0.0f, 0.0f },

	{ 1.0f,  1.0f,  1.0f, D3DCOLOR_XRGB(0, 255,   0), 1.0f, 0.0f },

	{ 1.0f,  1.0f, -1.0f, D3DCOLOR_XRGB(255,   0,   0), 1.0f, 1.0f },

	{ -1.0f,  1.0f,  1.0f, D3DCOLOR_XRGB(0,   0, 255), 0.0f, 0.0f },

	{ 1.0f,  1.0f, -1.0f, D3DCOLOR_XRGB(255,   0,   0), 1.0f, 1.0f },

	{ -1.0f,  1.0f, -1.0f, D3DCOLOR_XRGB(255, 255,   0), 0.0f, 1.0f },


	{ -1.0f, -1.0f, -1.0f, D3DCOLOR_XRGB(255, 255, 255), 0.0f, 0.0f },

	{ 1.0f, -1.0f, -1.0f, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0.0f },

	{ 1.0f, -1.0f,  1.0f, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 1.0f },

	{ -1.0f, -1.0f, -1.0f, D3DCOLOR_XRGB(255, 255, 255), 0.0f, 0.0f },

	{ 1.0f, -1.0f,  1.0f, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 1.0f },

	{ -1.0f, -1.0f,  1.0f, D3DCOLOR_XRGB(255, 255, 255), 0.0f, 1.0f }


};


CGameApp::CGameApp()
{
	m_pFramework = NULL;
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
	//Create the buffer
	m_VB.CreateBuffer(pDevice, 36, D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1, sizeof(CUSTOMVERTEX));
	m_VB.SetData(36, g_vertices, 0);

	//Load the texture
	char texture[MAX_PATH] = { 0 };
	CUtility::GetMediaFile("panel.jpg", texture);
	if (!texture)
	{
		SHOWERROR("Unable to find texture file.", __FILE__, __LINE__);
	}
	if (FAILED(D3DXCreateTextureFromFile(pDevice, texture, &m_pTexture)))
	{
		SHOWERROR("Unable to create texture.", __FILE__, __LINE__);
	}
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
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	
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
	m_VB.Release();
	SAFE_RELEASE(m_pTexture);
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

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Updates the current frame.
Parameters:
[in] pDevice – Pointer to a DIRECT3DDEVICE9 instance
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CGameApp::OnUpdateFrame(LPDIRECT3DDEVICE9 pDevice, float elapsedTime)
{
	//Create the new vertices
	m_transform.RotateRel(0.0f, 3.14f * elapsedTime, 0.0f);
	
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * *
Summary: Renders the current frame.
Parameters:
[in] pDevice – Pointer to a DIRECT3DDEVICE9 instance
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CGameApp::OnRenderFrame(LPDIRECT3DDEVICE9 pDevice, float elapsedTime)
{
	pDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 200), 1.0f, 0);
	pDevice->BeginScene();

	//Render scene here
	pDevice->SetTransform(D3DTS_WORLD, m_transform.GetTransform());
	m_VB.Render(pDevice, 12, D3DPT_TRIANGLELIST);

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
		if (m_pFramework != NULL)
		{
			m_pFramework->ToggleFullscreen();
		}
		break;
	case VK_F2:
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
	if (!pFramework->Initialize("Textures", hInstance, 640, 480, TRUE))
	{
		return 0;
	}

	//Rock and rool
	pFramework->Run();

	//Clean up resources
	SAFE_RELEASE(pApplication);

	return 0;
}
