#pragma once
#ifndef CGAMEAPP_H
#define CGAMEAPP_H

#include "stdafx.h"
#include "CFramework.h"
#include "CVertexBuffer.h"
#include "CIndexBuffer.h"
#include "CWorldTransform.h"
#include "CUtility.h"
#include "CTriangleStripPlane.h"
#include "cuCustomVertex.h"

class CGameApp : public CBaseApp
{
private:
	CFramework* m_pFramework;
	CVertexBuffer m_vb;
	CVertexBuffer m_vbDense;
	CIndexBuffer m_ib;
	CWorldTransform m_transform;
	LPDIRECT3DTEXTURE9 m_pTexture;
	D3DLIGHT9 m_light;

public:
	CGameApp();
	~CGameApp() { Release(); }
	void SetFramework(CFramework* pFramework);
	BOOL Initialize();
	virtual void Release();
	virtual void OnCreateDevice(LPDIRECT3DDEVICE9 pDevice);
	virtual void OnResetDevice(LPDIRECT3DDEVICE9 pDevice);
	virtual void OnLostDevice();
	virtual void OnDestroyDevice();
	virtual void OnUpdateFrame(LPDIRECT3DDEVICE9 pDevice, float elapsedTime);
	virtual void OnRenderFrame(LPDIRECT3DDEVICE9 pDevice, float elapsedTime);
	virtual void OnKeyDown(WPARAM wParam);
};

#endif // !CGAMEAPP_H