#pragma once

#include "GraphicsDevice.h"
#include "GameSprite.h"

class Game
{
public:
	Game();
	~Game();

	//Game functions
	bool Initialize(HWND hWnd);
	void Run();
	void Update(float gameTime);
	void Draw(float gameTime);
private:
	GraphicsDevice *gDevice;

	GameSprite *player;
};

