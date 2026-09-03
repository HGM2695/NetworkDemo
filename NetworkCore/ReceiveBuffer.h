#pragma once

#include <cstddef>
#include <vector>
#include <span>

namespace gm
{
	class ReceiveBuffer
	{
	public:		
		ReceiveBuffer(std::size_t size);

		std::span<const std::byte>	GetReadableSpan() const;
		bool						Append(std::span<const std::byte> data);
		bool						Consume(std::size_t consumeSize);

		std::size_t					GetReadableSize() const;
		std::size_t					GetWritableSize() const;
		bool						CanWrite(std::size_t writeSize) const;

	private:
		std::size_t				GetAvailableSize() const;
		bool					needCompact(std::size_t writeSize) const;
		void					Compact();

	private:
		std::vector<std::byte>	_buffer;
		std::size_t				_readPosition = 0;
		std::size_t				_writePosition = 0;
	};
}


