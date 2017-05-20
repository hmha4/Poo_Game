#pragma once

#include <d3dx9.h>

#define MAX_TEXTURES 250

class DXManager
{
	LPDIRECT3D9 m_d3dObject; // dirextx object
	LPDIRECT3DDEVICE9 m_d3dDevice; //directx device
	LPD3DXSPRITE m_sprite; //directx interface

	struct texture_data
	{
		LPDIRECT3DTEXTURE9 texture;
		char * fileName;
		bool inUse;
	};
	texture_data m_textures[MAX_TEXTURES];
	void CleanUpDirectX();

public:
	DXManager(void);
	~DXManager(void);

	bool InitializeDirectX(HWND hWnd);
	bool CreateSprite();
	HRESULT BegineScene();
	void StartScene();
	void EndScene();

	void DrawSprite(LPDIRECT3DTEXTURE9 texture, RECT drawRect, D3DXVECTOR3 Position);
	void DrawSprite(LPDIRECT3DTEXTURE9 texture, D3DXVECTOR3 Position);
	void DrawSprite(LPDIRECT3DTEXTURE9 texture);

	//Getters
	LPDIRECT3DTEXTURE9 GetTexture(char * fileName);
	LPDIRECT3DDEVICE9 GetDeviec() { return m_d3dDevice; }
};

extern DXManager g_DXManager;