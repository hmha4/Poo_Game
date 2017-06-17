#pragma once
#ifndef CFRAMEWORK_H
#define CFRAMEWORK_H

#include "stdafx.h"
#include "CGraphics.h"

//---------------------------------------------------------
//Interface that the main game application must implement
//---------------------------------------------------------
class CBaseApp
{
public:
	virtual ~CBaseApp() {}
	virtual void Release() = 0;
	virtual void OnCreateDevice(LPDIRECT3DDEVICE9 pDevice) = 0;
	virtual void OnResetDevice(LPDIRECT3DDEVICE9 pDevice) = 0;
	virtual void OnLostDevice() = 0;
	virtual void OnDestroyDevice() = 0;
	virtual void OnUpdateFrame(LPDIRECT3DDEVICE9 pDevice) = 0;
	virtual void OnRenderFrame(LPDIRECT3DDEVICE9 pDevice) = 0;
	virtual void OnKeyDown(WPARAM wParam) = 0;
};

class CFramework
{
public:
	CFramework();
	~CFramework();
};

#endif // !CFRAMEWORK_H