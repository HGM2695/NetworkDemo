#include "TcpServerService.h"

#include <WinSock2.h>
#include <iostream>
#include <utility>
#include <algorithm>

namespace gm
{
	bool TcpServerService::Initialize(std::uint16_t port, TcpSession::PacketHandler packetHandler)
	{
		if (_listener.IsListening())
			return false;

		if (port == 0)
			return false;
		
		if (packetHandler == nullptr)
			return false;

		_port = port;
		_endpoint.AssignAny(_port);
		if (_listener.StartListening(_endpoint) == false)
		{
			std::cout << "리스너 소켓 초기화 실패" << std::endl;
			return false;
		}

		_sessionPacketHandler = std::move(packetHandler);

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

	bool TcpServerService::Send(TcpSession::SessionId sessionId, std::uint16_t packetId, std::span<const std::byte> payload)
	{
		if (sessionId == TcpSession::InvalidSessionId)
			return false;

		TcpSession* session = FindSession(sessionId);
		if (session == nullptr)
			return false;

		if (session->Send(packetId, payload) == false)
			return false;

		return true;
	}

	void TcpServerService::Broadcast(std::uint16_t packetId, std::span<const std::byte> payload)
	{
		for (auto& session : _sessionList)
			session.Send(packetId, payload);
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

	TcpSession* TcpServerService::FindSession(TcpSession::SessionId id)
	{
		auto Iter = std::find_if(_sessionList.begin(), _sessionList.end(), [id](const TcpSession& session)
			{
				return session.GetSessionId() == id;
			});

		if (Iter == _sessionList.end())
			return nullptr;

		return &(*Iter);
	}
}
