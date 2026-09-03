#include "TcpServerService.h"

#include <WinSock2.h>
#include <iostream>
#include <utility>

namespace gm
{
	void TempHandler(TcpSession::SessionId sessionId, PacketView packetView)
	{

	}

	bool TcpServerService::Initialize()
	{
		_endpoint.AssignAny(_port);
		if (_listener.StartListening(_endpoint) == false)
		{
			std::cout << "리스너 소켓 초기화 실패" << std::endl;
			return false;
		}

		// 임시 핸들러 할당
		_sessionPacketHandler = TempHandler;

		return true;
	}

	void TcpServerService::Tick()
	{
		fd_set readFDS{};
		fd_set writeFDS{};

		const SOCKET _listenSocket = _listener.GetSocket().GetNativeSocket();
		FD_SET(_listenSocket, &readFDS);

		for (const TcpSession& session : _sessionList)
		{
			FD_SET(session.GetSocket().GetNativeSocket(), &readFDS);
			if (session.GetPendingSend())
				FD_SET(session.GetSocket().GetNativeSocket(), &writeFDS);
		}

		timeval timeout{};
		const int selectResult = select(0, &readFDS, &writeFDS, nullptr, &timeout);

		if (selectResult == SOCKET_ERROR)
		{
			const int error = WSAGetLastError();
			std::cout << "TcpServerService::Tick select 이후 SOCKET_ERROR 발생 Error : " << error << std::endl;
			return;
		}

		if (FD_ISSET(_listenSocket, &readFDS))
			CheckAccept();

		for (auto iter = _sessionList.begin(); iter != _sessionList.end(); )
		{
			const SOCKET socket = iter->GetSocket().GetNativeSocket();
			const bool readable = FD_ISSET(socket, &readFDS) != 0;
			const bool writable = FD_ISSET(socket, &writeFDS) != 0;

			if (readable == false && writable == false)
			{
				++iter;
				continue;
			}

			TcpSession::PollResult result = iter->Tick(readable, writable);
			if (result != TcpSession::PollResult::Alive)
				iter = _sessionList.erase(iter);
			else
				++iter;
		}
	}

	void TcpServerService::CheckAccept()
	{
		while (true)
		{
			TcpSocket socket{};
			const TcpListener::AcceptResult result = _listener.TryAccept(socket);

			if (result == TcpListener::AcceptResult::WouldBlock)
				break;

			if (result == TcpListener::AcceptResult::Failed)
			{
				std::cout << "TCP 연결 accept 실패." << std::endl;
				break;
			}

			_sessionList.emplace_back(GetNextSessionId(), std::move(socket), _sessionPacketHandler);
		}
	}

	TcpSession::SessionId TcpServerService::GetNextSessionId()
	{
		return _nextSessionId++;
	}
}
