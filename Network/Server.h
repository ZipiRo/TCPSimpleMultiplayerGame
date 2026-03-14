#pragma once

#include <winsock2.h>
#include <Packet.h>

const int MAX_CLIENTS = 2;

struct Server
{
    SOCKET listen_socket;
    SOCKET client_socket[2];
    int client_count;
};

bool CreateServer(Server &server, int port)
{
    WSADATA wsdata;
    WSAStartup(MAKEWORD(2, 2), &wsdata);

    server.listen_socket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.S_un.S_addr = INADDR_ANY;

    bind(server.listen_socket, (sockaddr *)&address, sizeof(address));

    if(server.listen_socket == SOCKET_ERROR)
        return false;

    listen(server.listen_socket, MAX_CLIENTS);

    return true;
}

int WaitForClient(Server &server)
{
    int client_index = server.client_count;
    SOCKET &client = server.client_socket[client_index]; 

    client = accept(server.listen_socket, 0, 0);

    if(client == INVALID_SOCKET)
    {
        return WSAGetLastError();
    }

    server.client_count++;

    return 1;
}

void CloseServer(Server &server)
{
    for(int i = 0; i < server.client_count; i++)
    {
        shutdown(server.client_socket[i], SD_BOTH);
        closesocket(server.client_socket[i]);
    }  
    
    closesocket(server.listen_socket);
    WSACleanup();
}