#pragma once

#include "AnimatedSprite.h"

class Entity
{
	AnimatedSprite m_AnimatedSprite;
	int m_ID;
	int m_Health;

protected:
	Entity();
	~Entity();

	void SetID(int ID) { m_ID = ID; }

public:
	friend class EntityManager;

	void Initialize(int width, int height);
	void Update(float dt);
	void Render(D3DXVECTOR3 Position);

	//Getters
	const RECT GetRect() const { return m_AnimatedSprite.GetRect(); }
	const int GetHealth() const { return m_Health; }
	const int GetID() const { return m_ID; }

	//Setters
	void SetHealth(int health) { m_Health = health; }
	void SetAnimation(int frames);
	void SetFileName(char* fileName) { m_AnimatedSprite.Initialize(fileName); }
};

