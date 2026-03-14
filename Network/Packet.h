#pragma once

#include <winsock2.h>

bool SendPacket(SOCKET socket, const void* data, int size)
{
    const char* buffer = (const char*)data;
    int total_sent = 0;

    while (total_sent < size)
    {
        int sent = send(socket, buffer + total_sent, size - total_sent, 0);

        if(sent == SOCKET_ERROR)
            return false;

        total_sent += sent;
    }
    
    return true;
}

bool ReceivePacket(SOCKET socket, void* data, int size)
{
    char* buffer = (char*)data;
    int total_received = 0;

    while (total_received < size)
    {
        int received = send(socket, buffer + total_received, size - total_received, 0);

        if(received <= 0)
            return false;

        total_received += received;
    }
    
    return true;
}