#include <iostream>
#include <string>

#include <conio.h>

#include <Game.h>
#include <Client.h>

#include <ServerPackets.h>
#include <ServerSettings.h>

int main()
{
    Client client;

    std::string ip; int port;
    std::cout << "IP & PORT\n";
    std::cin >> ip >> port;

    client.Connect(ip.c_str(), port);

    HelloPacket hello_packet;
    hello_packet.handshake_magic = HANDSHAKE_MAGIC;
    hello_packet.protocol_version = SERVER_PROTOCOL_VERSION;

    if(!SendPacket(client.network_socket, &hello_packet, sizeof(HelloPacket)))
    {
        std::cout << "FAILED TO SEND HELLO PACKET!\n"; 
        
        client.Close();
        return 1;
    }

    WellcomePacket wellcome_packet;
    
    if(!ReceivePacket(client.network_socket, &wellcome_packet, sizeof(WellcomePacket)))
    {
        std::cout << "FAILED TO RECEIVE WELLCOME PACKET!\n"; 
        
        client.Close();
        return 1;
    }

    int player_id = wellcome_packet.player_id;
    
    GameState game_state;

    std::cout << "YOU ARE PLAYER " << player_id << '\n';

    GameStatePacket gamestate_packet;
    if(!ReceivePacket(client.network_socket, &gamestate_packet, sizeof(GameStatePacket)))
    {
        std::cout << "FAILED TO RECEIVE GAMESTATE PACKET!\n";
        
        client.Close();
        return 1;
    }

    game_state = gamestate_packet.game_state;

    std::cout << "GAME STARTING...\n";

    while(game_state.game_phase == GamePhase::PLAY)
    {
        system("cls");
        if(game_state.current_player != player_id)
        {
            std::cout << "WAITING FOR THE OTHER PLAYER...\n";

            if(!ReceivePacket(client.network_socket, &gamestate_packet, sizeof(GameStatePacket)))
            {
                std::cout << "FAILED TO RECEIVE GAMESTATE PACKET!\n";
            
                client.Close();
                return 1;
            }

            game_state = gamestate_packet.game_state;

            continue;
        }

        std::cout << "YOU ARE PLAYER " << player_id << '\n'; 
        std::cout << "HEALTH: " << game_state.player[player_id].health << '\n';
        std::cout << "HITS: " << game_state.player[player_id].hit_count << '\n';
        std::cout << "MIN_DAMAGE " << game_state.player[player_id].min_damage << '\n';
        std::cout << "MAX_DAMAGE " << game_state.player[player_id].max_damage << '\n';
        
        std::cout << "==============================================\n";

        std::cout << "OTHER PLAYER HEALTH: " << game_state.player[1 - game_state.current_player].health << '\n';
        std::cout << "OTHER PLAYER TARGET HEALTH: " << game_state.player[1 - game_state.current_player].loose_health << '\n';
        
        std::cout << "==============================================\n";

        std::cout << "GIVE DAMAGE FROM THE DAMAGE INERVAL: ";

        HitAction hit_action;
        std::cin >> hit_action.damage;

        std::cout << "==============================================\n";

        HitActionPacket hitaction_packet;
        hitaction_packet.player_id = player_id;
        hitaction_packet.action = hit_action;

        if(!SendPacket(client.network_socket, &hitaction_packet, sizeof(HitActionPacket)))
        {
            std::cout << "FAILED TO SEND ACTION PACKET!\n";
            
            client.Close();
            return 1;
        }

        HitResultPacket hitresult_packet;
        if(!ReceivePacket(client.network_socket, &hitresult_packet, sizeof(HitResultPacket)))
        {
            std::cout << "FAILED TO RECEIVE HIT RESULT PACKET!\n";

            client.Close();
            return 1;
        }

        if(hitresult_packet.player_id != game_state.current_player)
        {
            std::cout << "PLAYER ID " << hitresult_packet.player_id << " IS NOT THE CURRENT PLAYER " << game_state.current_player <<"!\n";
            
            client.Close();
            break;
        }

        HitResult hit_result = hitresult_packet.hit_result;
        
        if(hit_result == HIT_OK)
        {
            system("cls");
            std::cout << "PLAYER: " << player_id + 1 << '\n';
            std::cout << "ENEMY TOOK : " << hit_action.damage << " POINTS OF DAMAGE\n";

            int difference = game_state.player[1 - game_state.current_player].health - 
                                game_state.player[1 - game_state.current_player].loose_health;

            std::cout << "DISTANCE TO TARGET: " << difference << '\n';

            std::cout << "===============================================================\n";
            std::cout << "PRESS ANY KEY TO CONTINUE...\n";

            getch();
        }
        else if(hit_result == HIT_GAME_OVER)
        {
            system("cls");
            std::cout << "PLAYER: " << player_id + 1 << " CAUSED GAME END\n";
            std::cout << "===============================================================\n";
            std::cout << "PRESS ANY KEY TO CONTINUE...\n";

            getch();
            break;
        }
        else if(hit_result == HIT_INVALID)
        {
            system("cls");
            std::cout << "HIT ACTION WAS INVALID!!!\n";
            std::cout << "===============================================================\n";
            std::cout << "PRESS ANY KEY TO CONTINUE...\n";

            getch();
        }

        if(!ReceivePacket(client.network_socket, &gamestate_packet, sizeof(GameStatePacket)))
        {
            std::cout << "FAILED TO RECEIVE GAMESTATE PACKET!\n";
        
            client.Close();
            return 1;
        }

        game_state = gamestate_packet.game_state;
    }

    client.Close();

    return 0;
}