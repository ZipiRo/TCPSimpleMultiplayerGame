#include <iostream>
#include <string>

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

    std::cout << "You are player: " << wellcome_packet.player_id << '\n';
    
    client.Close();

    return 0;
}