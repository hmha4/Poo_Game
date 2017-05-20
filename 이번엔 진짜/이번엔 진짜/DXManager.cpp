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
	if (NULL == m_d3dObject)
	{
		return false;
	}

	D3DPRESENT_PARAMETERS presParams;
	ZeroMemory(&presParams, sizeof(presParams));

	presParams.Windowed = TRUE;
	presParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
	presParams.hDeviceWindow = hWnd;
	presParams.BackBufferFormat = D3DFMT_X8R8G8B8;
	presParams.BackBufferWidth = SCREEN_WIDTH;
	presParams.BackBufferHeight = SCREEN_HEIGHT;
	presParams.EnableAutoDepthStencil = TRUE;
	presParams.AutoDepthStencilFormat = D3DFMT_D16;

	//Create the D3D Device
	HRESULT hr = m_d3dObject->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &presParams, &m_d3dDevice);
	if (FAILED(hr))
	{
		//sometimes cards doesn't have vertex processing so go back to software
		hr = m_d3dObject->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &presParams, &m_d3dDevice);
		if (FAILED(hr))
		{
			return false;
		}
	}
	return true;
}

void DXManager::CleanUpDirectX()
{
	for (int i = 0; i < MAX_TEXTURES; i++)
	{
		if (m_textures[i].texture)
		{
			m_textures[i].texture->Release();
		}
	}

	if (m_sprite)
	{
		m_sprite->Release();
	}
	if (m_d3dObject)
	{
		m_d3dObject->Release();
	}
	if (m_d3dDevice)
	{
		m_d3dDevice->Release();
	}
}

bool DXManager::CreateSprite()
{
	HRESULT hr = D3DXCreateSprite(m_d3dDevice, &m_sprite);
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

HRESULT DXManager::BegineScene()
{
	HRESULT hr = m_d3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	if (FAILED(hr))
	{
		return -1; //return nothing
	}

	hr = m_d3dDevice->BeginScene();

	return hr;
}

void DXManager::StartScene()
{
	m_sprite->Begin(D3DXSPRITE_ALPHABLEND);
}

void DXManager::EndScene()
{
	m_sprite->End();
	m_d3dDevice->EndScene();
	m_d3dDevice->Present(NULL, NULL, NULL, NULL);
}

LPDIRECT3DTEXTURE9 DXManager::GetTexture(char * fileName)
{
	for (int i = 0; i < MAX_TEXTURES; i++)
	{
		if (m_textures[i].inUse && NULL != m_textures[i].texture)
		{
			continue;
		}

		m_textures[i].inUse = true;
		m_textures[i].fileName = fileName;

		D3DXCreateTextureFromFileA(m_d3dDevice, m_textures[i].fileName, &m_textures[i].texture);

		return m_textures[i].texture;
	}

	return NULL;
}

void DXManager::DrawSprite(LPDIRECT3DTEXTURE9 texture, RECT drawRect, D3DXVECTOR3 Position)
{
	m_sprite->Draw(texture, &drawRect, NULL, &Position, 0xFFFFFFFF);
}

void DXManager::DrawSprite(LPDIRECT3DTEXTURE9 texture, D3DXVECTOR3 Position)
{
	m_sprite->Draw(texture, NULL, NULL, &Position, 0xFFFFFFFF);
}

void DXManager::DrawSprite(LPDIRECT3DTEXTURE9 texture)
{
	m_sprite->Draw(texture, NULL, NULL, NULL, 0xFFFFFFFF);
}