#pragma once

#include <WinSock2.h>
#include <cstdint>
#include <string>

namespace gm
{
	class Ipv4Endpoint
	{
	public:
		bool				Assign(const std::wstring& address, std::uint16_t port);
		void				AssignAny(std::uint16_t port);

		const sockaddr_in&	GetNativeAddress() const;

	private:
		sockaddr_in _address{};
	};
}
