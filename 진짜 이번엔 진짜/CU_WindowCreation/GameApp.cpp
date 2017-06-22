#include "stdafx.h"
#include "GameApp.h"

char* g_instructions = "WASD: Move\r\nClick and drag the mouse to look around\r\nF: Toggle bounding sphere display\r\nEsc: Quit\r\nF5: Toggle fullscreen\r\nF6: Toggle wireframe\nF7: Terrain\nF8: Light\n";
BOOL TrueFalse = FALSE;

D3DXVECTOR3* g_positions;
D3DXVECTOR3* g_speeds;

struct CUSTOMVERTEX
{
	float x, y, z; // Position in 3d space
	DWORD color;   // Color 
};

CGameApp::CGameApp()
{
	m_pFramework = NULL;
	m_showInstructions = FALSE;
	m_pSphere = NULL;
	m_pCSphere = NULL;
	m_displaySphere = FALSE;
	m_pStone = NULL;
	baseORdetail = FALSE;
	m_pTemple = NULL;
	m_pTextMesh = NULL;
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
	m_camera.SetMaxVelocity(100.0f);
	m_camera.SetMaxPitch(D3DXToRadian(85.0f));
	m_camera.SetPosition(new D3DXVECTOR3(0.0f, 5.0f, -5.0f));
	m_camera.SetLookAt(new D3DXVECTOR3(8.0f, 0.0f, 5.0f));
	m_camera.Update();

	// Create the light 
	ZeroMemory(&m_light, sizeof(D3DLIGHT9));
	m_light.Type = D3DLIGHT_POINT;
	m_light.Diffuse.r = 1.0f;
	m_light.Diffuse.g = 1.0f;
	m_light.Diffuse.b = 1.0f;
	m_light.Direction.x = -1.0f;
	m_light.Direction.y = -1.0f;
	m_light.Direction.z = 1.0f;
	m_light.Range = 1000.0f;
	m_light.Falloff = 1.0f;
	m_light.Attenuation0 = 1.0f;
	m_light.Theta = D3DXToRadian(10.0f);
	m_light.Phi = D3DXToRadian(15.0f);

	g_positions = new D3DXVECTOR3[1000];
	g_speeds = new D3DXVECTOR3[1000];
	// Seed random number generator 
	srand((UINT)time(NULL));
	for (int i = 0; i < 1000; i++)
	{

		g_speeds[i].x = ((float)rand() / RAND_MAX * 1000.0f - 500.0f) / 50.0f;
		g_speeds[i].y = ((float)rand() / RAND_MAX * 1000.0f - 500.0f) / 50.0f;
		g_speeds[i].z = ((float)rand() / RAND_MAX * 1000.0f - 500.0f) / 50.0f;
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
	D3DXCreateSprite(pDevice, &m_pTextSprite);

	//Temple
	m_mesh.Load(pDevice, "temple.x");
	if (m_pTemple)
	{
		for (int i = 0; i < 36; i++)
		{
			m_pTemple[i].Release();
		}
	}
	m_pTemple = new CMeshInstance[36];
	//left temple
	for (int i = 0; i < 9; i++)
	{
		m_pTemple[i].SetMesh(&m_mesh);
		m_pTemple[i].SetXPosition(-5.7f);
		m_pTemple[i].SetZPosition(-3.7f + (i * 11.0f));
		m_pTemple[i].ScaleAbs(0.9f, 1.9f, 0.9f);
	}
	//top temple
	for (int i = 9; i < 18; i++)
	{
		m_pTemple[i].SetMesh(&m_mesh);
		m_pTemple[i].SetXPosition(-102.7f + (i * 11.0f));
		m_pTemple[i].SetZPosition(95.0f);
		m_pTemple[i].RotateAbs(0, D3DXToRadian(90), 0);
		m_pTemple[i].ScaleAbs(0.9f, 1.9f, 0.9f);
	}
	//right temple
	for (int i = 18; i < 27; i++)
	{
		m_pTemple[i].SetMesh(&m_mesh);
		m_pTemple[i].SetXPosition(94.7f);
		m_pTemple[i].SetZPosition(-192.7f + (i * 11.0f));
		m_pTemple[i].ScaleAbs(0.9f, 1.9f, 0.9f);
	}
	//bottom temple
	for (int i = 27; i < 36; i++)
	{
		m_pTemple[i].SetMesh(&m_mesh);
		m_pTemple[i].SetXPosition(-292.7f + (i * 11.0f));
		m_pTemple[i].SetZPosition(-5.7f);
		m_pTemple[i].RotateAbs(0, D3DXToRadian(90), 0);
		m_pTemple[i].ScaleAbs(0.9f, 1.9f, 0.9f);
	}

	//Create 2D text
	m_font.Initialize(pDevice, "Arial", 12);

	m_boxMesh.Load(pDevice, "box.x");
	m_box.SetMesh(&m_boxMesh);

	//Bounding sphere mesh
	D3DXCreateSphere(pDevice, m_box.GetBoundingRadius(), 8, 8, &m_pSphere, NULL);

	// Floor
	m_floor.CreateBuffer(pDevice, 6, D3DFVF_XYZ | D3DFVF_TEX1, sizeof(cuCustomVertex::PositionTextured));
	cuCustomVertex::PositionTextured verts[6];
	verts[0] = cuCustomVertex::PositionTextured(-10.0f, -0.5f, -10.0f, 0.0f, 100.0f);
	verts[1] = cuCustomVertex::PositionTextured(-10.0f, -0.5f, 100.0f, 0.0f, 0.0f);
	verts[2] = cuCustomVertex::PositionTextured(100.0f, -0.5f, -10.0f, 100.0f, 100.0f);
	verts[3] = cuCustomVertex::PositionTextured(-10.0f, -0.5f, 100.0f, 0.0f, 0.0f);
	verts[4] = cuCustomVertex::PositionTextured(100.0f, -0.5f, 100.0f, 100.0f, 0.0f);
	verts[5] = cuCustomVertex::PositionTextured(100.0f, -0.5f, -10.0f, 100.0f, 100.0f);
	m_floor.SetData(6, &verts, 0);
	// Floor texture
	char path[MAX_PATH];
	CUtility::GetMediaFile("temple.jpg", path);
	D3DXCreateTextureFromFile(pDevice, path, &m_pStone);

	//SKYBOX-------------------------------------------------------------------------
	//skyL
	m_skyL.CreateBuffer(pDevice, 6, D3DFVF_XYZ | D3DFVF_TEX1, sizeof(cuCustomVertex::PositionTextured));
	cuCustomVertex::PositionTextured left[6];
	left[0] = cuCustomVertex::PositionTextured(-10.0f, -0.5f, -10.0f, 0.0f, 1.0f);
	left[1] = cuCustomVertex::PositionTextured(-10.0f, -0.5f, 100.0f, 0.0f, 0.0f);
	left[2] = cuCustomVertex::PositionTextured(100.0f, -0.5f, -10.0f, 1.0f, 1.0f);
	left[3] = cuCustomVertex::PositionTextured(-10.0f, -0.5f, 100.0f, 0.0f, 0.0f);
	left[4] = cuCustomVertex::PositionTextured(100.0f, -0.5f, 100.0f, 1.0f, 0.0f);
	left[5] = cuCustomVertex::PositionTextured(100.0f, -0.5f, -10.0f, 1.0f, 1.0f);
	m_skyL.SetData(6, &left, 0);
	// skyL texture
	char leftpath[MAX_PATH];
	CUtility::GetMediaFile("Left.bmp", leftpath);
	D3DXCreateTextureFromFile(pDevice, leftpath, &m_pLeft);

	//skyR
	m_skyR.CreateBuffer(pDevice, 6, D3DFVF_XYZ | D3DFVF_TEX1, sizeof(cuCustomVertex::PositionTextured));
	cuCustomVertex::PositionTextured right[6];
	right[0] = cuCustomVertex::PositionTextured(-10.0f, -0.5f, -10.0f, 0.0f, 1.0f);
	right[1] = cuCustomVertex::PositionTextured(-10.0f, -0.5f, 100.0f, 0.0f, 0.0f);
	right[2] = cuCustomVertex::PositionTextured(100.0f, -0.5f, -10.0f, 1.0f, 1.0f);
	right[3] = cuCustomVertex::PositionTextured(-10.0f, -0.5f, 100.0f, 0.0f, 0.0f);
	right[4] = cuCustomVertex::PositionTextured(100.0f, -0.5f, 100.0f, 1.0f, 0.0f);
	right[5] = cuCustomVertex::PositionTextured(100.0f, -0.5f, -10.0f, 1.0f, 1.0f);
	m_skyR.SetData(6, &right, 0);
	// skyR texture
	char rightpath[MAX_PATH];
	CUtility::GetMediaFile("Right.bmp", rightpath);
	D3DXCreateTextureFromFile(pDevice, rightpath, &m_pRight);

	//skyT
	m_skyT.CreateBuffer(pDevice, 6, D3DFVF_XYZ | D3DFVF_TEX1, sizeof(cuCustomVertex::PositionTextured));
	cuCustomVertex::PositionTextured top[6];
	top[0] = cuCustomVertex::PositionTextured(-10.0f, -0.5f, -10.0f, 0.0f, 1.0f);
	top[1] = cuCustomVertex::PositionTextured(-10.0f, -0.5f, 100.0f, 0.0f, 0.0f);
	top[2] = cuCustomVertex::PositionTextured(100.0f, -0.5f, -10.0f, 1.0f, 1.0f);
	top[3] = cuCustomVertex::PositionTextured(-10.0f, -0.5f, 100.0f, 0.0f, 0.0f);
	top[4] = cuCustomVertex::PositionTextured(100.0f, -0.5f, 100.0f, 1.0f, 0.0f);
	top[5] = cuCustomVertex::PositionTextured(100.0f, -0.5f, -10.0f, 1.0f, 1.0f);
	m_skyT.SetData(6, &top, 0);
	// skyT texture
	char toppath[MAX_PATH];
	CUtility::GetMediaFile("Top.bmp", toppath);
	D3DXCreateTextureFromFile(pDevice, toppath, &m_pTop);

	//skyB
	m_skyB.CreateBuffer(pDevice, 6, D3DFVF_XYZ | D3DFVF_TEX1, sizeof(cuCustomVertex::PositionTextured));
	cuCustomVertex::PositionTextured bottom[6];
	bottom[0] = cuCustomVertex::PositionTextured(-10.0f, -0.5f, -10.0f, 0.0f, 1.0f);
	bottom[1] = cuCustomVertex::PositionTextured(-10.0f, -0.5f, 100.0f, 0.0f, 0.0f);
	bottom[2] = cuCustomVertex::PositionTextured(100.0f, -0.5f, -10.0f, 1.0f, 1.0f);
	bottom[3] = cuCustomVertex::PositionTextured(-10.0f, -0.5f, 100.0f, 0.0f, 0.0f);
	bottom[4] = cuCustomVertex::PositionTextured(100.0f, -0.5f, 100.0f, 1.0f, 0.0f);
	bottom[5] = cuCustomVertex::PositionTextured(100.0f, -0.5f, -10.0f, 1.0f, 1.0f);
	m_skyB.SetData(6, &bottom, 0);
	// skyB texture
	char bottompath[MAX_PATH];
	CUtility::GetMediaFile("Bottom.bmp", bottompath);
	D3DXCreateTextureFromFile(pDevice, bottompath, &m_pBottom);

	

	//skyF
	m_skyF.CreateBuffer(pDevice, 6, D3DFVF_XYZ | D3DFVF_TEX1, sizeof(cuCustomVertex::PositionTextured));
	cuCustomVertex::PositionTextured Forward[6];
	Forward[0] = cuCustomVertex::PositionTextured(-10.0f, -0.5f, -10.0f, 0.0f, 1.0f);
	Forward[1] = cuCustomVertex::PositionTextured(-10.0f, -0.5f, 100.0f, 0.0f, 0.0f);
	Forward[2] = cuCustomVertex::PositionTextured(100.0f, -0.5f, -10.0f, 1.0f, 1.0f);
	Forward[3] = cuCustomVertex::PositionTextured(-10.0f, -0.5f, 100.0f, 0.0f, 0.0f);
	Forward[4] = cuCustomVertex::PositionTextured(100.0f, -0.5f, 100.0f, 1.0f, 0.0f);
	Forward[5] = cuCustomVertex::PositionTextured(100.0f, -0.5f, -10.0f, 1.0f, 1.0f);
	m_skyF.SetData(6, &Forward, 0);
	// skyF texture
	char Forwardpath[MAX_PATH];
	CUtility::GetMediaFile("Front.bmp", Forwardpath);
	D3DXCreateTextureFromFile(pDevice, Forwardpath, &m_pForward);

	//skyBack
	m_skyBack.CreateBuffer(pDevice, 6, D3DFVF_XYZ | D3DFVF_TEX1, sizeof(cuCustomVertex::PositionTextured));
	cuCustomVertex::PositionTextured Back[6];
	Back[0] = cuCustomVertex::PositionTextured(-10.0f, -0.5f, -10.0f, 0.0f, 1.0f);
	Back[1] = cuCustomVertex::PositionTextured(-10.0f, -0.5f, 100.0f, 0.0f, 0.0f);
	Back[2] = cuCustomVertex::PositionTextured(100.0f, -0.5f, -10.0f, 1.0f, 1.0f);
	Back[3] = cuCustomVertex::PositionTextured(-10.0f, -0.5f, 100.0f, 0.0f, 0.0f);
	Back[4] = cuCustomVertex::PositionTextured(100.0f, -0.5f, 100.0f, 1.0f, 0.0f);
	Back[5] = cuCustomVertex::PositionTextured(100.0f, -0.5f, -10.0f, 1.0f, 1.0f);
	m_skyBack.SetData(6, &Back, 0);
	// skyBack texture
	char Backpath[MAX_PATH];
	CUtility::GetMediaFile("Back.bmp", Backpath);
	D3DXCreateTextureFromFile(pDevice, Backpath, &m_pBack);
	//-------------------------------------------------------------------------------
	
	//terrain
	m_terrain2.Initialize(pDevice, "heightMap.raw", "terrain_base_texture.jpg", "terrain_detail_texture.jpg");
	m_terrain2.ScaleAbs(0.8f, 0.18f, 0.8f);
	m_terrain2.RotateAbs(0, 1.5f, 0);
	m_terrain2.TranslateAbs(0, -7, 0);

	//Text3D
	SAFE_RELEASE(m_pTextMesh);
	HFONT hFont = CreateFont(0, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial");

	HDC hdc = CreateCompatibleDC(NULL);

	// Save the old font 
	HFONT hFontOld = (HFONT)SelectObject(hdc, hFont);

	// Create the text mesh 
	if (FAILED(D3DXCreateText(pDevice, hdc, "DirectX", 1.01f, 0.4f, &m_pTextMesh, NULL, NULL)))
	{

		SHOWERROR("D3DXCreateText() – Failed", __FILE__, __LINE__);
	}
	// Restore the old font and clean up 
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
	// Create the buffer
	m_VB.CreateBuffer(pDevice, 1000, D3DFVF_XYZ | D3DFVF_DIFFUSE, sizeof(CUSTOMVERTEX), TRUE);

	m_pTextSprite->OnResetDevice();
	m_font.OnResetDevice();
	
	// Set transforms
	m_camera.SetAspectRatio((float)m_pFramework->GetWidth() / (float)m_pFramework->GetHeight());
	pDevice->SetTransform(D3DTS_PROJECTION, m_camera.GetProjectionMatrix());
	
	//Set up the render states

	float pointSize = 4.0f;
	pDevice->SetRenderState(D3DRS_POINTSIZE, *((DWORD*)&pointSize));
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
	SAFE_RELEASE(m_pTextSprite);
	SAFE_RELEASE(m_pTextMesh);
	SAFE_RELEASE(m_pStone);
	SAFE_RELEASE(m_pLeft);
	SAFE_RELEASE(m_pSphere);
	m_font.Release();
	for (int i = 0; i < 36; i++)
	{
		m_pTemple[i].Release();
	}
	m_box.Release();
	m_boxMesh.Release();
	m_terrain2.Release();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Updates the current frame.
Parameters:
[in] pDevice – Pointer to a DIRECT3DDEVICE9 instance
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CGameApp::OnUpdateFrame(LPDIRECT3DDEVICE9 pDevice, float elapsedTime)
{
	m_camera.Update();

	if (!TrueFalse)
	{
		pDevice->SetRenderState(D3DRS_FILLMODE, m_pFramework->GetFillMode());
		pDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
		pDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(100, 100, 100));
		pDevice->LightEnable(0, TRUE);
		pDevice->SetLight(0, &m_light);
	}
	else
	{
		pDevice->SetRenderState(D3DRS_FILLMODE, m_pFramework->GetFillMode());
		pDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	}

	// Set a material 
	D3DMATERIAL9 material;
	ZeroMemory(&material, sizeof(D3DMATERIAL9));
	material.Diffuse.r = material.Ambient.r = 0.4f;
	material.Diffuse.g = material.Ambient.g = 1.7f;
	material.Diffuse.b = material.Ambient.b = 2.0f;
	material.Diffuse.a = material.Ambient.a = 1.0f;
	pDevice->SetMaterial(&material);

	CUSTOMVERTEX verts[1000];
	for (int i = 0; i < 1000; i++)
	{
		g_positions[i] += g_speeds[i] * elapsedTime;
		// Constrain points 
		if (g_positions[i].x < -7.0f || g_positions[i].x > 7.0f ||
			g_positions[i].y < -5.0f || g_positions[i].y > 5.0f ||
			g_positions[i].z < -7.0f || g_positions[i].z > 7.0f)
		{
			g_positions[i].x = 0.0f;
			g_positions[i].y = 0.0f;
			g_positions[i].z = 0.0f;
		}
		// Form colors based on distance from origin 
		// Lots of parabola shift and squeeze math going on. 
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
	// Fill up the buffer with the new vertices 
	m_VB.SetData(1000, verts);

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * *
Summary: Renders the current frame.
Parameters:
[in] pDevice – Pointer to a DIRECT3DDEVICE9 instance
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CGameApp::OnRenderFrame(LPDIRECT3DDEVICE9 pDevice, float elapsedTime)
{
	pDevice->SetTransform(D3DTS_VIEW, m_camera.GetViewMatrix());
	

	pDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	pDevice->BeginScene();

	

	//Render scene here
	if (!baseORdetail)
	{
		//Render SkyL
		D3DXMATRIX SKyLeft;
		D3DXMATRIX SKyLeft1;
		D3DXMATRIX SKyLeft2;
		D3DXMATRIX SKyLeft3;
		D3DXMATRIX SKyLeft4;

		D3DXMatrixRotationYawPitchRoll(&SKyLeft, 0, D3DXToRadian(-90), 0);
		D3DXMatrixScaling(&SKyLeft1, 2.0f, 2.0f, 2.0f);
		D3DXMatrixMultiply(&SKyLeft2, &SKyLeft, &SKyLeft1);
		D3DXMatrixTranslation(&SKyLeft3, -80, 0, 117.5f);
		D3DXMatrixMultiply(&SKyLeft4, &SKyLeft2, &SKyLeft3);

		pDevice->SetTransform(D3DTS_WORLD, &SKyLeft4);
		pDevice->SetTexture(0, m_pLeft);
		m_skyL.Render(pDevice, 2, D3DPT_TRIANGLELIST);

		//Render SkyR
		D3DXMATRIX SkyRight;
		D3DXMATRIX SkyRight1;
		D3DXMATRIX SkyRight2;
		D3DXMATRIX SkyRight3;
		D3DXMATRIX SkyRight4;

		D3DXMatrixRotationYawPitchRoll(&SkyRight, 0, D3DXToRadian(270), D3DXToRadian(180));
		D3DXMatrixScaling(&SkyRight1, 2.0f, 2.0f, 2.0f);
		D3DXMatrixMultiply(&SkyRight2, &SkyRight, &SkyRight1);
		D3DXMatrixTranslation(&SkyRight3, 100, 0, -98.5f);
		D3DXMatrixMultiply(&SkyRight4, &SkyRight2, &SkyRight3);

		pDevice->SetTransform(D3DTS_WORLD, &SkyRight4);
		pDevice->SetTexture(0, m_pRight);
		m_skyR.Render(pDevice, 2, D3DPT_TRIANGLELIST);

		//Render SkyT
		D3DXMATRIX SkyTop;
		D3DXMATRIX SkyTop1;
		D3DXMATRIX SkyTop2;
		D3DXMATRIX SkyTop3;
		D3DXMATRIX SkyTop4;

		D3DXMatrixRotationYawPitchRoll(&SkyTop, D3DXToRadian(270), D3DXToRadian(180), 0);
		D3DXMatrixScaling(&SkyTop1, 2.0f, 2.0f, 2.0f);
		D3DXMatrixMultiply(&SkyTop2, &SkyTop, &SkyTop1);
		D3DXMatrixTranslation(&SkyTop3, -80, 197, -80);
		D3DXMatrixMultiply(&SkyTop4, &SkyTop2, &SkyTop3);

		pDevice->SetTransform(D3DTS_WORLD, &SkyTop4);
		pDevice->SetTexture(0, m_pTop);
		m_skyT.Render(pDevice, 2, D3DPT_TRIANGLELIST);

		//Render SkyB
		D3DXMATRIX SkyBottom;
		D3DXMATRIX SkyBottom1;
		D3DXMATRIX SkyBottom2;
		D3DXMATRIX SkyBottom3;
		D3DXMATRIX SkyBottom4;

		D3DXMatrixRotationYawPitchRoll(&SkyBottom, D3DXToRadian(270), 0, 0);
		D3DXMatrixScaling(&SkyBottom1, 2.0f, 2.0f, 2.0f);
		D3DXMatrixMultiply(&SkyBottom2, &SkyBottom, &SkyBottom1);
		D3DXMatrixTranslation(&SkyBottom3, 100, -19, -80);
		D3DXMatrixMultiply(&SkyBottom4, &SkyBottom2, &SkyBottom3);

		pDevice->SetTransform(D3DTS_WORLD, &SkyBottom4);
		pDevice->SetTexture(0, m_pBottom);
		m_skyB.Render(pDevice, 2, D3DPT_TRIANGLELIST);

		//Render SkyF
		D3DXMATRIX SkyForward;
		D3DXMATRIX SkyForward1;
		D3DXMATRIX SkyForward2;
		D3DXMATRIX SkyForward3;
		D3DXMATRIX SkyForward4;

		D3DXMatrixRotationYawPitchRoll(&SkyForward, D3DXToRadian(180), D3DXToRadian(270), D3DXToRadian(90));
		D3DXMatrixScaling(&SkyForward1, 2.0f, 2.0f, 2.0f);
		D3DXMatrixMultiply(&SkyForward2, &SkyForward, &SkyForward1);
		D3DXMatrixTranslation(&SkyForward3, -98, 0, -80);
		D3DXMatrixMultiply(&SkyForward4, &SkyForward2, &SkyForward3);

		pDevice->SetTransform(D3DTS_WORLD, &SkyForward4);
		pDevice->SetTexture(0, m_pForward);
		m_skyF.Render(pDevice, 2, D3DPT_TRIANGLELIST);

		//Render SkyBack
		D3DXMATRIX SkyBack;
		D3DXMATRIX SkyBack1;
		D3DXMATRIX SkyBack2;
		D3DXMATRIX SkyBack3;
		D3DXMATRIX SkyBack4;

		D3DXMatrixRotationYawPitchRoll(&SkyBack, 0, D3DXToRadian(-90), D3DXToRadian(90));
		D3DXMatrixScaling(&SkyBack1, 2.0f, 2.0f, 2.0f);
		D3DXMatrixMultiply(&SkyBack2, &SkyBack, &SkyBack1);
		D3DXMatrixTranslation(&SkyBack3, 118, 0, 100);
		D3DXMatrixMultiply(&SkyBack4, &SkyBack2, &SkyBack3);

		pDevice->SetTransform(D3DTS_WORLD, &SkyBack4);
		pDevice->SetTexture(0, m_pBack);
		m_skyBack.Render(pDevice, 2, D3DPT_TRIANGLELIST);

		m_terrain2.Render(pDevice);

		m_transform.SetXScale(30);
		m_transform.SetYScale(30);
		m_transform.SetZScale(30);
		m_transform.SetYPosition(150);
		pDevice->SetTransform(D3DTS_WORLD, m_transform.GetTransform());
		m_VB.Render(pDevice, 1000, D3DPT_POINTLIST);
	}
	else
	{
		//Render 3D text
		if (m_pTextMesh)
		{
			D3DXMATRIX _3DText;
			m_transform.SetXPosition(40);
			m_transform.SetYPosition(10);
			m_transform.SetZPosition(40);
			m_transform.SetXScale(3);
			m_transform.SetYScale(3);
			m_transform.SetZScale(3);
			m_transform.RotateRel(0.0f, D3DXToRadian(-90.0f) * elapsedTime, 0.0f);
			pDevice->SetTransform(D3DTS_WORLD, m_transform.GetTransform());
			m_pTextMesh->DrawSubset(0);
		}
		
		// Render floor
		D3DXMATRIX identity;
		D3DXMatrixIdentity(&identity);
		pDevice->SetTransform(D3DTS_WORLD, &identity);
		pDevice->SetTexture(0, m_pStone);
		m_floor.Render(pDevice, 2, D3DPT_TRIANGLELIST);

		//Render crates
		int count = 0;
		for (int i = 0; i < 30; i++)
		{
			for (int j = 0; j < 30; j++)
			{
				m_box.TranslateAbs((float)i * 3.0f, 0.0f, (float)j * 3.0f);
				if (m_camera.SphereInFrustum(m_box.GetPosition(), m_box.GetBoundingRadius()))
				{
					if (m_displaySphere)
					{
						// Render bounding sphere
						pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
						m_pSphere->DrawSubset(0);
						pDevice->SetRenderState(D3DRS_FILLMODE, m_pFramework->GetFillMode());
					}
					m_box.Render(pDevice);
					count++;
				}
			}
		}
		sprintf(m_info, "Rendering %i of 900 crates.", count);
		sprintf(m_fps, "%.2f fps", m_pFramework->GetFPS());

		// Render Temple
		for (int i = 0; i < 36; i++)
		{
			m_pTemple[i].Render(pDevice);
		}

	}

	//Display framerate and instructions
	m_pTextSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
	m_font.Print(m_fps, 5, 5, D3DCOLOR_XRGB(255, 0, 0), m_pTextSprite);
	m_font.Print(m_info, 100, 5, D3DCOLOR_XRGB(255, 0, 0), m_pTextSprite);
	if (m_showInstructions)
	{
		m_font.Print(g_instructions, 5, 20, D3DCOLOR_XRGB(255, 255, 255), m_pTextSprite);
	}
	else
	{
		m_font.Print("Hit F1 to view the instructions.", 5, 20, D3DCOLOR_XRGB(255, 255, 255), m_pTextSprite);
	}
	m_pTextSprite->End();
	
	

	pDevice->EndScene();
	pDevice->Present(0, 0, 0, 0);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Responds to key presses
Parameters:
[in] wParam – Key down argument
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//void CGameApp::OnKeyDown(WPARAM wParam)
//{
//	switch (wParam)
//	{
//	case VK_ESCAPE:
//		PostQuitMessage(0);
//		break;
//	case VK_F1:
//		m_showInstructions = !m_showInstructions;
//		break;
//	case VK_F5:
//		if (m_pFramework != NULL)
//		{
//			m_pFramework->ToggleFullscreen();
//		}
//		break;
//	case VK_F6:
//		if (m_pFramework != NULL)
//		{
//			m_pFramework->ToggleWireframe();
//		}
//	}
//}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Responds to key presses
Parameters:
[in] xDelta – Change in mouse x-axis since last frame
[in] yDelta – Change in mouse y-axis since last frame
[in] zDelta – Change in mouse z-axis since last frame
[in] pMouseButtons – Mouse button states
[in] pPressedKeys – Keyboard keys that are pressed and not locked
[in] elapsedTime – Time elapsed since last frame
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CGameApp::ProcessInput(long xDelta, long yDelta, long zDelta, BOOL* pMouseButtons, BOOL* pPressedKeys, float elapsedTime)
{
	float cameraSpeed = 20.0f;
	float mouseSensitivity = 0.005f;
	if (pMouseButtons[0])
	{
		m_camera.Yaw(xDelta * mouseSensitivity);
		m_camera.Pitch(yDelta * mouseSensitivity);
	}
	if (pPressedKeys[DIK_W])
	{
		m_camera.MoveForward(cameraSpeed * elapsedTime);
	}
	if (pPressedKeys[DIK_A])
	{
		m_camera.Strafe(-cameraSpeed * elapsedTime);
	}
	if (pPressedKeys[DIK_S])
	{
		m_camera.MoveForward(-cameraSpeed * elapsedTime);
	}
	if (pPressedKeys[DIK_D])
	{
		m_camera.Strafe(cameraSpeed * elapsedTime);
	}
	if (pPressedKeys[DIK_Q])
	{
		m_terrain2.ScaleRel(0.0f, -0.5f * elapsedTime, 0.0f);
	}
	if (pPressedKeys[DIK_E])
	{
		m_terrain2.ScaleRel(0.0f, 0.5f * elapsedTime, 0.0f);
	}
	if (pPressedKeys[DIK_F])
	{
		m_pFramework->LockKey(DIK_F);
		m_displaySphere = !m_displaySphere;
	}
	if (pPressedKeys[DIK_ESCAPE])
	{
		m_pFramework->LockKey(DIK_ESCAPE);
		PostQuitMessage(0);
	}
	if (pPressedKeys[DIK_F8])
	{
		m_pFramework->LockKey(DIK_F8);
		TrueFalse = !TrueFalse;
	}
	if (pPressedKeys[DIK_F1])
	{
		m_pFramework->LockKey(DIK_F1);
		m_showInstructions = !m_showInstructions;
	}
	
	if (pPressedKeys[DIK_F5])
	{
		m_pFramework->LockKey(DIK_F5);
		if (m_pFramework != NULL)
		{
			m_pFramework->ToggleFullscreen();
		}
	}
	if (pPressedKeys[DIK_F6])
	{
		m_pFramework->LockKey(DIK_F6);
		if (m_pFramework != NULL)
		{
			m_pFramework->ToggleWireframe();
		}
	}
	if (pPressedKeys[DIK_F7])
	{
		m_pFramework->LockKey(DIK_F7);
		baseORdetail = !baseORdetail;
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
	if (!pFramework->Initialize("Finish", hInstance, 640, 480, TRUE))
	{
		return 0;
	}

	//Rock and rool
	pFramework->Run();

	//Clean up resources
	SAFE_RELEASE(pApplication);

	return 0;
}
