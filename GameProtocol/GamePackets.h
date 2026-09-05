#pragma once

#include "GameProtocolTypes.h"

namespace gm
{
    enum class PacketId : std::uint16_t
    {
        Invalid = 0,

        C2S_JoinRequest,
        S2C_JoinAccepted,

        S2C_PlayerJoined,
        S2C_PlayerLeft,

        C2S_MoveRequest,
        S2C_PlayerMoved,

        C2S_ChatRequest,
        S2C_ChatBroadcast,
    };

    // C2S_JoinRequest: payload 전체가 UTF-8 nickname bytes
    // C2S_ChatRequest: payload 전체가 UTF-8 message bytes

    // S2C_PlayerJoined: S2CPlayerJoinedPrefix 뒤에 UTF-8 nickname bytes
    // S2C_ChatBroadcast: S2CChatBroadcastPrefix 뒤에 UTF-8 message bytes

    struct S2CJoinAccepted
    {
        PlayerId        playerId = InvalidPlayerId;
    };

    struct S2CPlayerJoinedPrefix
    {
        PlayerId        playerId = InvalidPlayerId;
        float           positionX = 0.f;
        float           positionY = 0.f;
    };

    struct S2CPlayerLeft
    {
        PlayerId        playerId = InvalidPlayerId;
    };

    struct C2SMoveRequest
    {
        float           directionX = 0.f;
        float           directionY = 0.f;
    };

    struct S2CPlayerMoved
    {
        PlayerId        playerId = InvalidPlayerId;
        float           positionX = 0.f;
        float           positionY = 0.f;
    };

    struct S2CChatBroadcastPrefix
    {
        PlayerId        senderId = InvalidPlayerId;
    };
}
