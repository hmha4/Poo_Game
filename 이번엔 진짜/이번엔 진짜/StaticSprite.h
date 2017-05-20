#pragma once
#include "DXManager.h"

class StaticSprite
{
	LPDIRECT3DTEXTURE9 m_textures;
	RECT m_rect;
	void CreateTexture(char * fileName);

public:
	StaticSprite();
	~StaticSprite();

	virtual void Initialize(char * fileName);
	//stationary background
	void Render();
	//moveable(parallax) backgrounds
	virtual void Render(D3DXVECTOR3 Position);
	//mosly the ground
	void Render(D3DXVECTOR3 Position, RECT rect);

	//Getter
	const RECT GetRect() const { return m_rect; }

	//Setters
	void SetRect(RECT rect) { m_rect = rect; }
	void SetRect(int left, int right, int top, int bottom) { m_rect.left = left; m_rect.right = right, m_rect.top = top, m_rect.bottom = bottom; }

};

