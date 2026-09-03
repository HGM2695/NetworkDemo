#pragma once

#include <cstddef>
#include <vector>
#include <span>

namespace gm
{
	class ByteBuffer
	{
	public:		
		ByteBuffer(std::size_t capacity);

		std::span<const std::byte>	Peek() const;
		bool						Consume(std::size_t consumeSize);
		bool						Write(std::span<const std::byte> data);

		std::size_t					GetReadableSize() const;
		std::size_t					GetWritableSize() const;
		bool						CanWrite(std::size_t writeSize) const;

	private:
		std::size_t				GetContiguousWritableSize() const;
		bool					NeedsCompaction(std::size_t writeSize) const;
		void					Compact();

	private:
		std::vector<std::byte>	_storage;
		std::size_t				_readPosition = 0;
		std::size_t				_writePosition = 0;
	};
}


