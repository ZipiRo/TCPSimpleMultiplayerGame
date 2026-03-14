#include <iostream>

#include <Game.h>
#include <Server.h>

int main()
{
    Server server;

    CreateServer(server, 54000);

    std::cout << "WAITING FOR CLIENT 1\n";
    
    int error = 0;
    error = WaitForClient(server); 
    if(error != 1)
    {
        std::cout << "ACCEPT FAILED WITH ERROR\n" << error << '\n';
        CloseServer(server);
        return 0;
    }

    std::cout << "WAITING FOR CLIENT 2\n";

    error = WaitForClient(server); 
    if(error != 1)
    {
        std::cout << "ACCEPT FAILED WITH ERROR\n" << error << '\n';
        CloseServer(server);
        return 0;
    }

    GameState game;
    Engine::Init(game);
    Engine::Start(game);

    while(game.game_phase == PLAY)
    {
        HitAction action;

        ReceivePacket(server.client_socket[game.current_player], &action, sizeof(action));

        std::cout << "PLAYER " << game.current_player + 1 << "HIT\n";
        
        Engine::Hit(game, action);

        SendPacket(server.client_socket[0], &game, sizeof(game));
        SendPacket(server.client_socket[1], &game, sizeof(game));
    }

    return 0;
}