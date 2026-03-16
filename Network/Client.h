#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <Packet.h>

struct Client
{
    SOCKET network_socket;

    bool Connect(const char* ip, int port)
    {
        WSADATA wsdata;
        WSAStartup(MAKEWORD(2, 2), &wsdata);

        network_socket = socket(AF_INET, SOCK_STREAM, 0);

        sockaddr_in server{};
        server.sin_family = AF_INET;
        server.sin_port = htons(port);

        inet_pton(AF_INET, ip, &server.sin_addr);

        if(connect(network_socket, (sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
        {
            closesocket(network_socket);
            WSACleanup();
            return false;
        }

        return true;
    }
    
    void Close()
    {
        shutdown(network_socket, SD_BOTH);
        closesocket(network_socket);
    }
};