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
#include "CFont.h"
#include "CMesh.h"

class CGameApp : public CBaseApp
{
private:
	CFramework*     m_pFramework;
	CWorldTransform m_transform;
	CFont           m_font;
	LPD3DXSPRITE    m_pTextSprite;
	D3DLIGHT9       m_light;
	CMesh           m_mesh;
	CMeshInstance*  m_pTemple;

	char m_fps[10];
	BOOL m_showInstructions;

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
	virtual void ProcessInput(long xDelta, long yDelta, long zDelta, BOOL* pMouseButtons, BOOL* pPressedKeys, float elapsedTime);
};

#endif // !CGAMEAPP_H