#include "Game.h"
#include <memory>

int main()
{
	Game* game = new Game();
	game->Initialize();
	game->Run();
	delete game;
}