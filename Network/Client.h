#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <Packet.h>

struct Client
{
    SOCKET socket;
};

bool ConnectToServer(Client &client, const char* ip, int port)
{
    WSADATA wsdata;
    WSAStartup(MAKEWORD(2, 2), &wsdata);

    client.socket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in server{};
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    inet_pton(AF_INET, ip, &server.sin_addr);

    if(connect(client.socket, (sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
    {
        closesocket(client.socket);
        WSACleanup();
        return false;
    }

    return true;
}

void CloseClient(Client &client)
{
    shutdown(client.socket, SD_BOTH);
    closesocket(client.socket);
}