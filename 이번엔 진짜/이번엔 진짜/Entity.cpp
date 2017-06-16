#include "stdafx.h"
#include "Entity.h"


Entity::Entity()
{
	m_Health = 0;
}


Entity::~Entity()
{
}

void Entity::Initialize(int width, int height)
{
	m_AnimatedSprite.SetWidthHeight(width, height);
}

void Entity::Update(float dt)
{
	m_AnimatedSprite.Update(dt);
}

void Entity::Render(D3DXVECTOR3 Position)
{
	m_AnimatedSprite.Render(Position, m_AnimatedSprite.GetRect());
}

void Entity::SetAnimation(int frames)
{
	m_AnimatedSprite.SetMaxFrames(frames);
	if (frames == 0)
	{
		m_AnimatedSprite.SetAnimating(false);
	}
	else
	{
		m_AnimatedSprite.SetAnimating(true);
	}
}