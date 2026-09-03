#pragma once

#include <cstddef>
#include <cstdint>
#include <span>

namespace gm
{
	struct PacketHeader
	{
		std::uint16_t	packetSize = 0;
		std::uint16_t	packetId = 0;
	};

	static_assert(sizeof(PacketHeader) == 4);

	inline constexpr std::size_t PacketHeaderSize = sizeof(PacketHeader);
	inline constexpr std::size_t MaxPacketSize = 16 * 1024; // 16KB

	struct PacketView
	{
		PacketHeader				header;
		std::span<const std::byte>	payload;
	};
}
