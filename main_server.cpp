#include <iostream>

#include <Game.h>
#include <Server.h>

#include <ServerPackets.h>
#include <ServerSettings.h>

#define DEBUG_LOGS

bool IsClientValid(const HelloPacket &packet)
{
    if(packet.type != PacketType::PACKET_HELLO ||
        packet.handshake_magic != HANDSHAKE_MAGIC ||
        packet.protocol_version != SERVER_PROTOCOL_VERSION) 
            return false;

    return true;
}

void WaitForGameClient(Server &server, int player)
{
    bool valid_client = false;

    while (!valid_client)
    {
        int result = 0;
        while (result != CLIENT_OK) 
        {
            result = server.WaitForClient();

            if(result != CLIENT_OK)
            {
                std::cout << "CLIENT HAD SOME ERRORS: " << result << '\n';
                std::cout << "TRY AGAIN...\n";
            }
        }

        HelloPacket hello_packet;
        if(!ReceivePacket(server.client_socket[player], &hello_packet, sizeof(HelloPacket)))
        {
            std::cout << "FAILED TO RECEIVE HELLO PACKET!\n";
            continue; 
        }

        #ifdef DEBUG_LOGS
        std::cout << "CLIENT PACKET TYPE: " << hello_packet.type << '\n';
        std::cout << "CLIENT MAGIC: " << hello_packet.handshake_magic << '\n';
        std::cout << "CLIENT PROTOCOL VERSION: " << hello_packet.protocol_version << '\n';
        #endif

        valid_client = IsClientValid(hello_packet); 
        if(!valid_client)
        {
            std::cout << "THE CLIENT IS NOT VALID, TRY AGAIN...\n";
        }
    }

    std::cout << "Player " << player << " has connected!\n";
}

bool SendPacketToPlayers(Server& server, const void* packet, int size)
{
    for(int player = 0; player < MAX_CLIENTS; player++)
    {
        if(!SendPacket(server.client_socket[player], packet, size))
        {
            std::cout << "FAILED TO SEND GAMESTATE PACKET!\n";
            return false;
        }

        std::cout << "GAMESTATE PACKET SENT TO PLAYER " << player << '\n';
    }

    return true;
}

int main()
{
    Server server;
    server.Create(54000);

    #ifdef DEBUG_LOGS
    std::cout << "EXPECTED MAGIC: " << HANDSHAKE_MAGIC << '\n';
    std::cout << "EXPECTED PROTOCOL_VERSION: " << SERVER_PROTOCOL_VERSION << '\n';
    #endif

    WellcomePacket wellcome_packet;

    for(int player = 0; player < MAX_CLIENTS; player++)
    {
        std::cout << "WAITING FOR PLAYER: " << player << '\n';
        WaitForGameClient(server, player);    
        wellcome_packet.player_id = player;   

        if(!SendPacket(server.client_socket[player], &wellcome_packet, sizeof(WellcomePacket)))
        {
            std::cout << "FAILED TO SEND HELLO PACKET!\n";
            player--; 
        }
    }

    GameState game_state;

    Engine::Init(game_state); 
    Engine::Start(game_state);

    GameStatePacket gamestate_packet;
    gamestate_packet.game_state;

    if(!SendPacketToPlayers(server, &gamestate_packet, sizeof(GameStatePacket)))
    {
        std::cout << "FAILED TO SEND STARTGAME PACKET TO PLAYERS!\n";

        server.Close();
        return 1;
    }

    std::cout << "GAME STARTED\n";

    while(game_state.game_phase == GamePhase::PLAY)
    {
        HitActionPacket action_packet;

        if(!ReceivePacket(server.client_socket[game_state.current_player], &action_packet, sizeof(HitActionPacket)))
        {
            std::cout << "FAILED TO RECEIVE HIT ACTION PACKET!\n";
            break;
        }

        if(action_packet.player_id != game_state.current_player)
        {
            std::cout << "PLAYER ID " << action_packet.player_id << " IS NOT THE CURRENT PLAYER " << game_state.current_player <<"!\n";
            break;
        }

        HitResult hit_result = Engine::Hit(game_state, action_packet.action);
        
        HitResultPacket hitresult_packet;
        hitresult_packet.player_id = game_state.current_player;
        hitresult_packet.hit_result = hit_result;

        if(!SendPacket(server.client_socket[game_state.current_player], &hitresult_packet, sizeof(HitResultPacket)))
        {
            std::cout << "FAILED TO SEND HIT RESULT PACKET!\n";
            break;
        }

        GameStatePacket gamestate_packet;
        gamestate_packet.game_state = game_state; 

        if(!SendPacketToPlayers(server, &gamestate_packet, sizeof(GameStatePacket)))
        {
            std::cout << "FAILED TO SEND GAME STATE TO PLAYERS\n";
            break;
        }
    }

    server.Close();

    return 0;
}