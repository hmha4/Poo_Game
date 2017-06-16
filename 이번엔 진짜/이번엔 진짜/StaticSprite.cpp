#include "stdafx.h"
#include "StaticSprite.h"


StaticSprite::StaticSprite()
{
	m_textures = NULL;
}


StaticSprite::~StaticSprite()
{

}

void StaticSprite::Initialize(char * fileName)
{
	CreateTexture(fileName);
}

void StaticSprite::CreateTexture(char * fileName)
{
	m_textures = g_DXManager.GetTexture(fileName);
}

void StaticSprite::Render()
{
	g_DXManager.DrawSprite(m_textures);
}

void StaticSprite::Render(D3DXVECTOR3 Position)
{
	g_DXManager.DrawSprite(m_textures, Position);
}

