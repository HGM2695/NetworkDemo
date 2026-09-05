#pragma once

#include <cstdint>
#include <cstddef>

namespace gm
{
	using PlayerId = std::uint32_t;

	inline constexpr PlayerId		InvalidPlayerId = 0;
	inline constexpr std::size_t	MaxNicknameByteLength = 32;
	inline constexpr std::size_t	MaxChatMessageByteLength = 256;

	enum class PlayerMotionState : std::uint8_t
	{
		Idle,
		Walk,
		Jump,
		Fall
	};

	enum class PlayerFacingDirection : std::uint8_t
	{
		Left,
		Right
	};
}
