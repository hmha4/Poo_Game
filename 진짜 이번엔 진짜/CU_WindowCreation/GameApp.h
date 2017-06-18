#pragma once
#ifndef CGAMEAPP_H
#define CGAMEAPP_H

#include "stdafx.h"
#include "CFramework.h"
#include "CVertexBuffer.h"
#include "CIndexBuffer.h"

class CGameApp : public CBaseApp
{
private:
	CFramework* m_pFramework;
	CVertexBuffer m_VB4;
	CVertexBuffer m_VB6;
	CIndexBuffer m_IB;
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
	virtual void OnUpdateFrame(LPDIRECT3DDEVICE9 pDevice);
	virtual void OnRenderFrame(LPDIRECT3DDEVICE9 pDevice);
	virtual void OnKeyDown(WPARAM wParam);
};

#endif // !CGAMEAPP_H