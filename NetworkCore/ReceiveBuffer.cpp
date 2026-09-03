#include "ReceiveBuffer.h"

#include <algorithm>

namespace gm
{
	ReceiveBuffer::ReceiveBuffer(std::size_t size)
	{
		_buffer.resize(size);
	}

	std::span<const std::byte> ReceiveBuffer::GetReadableSpan() const
	{
		return std::span<const std::byte> {_buffer.data() + _readPosition, GetReadableSize()};
	}

	bool ReceiveBuffer::Append(std::span<const std::byte> data)
	{
		std::size_t writeSize = data.size();
		if (CanWrite(writeSize) == false)
			return false;

		if (needCompact(writeSize))
			Compact();

		std::copy(data.begin(), data.end(), _buffer.begin() + _writePosition);
		_writePosition += writeSize;

		return true;
	}

	bool ReceiveBuffer::Consume(std::size_t consumeSize)
	{
		if (consumeSize > GetReadableSize())
			return false;

		_readPosition += consumeSize;

		return true;
	}

	std::size_t ReceiveBuffer::GetReadableSize() const
	{
		return _writePosition - _readPosition;
	}

	std::size_t ReceiveBuffer::GetWritableSize() const
	{
		return _buffer.size() - _writePosition;
	}

	bool ReceiveBuffer::CanWrite(std::size_t writeSize) const
	{
		return GetAvailableSize() >= writeSize;
	}

	std::size_t ReceiveBuffer::GetAvailableSize() const
	{
		return _readPosition + GetWritableSize();
	}

	bool ReceiveBuffer::needCompact(std::size_t writeSize) const
	{
		return writeSize > GetWritableSize();
	}

	void ReceiveBuffer::Compact()
	{
		if (_writePosition == 0)
			return;

		std::copy(_buffer.begin() + _readPosition, _buffer.begin() + _writePosition, _buffer.begin());

		_writePosition = GetReadableSize();
		_readPosition = 0;
	}
}
