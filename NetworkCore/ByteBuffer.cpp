#include "ByteBuffer.h"

#include <algorithm>

namespace gm
{
	ByteBuffer::ByteBuffer(std::size_t capacity)
	{
		_storage.resize(capacity);
	}

	std::span<const std::byte> ByteBuffer::Peek() const
	{
		return std::span<const std::byte> {_storage.data() + _readPosition, GetReadableSize()};
	}

	bool ByteBuffer::Consume(std::size_t consumeSize)
	{
		if (consumeSize > GetReadableSize())
			return false;

		_readPosition += consumeSize;

		return true;
	}

	bool ByteBuffer::Write(std::span<const std::byte> data)
	{
		std::size_t writeSize = data.size();
		if (CanWrite(writeSize) == false)
			return false;

		if (NeedsCompaction(writeSize))
			Compact();

		std::copy(data.begin(), data.end(), _storage.begin() + _writePosition);
		_writePosition += writeSize;

		return true;
	}

	std::size_t ByteBuffer::GetReadableSize() const
	{
		return _writePosition - _readPosition;
	}

	std::size_t ByteBuffer::GetWritableSize() const
	{
		return _storage.size() - GetReadableSize();
	}

	bool ByteBuffer::CanWrite(std::size_t writeSize) const
	{
		return GetWritableSize() >= writeSize;
	}

	std::size_t ByteBuffer::GetContiguousWritableSize() const
	{
		return _storage.size() - _writePosition;
	}

	bool ByteBuffer::NeedsCompaction(std::size_t writeSize) const
	{
		return writeSize > GetContiguousWritableSize();
	}

	void ByteBuffer::Compact()
	{
		if (_writePosition == 0)
			return;

		std::copy(_storage.begin() + _readPosition, _storage.begin() + _writePosition, _storage.begin());

		_writePosition = GetReadableSize();
		_readPosition = 0;
	}
}
