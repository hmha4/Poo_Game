#include "stdafx.h"
#include "GameApp.h"

struct CUSTOMVERTEX
{
	float x, y, z; //  Position in 3D space
	DWORD color;   //  Color
};



CUSTOMVERTEX g_vertices[] =
{
	{ -1.0f, -1.0f, 0.0f, D3DCOLOR_XRGB(255,   0,   0) }, // 0    
	{ -1.0f,  1.0f, 0.0f, D3DCOLOR_XRGB(255, 255,   0) }, // 1    
	{ 1.0f, -1.0f, 0.0f, D3DCOLOR_XRGB(0, 255,   0) }, // 2    
	{ 1.0f,  1.0f, 0.0f, D3DCOLOR_XRGB(0,   0, 255) }  // 3    
};



USHORT g_indices[] = { 0, 1, 2, 1, 3, 2 };



//Transformation variables
float g_Xdirection = 0.001f;
float g_Ydirection = 0.001f;
float g_scale = 0.0002f;

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
	m_VB.CreateBuffer(pDevice, 4, D3DFVF_XYZ | D3DFVF_DIFFUSE, sizeof(CUSTOMVERTEX));
	m_VB.SetData(4, g_vertices);
	m_IB.CreateBuffer(pDevice, 6, D3DFMT_INDEX16);
	m_IB.SetData(6, g_indices);
	m_VB.SetIndexBuffer(&m_IB);
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
	D3DVERTEXELEMENT9 vertexDeclaration[] =
	{
		{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
		D3DDECL_END()
	};

	LPDIRECT3DVERTEXDECLARATION9 _vertexDeclaration = 0;
	pDevice->CreateVertexDeclaration(vertexDeclaration, &_vertexDeclaration);
	pDevice->SetVertexDeclaration(_vertexDeclaration);

	//Set up the render states
	pDevice->SetRenderState(D3DRS_FILLMODE, m_pFramework->GetFillMode());
	pDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//Set the world and view matrices
	D3DXVECTOR3 cameraPosition(0.0f, 0.0f, -10.0f);
	D3DXVECTOR3 cameraTarget(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 cameraUp(0.0f, 1.0f, 0.0f);
	D3DXMATRIX viewMatrix;
	D3DXMatrixLookAtLH(&viewMatrix, &cameraPosition, &cameraTarget, &cameraUp);
	pDevice->SetTransform(D3DTS_VIEW, &viewMatrix);

	//Set the projection matrix
	D3DXMATRIX projection;
	float aspect = (float)m_pFramework->GetWidth() / (float)m_pFramework->GetHeight();
	D3DXMatrixPerspectiveFovLH(&projection, D3DX_PI / 3, aspect, 1.0f, 1000.0f);
	pDevice->SetTransform(D3DTS_PROJECTION, &projection);
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
	m_VB.Release();
	m_IB.Release();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Updates the current frame.
Parameters:
[in] pDevice – Pointer to a DIRECT3DDEVICE9 instance
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CGameApp::OnUpdateFrame(LPDIRECT3DDEVICE9 pDevice)
{
	//Translation
	m_transform.TranslateRel(g_Xdirection, g_Ydirection, 0.0f);
	if (m_transform.GetXPosition() > 3.0f || m_transform.GetXPosition() < -3.0f)
	{
		g_Xdirection *= -1.0f;
	}
	if (m_transform.GetYPosition() > 4.0f || m_transform.GetYPosition() < -4.0f)
	{
		g_Ydirection *= -1.0f;
	}

	//Rotation
	m_transform.RotateRel(0.0f, 0.0f, 0.001f);

	//Scale
	m_transform.ScaleRel(g_scale, g_scale, g_scale);
	if (m_transform.GetXScale() > 2.0f || m_transform.GetXScale() < 0.5f)
	{
		g_scale *= -1.0f;
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * *
Summary: Renders the current frame.
Parameters:
[in] pDevice – Pointer to a DIRECT3DDEVICE9 instance
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CGameApp::OnRenderFrame(LPDIRECT3DDEVICE9 pDevice)
{
	pDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 200), 1.0f, 0);
	pDevice->BeginScene();

	//Render scene here
	pDevice->SetTransform(D3DTS_WORLD, m_transform.GetTransform());
	m_VB.Render(pDevice, 2, D3DPT_TRIANGLELIST);
	/*pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 1, g_triangle, sizeof(CUSTOMVERTEX));*/


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
	if (!pFramework->Initialize("Creating a Framework", hInstance, 640, 480, TRUE))
	{
		return 0;
	}

	//Rock and rool
	pFramework->Run();

	//Clean up resources
	SAFE_RELEASE(pApplication);

	return 0;
}
