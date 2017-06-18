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
private:
	LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM, LPARAM lParam);
	void OnCreateDevice();
	void OnResetDevice();
	void OnLostDevice();
	void OnDestroyDevice();
	void OnUpdateFrame();
	void OnRenderFrame();

	HWND m_hWnd;
	HINSTANCE m_hInstance;
	BOOL m_active;
	int m_windowWidth;
	int m_windowHeight;
	int m_fullscreenWidth;
	int m_fullscreenHeight;
	WINDOWPLACEMENT m_wp;
	DWORD m_fillMode;

	CGraphics* m_pGraphics;
	CBaseApp* m_pGameApp;

public:
	CFramework(CBaseApp* pGameApp);
	~CFramework() { Release(); }
	BOOL Initialize(char* title, HINSTANCE hInstance, int width, int height, BOOL windowed = TRUE);
	void Run();
	void Release();
	void ToggleFullscreen();
	static LRESULT CALLBACK StaticWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	DWORD GetFillMode();
	int GetWidth();
	int GetHeight();
};

#endif // !CFRAMEWORK_H