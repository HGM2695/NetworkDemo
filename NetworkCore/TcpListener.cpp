#include "TcpListener.h"

namespace gm
{
	TcpListener::~TcpListener()
	{
		StopListening();
	}

	bool TcpListener::StartListening(const Ipv4Endpoint& endpoint, int backlog)
	{
		if (_isListening)
			return false;

		if (endpoint.IsValid() == false)
			return false;

		if (_listenSocket.Open() == false)
			return false;

		const SOCKET nativeSocket = _listenSocket.GetNativeSocket();
		const sockaddr_in& nativeAddress = endpoint.GetNativeAddress();
		int result = bind(nativeSocket, reinterpret_cast<const sockaddr*>(&nativeAddress), static_cast<int>(sizeof(nativeAddress)));
		if (result != 0)
		{
			StopListening();
			return false;
		}

		result = listen(nativeSocket, backlog);
		if (result != 0)
		{
			StopListening();
			return false;
		}

		_isListening = true;
		return true;
	}

	void TcpListener::StopListening()
	{
		_listenSocket.Close();
		_isListening = false;
	}

	TcpListener::AcceptResult TcpListener::TryAccept(TcpSocket& outSocket)
	{
		if (IsListening() == false)
			return AcceptResult::Failed;

		SOCKET acceptedSocket = accept(_listenSocket.GetNativeSocket(), nullptr, nullptr);
		if (acceptedSocket == INVALID_SOCKET)
		{
			if (WSAGetLastError() == WSAEWOULDBLOCK)
				return AcceptResult::WouldBlock;

			return AcceptResult::Failed;
		}
		
		if (outSocket.SetNativeSocket(acceptedSocket) == false)
			return AcceptResult::Failed;

		return AcceptResult::Accepted;
	}
}
