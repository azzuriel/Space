#include "ShaderTest.h"
#include <memory>

int main()
{
    //copy_file("lastlog.txt", "lastlog_1.txt");
    //LOG(INFO) << "//////////////////////////////////////////////////////////////////////////";
    Game* game = new Game();
    game->Initialize();
    game->Run();
    game->Destroy();
    delete game;
}