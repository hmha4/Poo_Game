#include "stdafx.h"
#include "GameApp.h"

struct CUSTOMVERTEX
{
	float x, y, z; //  Position in 3D space
	DWORD color;   //  Color
};

D3DXVECTOR3* g_positions;
D3DXVECTOR3* g_speeds;


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
	g_positions = new D3DXVECTOR3[1000];
	g_speeds = new D3DXVECTOR3[1000];

	//Seed random number generator
	srand((UINT)time(NULL));
	for (int i = 0; i < 1000; i++)
	{
		g_speeds[i].x = ((float)rand() / RAND_MAX * 1000.0f - 500.0f) / 50.0f;
		g_speeds[i].y = ((float)rand() / RAND_MAX * 1000.0f - 500.0f) / 50.0f;
		g_speeds[i].z = 0.0f;

		g_positions[i].x = g_positions[i].y = g_positions[i].z = 0.0f;
	}

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

	/*D3DVERTEXELEMENT9 vertexDeclaration[] =
	{
		{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
		D3DDECL_END()
	};

	LPDIRECT3DVERTEXDECLARATION9 _vertexDeclaration = 0;
	pDevice->CreateVertexDeclaration(vertexDeclaration, &_vertexDeclaration);
	pDevice->SetVertexDeclaration(_vertexDeclaration);*/

	// Create the buffer
	m_VB.CreateBuffer(pDevice, 1000, D3DFVF_XYZ | D3DFVF_DIFFUSE, sizeof(CUSTOMVERTEX), TRUE);

	// Set transforms
	pDevice->SetTransform(D3DTS_WORLD, m_transform.GetTransform());
	D3DXVECTOR3 cameraPosition(0.0f, 0.0f, -10.0f);
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
	float pointSize = 3.0f;
	pDevice->SetRenderState(D3DRS_POINTSIZE, *((DWORD*)&pointSize));

	//Set the world and view matrices
	//D3DXVECTOR3 cameraPosition(0.0f, 0.0f, -10.0f);
	//D3DXVECTOR3 cameraTarget(0.0f, 0.0f, 0.0f);
	//D3DXVECTOR3 cameraUp(0.0f, 1.0f, 0.0f);
	//D3DXMATRIX viewMatrix;
	//D3DXMatrixLookAtLH(&viewMatrix, &cameraPosition, &cameraTarget, &cameraUp);
	//pDevice->SetTransform(D3DTS_VIEW, &viewMatrix);

	////Set the projection matrix
	//D3DXMATRIX projection;
	//float aspect = (float)m_pFramework->GetWidth() / (float)m_pFramework->GetHeight();
	//D3DXMatrixPerspectiveFovLH(&projection, D3DX_PI / 3, aspect, 1.0f, 1000.0f);
	//pDevice->SetTransform(D3DTS_PROJECTION, &projection);
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
	CUSTOMVERTEX verts[1000];
	for (int i = 0; i < 1000; i++)
	{
		g_positions[i] += g_speeds[i] * elapsedTime;
		//Constrain points
		if(g_positions[i].x < -7.0f || g_positions[i].x > 7.0f || g_positions[i].y < -5.0f || g_positions[i].y > 5.0f)
		{
			g_positions[i].x = 0.0f;
			g_positions[i].y = 0.0f;
		}
		//Form colors based on distance from origin
		//Lots of parabola shift and squeeze math going on.
		float distanceFromOrigin = D3DXVec3Length(&g_positions[i]);
		int red = -(int)(distanceFromOrigin * distanceFromOrigin * 15.0f) + 255;
		red = (red < 0) ? 0 : red;
		red = (red > 255) ? 255 : red;
		int green = -(int)((distanceFromOrigin - 5) * (distanceFromOrigin - 5) * 15) + 255;
		green = (green < 0) ? 0 : green;
		green = (green > 255) ? 255 : green;
		int blue = -(int)((distanceFromOrigin - 8) * (distanceFromOrigin - 8) * 15) + 255;
		blue = (blue < 0) ? 0 : blue;
		blue = (blue > 255) ? 255 : blue;
		D3DCOLOR color = D3DCOLOR_XRGB(red, green, blue);
		ZeroMemory(&verts[i], sizeof(CUSTOMVERTEX));
		verts[i].x = g_positions[i].x;
		verts[i].y = g_positions[i].y;
		verts[i].z = g_positions[i].z;
		verts[i].color = color;
	}

	//Fill up the buffer with the new vertices
	m_VB.SetData(1000, verts);
	

	
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
	m_VB.Render(pDevice, 1000, D3DPT_POINTLIST);

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
