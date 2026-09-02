#include "Ipv4Endpoint.h"

#include <WS2tcpip.h>

namespace gm
{
	bool Ipv4Endpoint::Assign(const std::wstring& address, std::uint16_t port)
	{
		sockaddr_in curAddress{};
		if (InetPtonW(AF_INET, address.c_str(), &curAddress.sin_addr) != 1)
			return false;

		curAddress.sin_family = AF_INET;
		curAddress.sin_port = htons(port);
		_address = curAddress;

		return true;
	}

	void Ipv4Endpoint::AssignAny(std::uint16_t port)
	{
		_address.sin_family = AF_INET;
		_address.sin_port = htons(port);
		_address.sin_addr.s_addr = htonl(INADDR_ANY);
	}

	const sockaddr_in& Ipv4Endpoint::GetNativeAddress() const
	{
		return _address;
	}

	bool Ipv4Endpoint::IsValid() const
	{
		return _address.sin_family == AF_INET;
	}
}
