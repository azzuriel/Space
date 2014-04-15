#include <memory>
#include "Game.h"

int main()
{
	Game* game = new Game();
	game->Initialize();
	game->Run();
	delete game;
}