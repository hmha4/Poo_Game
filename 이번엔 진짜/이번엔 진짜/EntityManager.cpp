#include "stdafx.h"
#include "EntityManager.h"


EntityManager::EntityManager()
{
	for (int i = 0; i < MAX_ENTITIES; i++)
	{
		m_Entities[i].SetID(-1);
	}
}


EntityManager::~EntityManager()
{
}

int EntityManager::NewEntity(int width, int height)
{
	int currentEntity;

	for (int i = 0; i < MAX_ENTITIES; i++)
	{
		if (m_Entities[i].GetID() != -1)
		{
			continue;
		}
		m_Entities[i].Initialize(width, height);
		m_Entities[i].SetID(i);
		currentEntity = i;
		break;
	}

	return currentEntity;
}


Entity* EntityManager::GetEntity(int width, int height)
{
	return &m_Entities[NewEntity(width, height)];
}

void EntityManager::Update(float dt)
{
	for (int i = 0; i < MAX_ENTITIES; i++)
	{
		if (m_Entities[i].GetID() != -1)
		{
			m_Entities[i].Update(dt);
		}
	}
}

void EntityManager::Render(D3DXVECTOR3 Position)
{
	for (int i = 0; i < MAX_ENTITIES; i++)
	{
		if (m_Entities[i].GetID() != -1)
		{
			m_Entities[i].Render(Position);
		}
	}
}
