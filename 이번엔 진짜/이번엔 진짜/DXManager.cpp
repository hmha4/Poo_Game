#include "stdafx.h"
#include "DXManager.h"


DXManager::DXManager()
{
	m_d3dObject = NULL;
	m_d3dDevice = NULL;
	m_sprite = NULL;

	for (int i = 0; i < MAX_TEXTURES; i++)
	{
		m_textures[i].texture = NULL;
		m_textures[i].fileName = "";
		m_textures[i].inUse = false;
	}
}


DXManager::~DXManager()
{
	CleanUpDirectX();
}

bool DXManager::InitializeDirectX(HWND hWnd)
{
	m_d3dObject = Direct3DCreate9(D3D_SDK_VERSION);
}
