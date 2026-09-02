#include "TcpConnector.h"

#include <utility>

namespace gm
{
	bool TcpConnector::StartConnect(const Ipv4Endpoint& targetEndpoint)
	{
		if (targetEndpoint.IsValid() == false)
			return false;

		Clear();

		if (_connectingSocket.Open() == false)
			return false;

		sockaddr_in targetAddr = targetEndpoint.GetNativeAddress();
		int connect_result = connect(_connectingSocket.GetNativeSocket(), reinterpret_cast<const sockaddr*>(&targetAddr), static_cast<int>(sizeof(targetAddr)));
		if (connect_result != 0)
		{
			if (WSAGetLastError() == WSAEWOULDBLOCK)
			{
				_state = State::Connecting;
				return true;
			}

			Clear();
			return false;
		}

		_state = State::Complete;
		return true;
	}

	TcpConnector::ConnectResult TcpConnector::TryTakeSocket(TcpSocket& outSocket)
	{
		if (_state == State::Complete)
		{
			outSocket = std::move(_connectingSocket);
			Clear();
			return ConnectResult::Complete;
		}
		else if (_state == State::Connecting)
		{
			fd_set writeFDS{};
			fd_set exceptFDS{};

			const SOCKET nativeSocket = _connectingSocket.GetNativeSocket();
			FD_SET(nativeSocket, &writeFDS);
			FD_SET(nativeSocket, &exceptFDS);

			timeval timeout{};
			const int selectResult = select(0, nullptr, &writeFDS, &exceptFDS, &timeout);
			if (selectResult == SOCKET_ERROR)
			{
				Clear();
				return ConnectResult::Failed;
			}
			else if (selectResult == 0)
			{
				return ConnectResult::InProgress;
			}

			if (FD_ISSET(nativeSocket, &exceptFDS))
			{
				Clear();
				return ConnectResult::Failed;
			}

			int socketError = 0;
			int optionLength = sizeof(socketError);
			const int optionResult = getsockopt(nativeSocket, SOL_SOCKET, SO_ERROR, reinterpret_cast<char*>(&socketError), &optionLength);
			if (socketError != 0 || optionResult != 0)
			{
				Clear();
				return ConnectResult::Failed;
			}

			outSocket = std::move(_connectingSocket);
			Clear();
			return ConnectResult::Complete;
		}

		return ConnectResult::Failed;
	}

	void TcpConnector::Clear()
	{
		_connectingSocket.Close();
		_state = State::Idle;
	}
}
