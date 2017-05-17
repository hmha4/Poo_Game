#include "Game.h"


Game::Game()
{
}

bool Game::Initialize(HWND hWnd)
{
	gDevice = new GraphicsDevice();
	if (!gDevice->Initialize(hWnd, true))
	{
		MessageBox(NULL, "Error in Device Initialize.", NULL, NULL);
		return false;
	}

	player = new GameSprite(100, 200);
	if (!player->Initialize(gDevice->device, "Texture/Player/PlayerPaper.png", 110, 112))
	{
		MessageBox(NULL, "Error in Player Texture Initialize.", NULL, NULL);
		return false;
	}

	return true;
}

void Game::Run()
{
	//Get game time and update + draw
	float gameTime = 0;

	Update(gameTime);
	Draw(gameTime);
}

void Game::Update(float gameTime)
{
	//Update our sprites and other game logic
}

void Game::Draw(float gameTime)
{
	//Simple RGB value for the background so use XRGB instead of ARGB
	gDevice->Clear(D3DCOLOR_XRGB(0, 100, 100));
	gDevice->Begin();

	//Draw logic here.
	if (player && player->IsInitialized())
		player->Draw(gameTime);

	gDevice->End();
	gDevice->Present();
}

Game::~Game()
{
	if (player) { delete player; player = 0; }
	if (gDevice) { delete gDevice; gDevice = 0; }
}
