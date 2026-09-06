#pragma once

#include "TcpSocket.h"
#include "Ipv4Endpoint.h"

namespace gm
{
	class TcpConnector
	{
	public:
		enum class ConnectResult
		{
			Failed,
			InProgress,
			Complete
		};

		bool			StartConnect(const Ipv4Endpoint& targetEndpoint);
		ConnectResult	TryTakeSocket(TcpSocket& outSocket);

	private:
		ConnectResult	TakeConnectedSocket(TcpSocket& outSocket);
		void			Clear();

	private:
		enum class State
		{
			Idle,
			Connecting,
			Complete
		};

		State			_state = State::Idle;
		TcpSocket		_connectingSocket{};
	};
}



