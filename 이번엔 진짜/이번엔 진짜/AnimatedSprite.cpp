#include "stdafx.h"
#include "AnimatedSprite.h"


AnimatedSprite::AnimatedSprite()
{
	m_textures = NULL;
	m_animationTimer = 0;
	m_lastUpdate = 0;
}


AnimatedSprite::~AnimatedSprite()
{
}

void AnimatedSprite::Initialize(char * fileName)
{
	m_frameNumber = 0;
	m_maxFrame = 0;
	m_animate = false;
	StaticSprite::Initialize(fileName);
}

void AnimatedSprite::Update(float dt)
{
	m_rect.left = m_frameNumber * m_frameWidth;
	m_rect.right = m_rect.left + m_frameWidth;
	m_rect.top = 0;
	m_rect.bottom = m_frameHeight;

	doAnimation(dt);
}

void AnimatedSprite::doAnimation(float dt)
{
	m_animationTimer += dt;
	
		if (m_animate)
		{
			if (m_animationTimer - m_lastUpdate > SPRITE_UPDATE)
			{
				m_frameNumber++;
				if (m_frameNumber > m_maxFrame)
				{
					m_frameNumber = 0;
				}
				m_lastUpdate = m_animationTimer;
			}
		}
		else
		{
			m_frameNumber = 0;
		}
	
}

void AnimatedSprite::Render(D3DXVECTOR3 Position, RECT rect)
{
	g_DXManager.DrawSprite(m_textures, rect, Position);
}