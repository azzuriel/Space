#include "Game_Main.h"

int main()
{
    LOG(info) << "Start";
    Game* game = new Game();
    game->Initialize();
    game->Run();
    game->Destroy();
    delete game;

    return true;
}