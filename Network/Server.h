#pragma once

#include <winsock2.h>
#include <Packet.h>

const int MAX_CLIENTS = 2;

struct Server
{
    SOCKET listen_socket;
    SOCKET client_socket[2];
    int client_count = 0;
    
    bool Create(int port)
    {
        WSADATA wsdata;
        WSAStartup(MAKEWORD(2, 2), &wsdata);

        listen_socket = socket(AF_INET, SOCK_STREAM, 0);

        sockaddr_in address{};
        address.sin_family = AF_INET;
        address.sin_port = htons(port);
        address.sin_addr.S_un.S_addr = INADDR_ANY;

        bind(listen_socket, (sockaddr *)&address, sizeof(address));

        if(listen_socket == SOCKET_ERROR)
            return false;

        listen(listen_socket, MAX_CLIENTS);

        return true;
    }

    int WaitForClient()
    {
        int client_index = client_count;
        SOCKET &client = client_socket[client_index]; 

        client = accept(listen_socket, 0, 0);

        if(client == INVALID_SOCKET)
            return WSAGetLastError();

        client_count++;

        return 1;
    }

    void Close()
    {
        for(int i = 0; i < client_count; i++)
        {
            shutdown(client_socket[i], SD_BOTH);
            closesocket(client_socket[i]);
        }  
        
        closesocket(listen_socket);
        WSACleanup();
    }
};
