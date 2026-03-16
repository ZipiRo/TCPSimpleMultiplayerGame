#pragma once

enum PacketType
{
    PACKET_HELLO,
    PACKET_WELOCME
};

#pragma pack(push, 1)

struct Packet { PacketType type; };

struct HelloPacket : public Packet
{
    HelloPacket() { type = PacketType::PACKET_HELLO; }
    int handshake_magic;
    int protocol_version;
};

struct WellcomePacket : public Packet
{
    WellcomePacket() {type = PacketType::PACKET_WELOCME; }
    int player_id; 
};

#pragma pack(pop)