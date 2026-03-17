#pragma once

#include <Game.h>

enum PacketType
{
    PACKET_HELLO,
    PACKET_WELOCME,
    PACKET_SENDGAMESTATE,
    PACKET_HITACTION,
    PACKET_HITRESULT
};

#pragma pack(push, 1)

struct Packet {
    PacketType type; 
};

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

struct GameStatePacket : public Packet
{
    GameStatePacket() { type = PacketType::PACKET_SENDGAMESTATE; }
    GameState game_state;
};

struct HitActionPacket : public Packet
{
    HitActionPacket() { type = PacketType::PACKET_HITACTION; }
    int player_id;
    HitAction action;
};

struct HitResultPacket : public Packet
{
    HitResultPacket() { type = PacketType::PACKET_HITRESULT; }
    int player_id;
    HitResult hit_result;
};

#pragma pack(pop)