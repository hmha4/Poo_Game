#pragma once
#ifndef CFONT_H
#define CFONT_H

#include "stdafx.h"

//Font alignment
enum FONTALIGNMENT {
	FA_LEFT, FA_CENTER, FA_RIGHT, FA_TOPRIGHT, FA_TOPLEFT,
	FA_BOTTMRIGHT, FA_BOTTMLEFT
};

class CFont
{
private:
	LPD3DXFONT m_pFont;
public:
	CFont();
	~CFont() { Release(); }
	BOOL Initialize(LPDIRECT3DDEVICE9 pDevice, char* faceName, int size, BOOL bold = FALSE,
		BOOL italic = FALSE);
	void Print(char* text, int xPosition, int yPosition, DWORD color, LPD3DXSPRITE sprite = NULL,
		int textBoxWidth = 0, int textBoxHeight = 0, FONTALIGNMENT alignment = FA_LEFT);
	void OnLostDevice();
	void OnResetDevice();
	void Release();
};

#endif // !CFONT_H