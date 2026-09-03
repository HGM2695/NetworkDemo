#pragma once

#include "GMEngine/TimeSystem.h"

#include "NetworkCore/TcpServerService.h"
#include "NetworkCore/WinsockRuntime.h"

namespace gm
{
	class GameServerApplication
	{
	public:
		bool	Initialize();
		void	Run();

	private:
		WinsockRuntime		_winsockRuntime;
		TimeSystem			_timeSystem;
		TcpServerService	_serverService;
	};
}
