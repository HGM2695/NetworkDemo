#pragma once

#include <cstdint>

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
}