#include "WinsockRuntime.h"

#include <WinSock2.h>

namespace gm
{
	WinsockRuntime::~WinsockRuntime()
	{
		Shutdown();
	}

	bool WinsockRuntime::Initialize()
	{
		if (_isInitialized)
			return true;
		
		WSADATA data{};
		const int result = WSAStartup(MAKEWORD(2, 2), &data);
		
		if (result != 0)
			return false;

		if (LOBYTE(data.wVersion) != 2 || HIBYTE(data.wVersion) != 2)
		{
			WSACleanup();
			return false;
		}

		_isInitialized = true;
		return true;
	}

	void WinsockRuntime::Shutdown()
	{
		if (_isInitialized == false)
			return;

		WSACleanup();
		_isInitialized = false;
	}

	bool WinsockRuntime::IsInitialized() const
	{
		return _isInitialized;
	}
}