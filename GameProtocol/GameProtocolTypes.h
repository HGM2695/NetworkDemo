#pragma once

#include <cstdint>
#include <cstddef>

namespace gm
{
	using PlayerId = std::uint32_t;

	inline constexpr PlayerId		InvalidPlayerId = 0;
	inline constexpr std::size_t	MaxNicknameByteLength = 32;
	inline constexpr std::size_t	MaxChatMessageByteLength = 256;

	struct NetworkPosition2D
	{
		float x = 0.f;
		float y = 0.f;
	}; 
		
}
