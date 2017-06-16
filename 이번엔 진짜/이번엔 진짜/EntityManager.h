#pragma once

#include "Entity.h"

#define MAX_ENTITIES 250
class EntityManager
{
	Entity m_Entities[MAX_ENTITIES];
	int NewEntity(int width, int height);

public:
	EntityManager();
	~EntityManager();

	void Update(float dt);
	void Render(D3DXVECTOR3 Position);

	//Getters
	Entity* GetEntity(int width, int height);
};

