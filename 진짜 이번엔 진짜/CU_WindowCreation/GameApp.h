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
#include "CTerrain.h"
#include "CCamera.h"

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
	CTerrain        m_terrain1;
	CTerrain        m_terrain2;
	CCamera         m_camera;
	CMesh			m_boxMesh;
	CMeshInstance   m_box;
	LPD3DXMESH      m_pSphere;
	LPD3DXMESH      m_pCSphere;
	BOOL            m_displaySphere;
	CVertexBuffer   m_floor;

	CVertexBuffer   m_skyL;
	CVertexBuffer   m_skyR;
	CVertexBuffer   m_skyB;
	CVertexBuffer   m_skyT;
	CVertexBuffer   m_skyF;
	CVertexBuffer   m_skyBack;

	LPDIRECT3DTEXTURE9 m_pStone;
	LPDIRECT3DTEXTURE9 m_pLeft;
	LPDIRECT3DTEXTURE9 m_pRight;
	LPDIRECT3DTEXTURE9 m_pBottom;
	LPDIRECT3DTEXTURE9 m_pTop;
	LPDIRECT3DTEXTURE9 m_pForward;
	LPDIRECT3DTEXTURE9 m_pBack
		;
	char            m_info[40];

	char m_fps[10];
	BOOL m_showInstructions;
	BOOL baseORdetail;
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