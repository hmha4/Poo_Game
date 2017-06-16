#pragma once
#include "DXManager.h"

class StaticSprite
{
	void CreateTexture(char * fileName);

protected:
	LPDIRECT3DTEXTURE9 m_textures;
	RECT m_rect;
	

public:
	StaticSprite();
	~StaticSprite();

	virtual void Initialize(char * fileName);
	//stationary background
	void Render();
	//moveable(parallax) backgrounds
	void Render(D3DXVECTOR3 Position);
	

	//Getter
	const RECT GetRect() const { return m_rect; }

	//Setters
	void SetRect(RECT rect) { m_rect = rect; }
	void SetRect(int left, int right, int top, int bottom) { m_rect.left = left; m_rect.right = right, m_rect.top = top, m_rect.bottom = bottom; }

};

