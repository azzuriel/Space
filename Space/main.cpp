#include "Game.h"
#include <memory>
#include <easylogging++.h>

_INITIALIZE_EASYLOGGINGPP 

    void copy_file(const std::string& src, const std::string& dst, bool rw = true) 
{
    std::ifstream in(src.c_str(), std::ios::binary);
    if ( !in ) 
    {
        throw std::runtime_error("can`t open file: " + src);
    }
    std::ofstream out(dst.c_str(), (std::ios::binary | ((rw) ? std::ios::trunc : std::ios::binary)));
    if ( !out ) 
    {
        throw std::runtime_error("can`t create file: " + dst);
    }
    out << in.rdbuf();
    in.close();
    out.close();
    remove(src.c_str());
}

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