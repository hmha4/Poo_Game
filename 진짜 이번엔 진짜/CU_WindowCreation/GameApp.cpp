#include "stdafx.h"
#include "GameApp.h"

CGameApp::CGameApp()
{
	m_pFramework = NULL;
	m_pTexture = NULL;
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
	m_light.Type = D3DLIGHT_SPOT;
	m_light.Diffuse.r = 1.0f;
	m_light.Diffuse.g = 1.0f;
	m_light.Diffuse.b = 1.0f;
	m_light.Direction.x = 0.0f;
	m_light.Direction.y = -0.5f;
	m_light.Direction.z = 1.0f;
	m_light.Range = 1000.0f;
	m_light.Falloff = 1.0f;
	m_light.Attenuation0 = 1.0f;
	m_light.Theta = D3DXToRadian(10.0f);
	m_light.Phi = D3DXToRadian(15.0f);
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
	cuCustomVertex::PositionNormalTextured vertices[] =
	{
		cuCustomVertex::PositionNormalTextured(-1.0f,  1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f),
		cuCustomVertex::PositionNormalTextured(1.0f,  1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f),
		cuCustomVertex::PositionNormalTextured(1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f),
		cuCustomVertex::PositionNormalTextured(-1.0f,  1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f),
		cuCustomVertex::PositionNormalTextured(1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f),
		cuCustomVertex::PositionNormalTextured(-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f),

		cuCustomVertex::PositionNormalTextured(1.0f,  1.0f, -1.0f, 1.0f, 0.0f,  0.0f, 0.0f, 0.0f),
		cuCustomVertex::PositionNormalTextured(1.0f,  1.0f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f),
		cuCustomVertex::PositionNormalTextured(1.0f, -1.0f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f, 1.0f),
		cuCustomVertex::PositionNormalTextured(1.0f,  1.0f, -1.0f, 1.0f, 0.0f,  0.0f, 0.0f, 0.0f),
		cuCustomVertex::PositionNormalTextured(1.0f, -1.0f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f, 1.0f),
		cuCustomVertex::PositionNormalTextured(1.0f, -1.0f, -1.0f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f),

		cuCustomVertex::PositionNormalTextured(-1.0f,  1.0f,  1.0f, 0.0f, 1.0f,  0.0f, 0.0f, 0.0f),
		cuCustomVertex::PositionNormalTextured(1.0f,  1.0f,  1.0f, 0.0f, 1.0f,  0.0f, 1.0f, 0.0f),
		cuCustomVertex::PositionNormalTextured(1.0f,  1.0f, -1.0f, 0.0f, 1.0f,  0.0f, 1.0f, 1.0f),
		cuCustomVertex::PositionNormalTextured(-1.0f,  1.0f,  1.0f, 0.0f, 1.0f,  0.0f, 0.0f, 0.0f),
		cuCustomVertex::PositionNormalTextured(1.0f,  1.0f, -1.0f, 0.0f, 1.0f,  0.0f, 1.0f, 1.0f),
		cuCustomVertex::PositionNormalTextured(-1.0f,  1.0f, -1.0f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f),
	};
	// Create low-poly crate
	m_vb.CreateBuffer(pDevice, 18, D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1, sizeof(cuCustomVertex::PositionNormalTextured));
	m_vb.SetData(18, vertices, 0);

	// Create high-poly crate
	cuCustomVertex::PositionNormalTextured* verticesDense = NULL;
	CTriangleStripPlane::GeneratePositionNormalTextured(&verticesDense, 21, 21);
	m_vbDense.CreateBuffer(pDevice, 21 * 21, D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1, sizeof(cuCustomVertex::PositionNormalTextured));
	m_vbDense.SetData(21 * 21, verticesDense, 0);

	// Create high-poly index buffer
	USHORT* indices = NULL;
	int numIndices = CTriangleStripPlane::GenerateIndices(&indices, 21, 21);
	m_ib.CreateBuffer(pDevice, numIndices, D3DFMT_INDEX16);
	m_ib.SetData(numIndices, indices, 0);
	m_vbDense.SetIndexBuffer(&m_ib);

	// Load the texture
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
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	pDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(100, 100, 100));
	pDevice->LightEnable(0, TRUE);

	//Set a material
	D3DMATERIAL9 material;
	ZeroMemory(&material, sizeof(D3DMATERIAL9));
	material.Diffuse.r = material.Ambient.r = 1.0f;
	material.Diffuse.g = material.Ambient.g = 1.0f;
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
	m_vb.Release();
	m_vbDense.Release();
	m_ib.Release();
	SAFE_RELEASE(m_pTexture);
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
	
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * *
Summary: Renders the current frame.
Parameters:
[in] pDevice – Pointer to a DIRECT3DDEVICE9 instance
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CGameApp::OnRenderFrame(LPDIRECT3DDEVICE9 pDevice, float elapsedTime)
{
	pDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	pDevice->BeginScene();

	//Render scene here
	// Low-poly crate
	m_transform.Reset();
	m_transform.SetYRotation(D3DXToRadian(45.0f));
	m_transform.SetXPosition(-1.7f);
	pDevice->SetTransform(D3DTS_WORLD, m_transform.GetTransform());
	m_vb.Render(pDevice, 6, D3DPT_TRIANGLELIST);

	// Left side
	m_transform.Reset();
	m_transform.TranslateAbs(1.0f, 0.0f, -0.7071f);
	m_transform.RotateAbs(D3DXToRadian(-90.0f), D3DXToRadian(45.0f), 0.0f);
	m_transform.ScaleAbs(0.1f, 0.1f, 0.1f);
	pDevice->SetTransform(D3DTS_WORLD, m_transform.GetTransform());
	m_vbDense.Render(pDevice, m_ib.GetNumIndices() - 2, D3DPT_TRIANGLESTRIP);

	// Right side
	m_transform.Reset();
	m_transform.TranslateAbs(2.414f, 0.0f, -0.7071f);
	m_transform.RotateAbs(D3DXToRadian(-90.0f), D3DXToRadian(-45.0f), 0.0f);
	m_transform.ScaleAbs(0.1f, 0.1f, 0.1f);
	pDevice->SetTransform(D3DTS_WORLD, m_transform.GetTransform());
	m_vbDense.Render(pDevice, m_ib.GetNumIndices() - 2, D3DPT_TRIANGLESTRIP);

	// Top side
	m_transform.Reset();
	m_transform.TranslateAbs(1.7071f, 1.0f, 0.0f);
	m_transform.SetYRotation(D3DXToRadian(45.0f));
	m_transform.ScaleAbs(0.1f, 0.1f, 0.1f);
	pDevice->SetTransform(D3DTS_WORLD, m_transform.GetTransform());
	m_vbDense.Render(pDevice, m_ib.GetNumIndices() - 2, D3DPT_TRIANGLESTRIP);

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
