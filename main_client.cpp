#include <iostream>

#include <Game.h>
#include <Client.h>

int main()
{
    Client client;

    ConnectToServer(client, "127.0.0.1", 54000);

    while (true)
    {
        HitAction action;

        std::cout << "Damage: ";
        std::cin >> action.damage;

        SendPacket(client.socket, &action, sizeof(action));

        GameState state;

        ReceivePacket(client.socket, &state, sizeof(state));

        std::cout << "Enemy Health: " << state.player[1].health << '\n';
    }
    
    return 0;
}