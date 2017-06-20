#include "stdafx.h"
#include "GameApp.h"

char* g_instructions = "Esc: Quit\r\nF5: Toggle fullscreen\r\nF6: Toggle wireframe";

CGameApp::CGameApp()
{
	m_pFramework = NULL;
	m_showInstructions = FALSE;
	m_pSphere = NULL;
	m_displaySphere = FALSE;
	m_pStone = NULL;
	baseORdetail = FALSE;
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
	CUtility::GetMediaFile("stone.jpg", path);
	D3DXCreateTextureFromFile(pDevice, path, &m_pStone);

	/*m_terrain1.Initialize(pDevice, "heightMap.raw", "terrain_base_texture.jpg");
	m_terrain1.ScaleAbs(0.5f, 0.15f, 0.5f);
	m_terrain1.RotateAbs(0, 1.5f, 0);
	m_terrain1.TranslateAbs(0, -7, 0);
	
	m_terrain2.Initialize(pDevice, "heightMap.raw", "terrain_base_texture.jpg", "terrain_detail_texture.jpg");
	m_terrain2.ScaleAbs(0.5f, 0.15f, 0.5f);
	m_terrain2.RotateAbs(0, 1.5f, 0);
	m_terrain2.TranslateAbs(0, -7, 0);*/
	
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
	m_camera.SetAspectRatio((float)m_pFramework->GetWidth() / (float)m_pFramework->GetHeight());
	pDevice->SetTransform(D3DTS_PROJECTION, m_camera.GetProjectionMatrix());

	//Set up the render states
	pDevice->SetRenderState(D3DRS_FILLMODE, m_pFramework->GetFillMode());
	pDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	
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
	SAFE_RELEASE(m_pStone);
	SAFE_RELEASE(m_pSphere);
	m_font.Release();
	m_box.Release();
	m_boxMesh.Release();
	/*m_terrain1.Release();
	m_terrain2.Release();*/
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Updates the current frame.
Parameters:
[in] pDevice – Pointer to a DIRECT3DDEVICE9 instance
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CGameApp::OnUpdateFrame(LPDIRECT3DDEVICE9 pDevice, float elapsedTime)
{
	m_camera.Update();
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
	/*if (baseORdetail)
		m_terrain1.Render(pDevice);
		
	else
		m_terrain2.Render(pDevice);*/
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
				m_box.Render(pDevice);
				count++;

				if (m_displaySphere)
				{
					// Render bounding sphere
					pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
					m_pSphere->DrawSubset(0);
					pDevice->SetRenderState(D3DRS_FILLMODE, m_pFramework->GetFillMode());
				}
			}
		}
	}
	sprintf(m_info, "Rendering %i of 900 crates.", count);
	sprintf(m_fps, "%.2f fps", m_pFramework->GetFPS());

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
