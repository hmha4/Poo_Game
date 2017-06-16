#pragma once
#include "StaticSprite.h"

#define SPRITE_UPDATE 100

class AnimatedSprite : public StaticSprite
{
	int m_frameNumber;
	int m_maxFrame;
	int m_frameWidth;
	int m_frameHeight;
	float m_lastUpdate, m_animationTimer;
	bool m_animate;

	void doAnimation(float dt);

public:
	AnimatedSprite();
	~AnimatedSprite();

	virtual void Initialize(char * fileName);
	void Update(float dt);
	void Render(D3DXVECTOR3 Position, RECT rect);
	
	//Getters
	const RECT GetRect() const { return m_rect; }

	//Setters
	void SetWidthHeight(int width, int height) { m_frameWidth = width; m_frameHeight = height; }
	void SetAnimating(bool animate) { m_animate = animate; }
	void SetMaxFrames(int maxFrame) { m_maxFrame = maxFrame; }
};

