#include "TcpClientService.h"

#include <utility>

namespace gm
{
	static constexpr TcpSession::SessionId ClientDefaultSessionId = 1;

	bool TcpClientService::Initialize(TcpSession::PacketHandler packetHandler, TcpSession::SessionCloseHandler closeHandler)
	{
		if (packetHandler == nullptr)
			return false;

		if (closeHandler == nullptr)
			return false;

		_packetHandler = std::move(packetHandler);
		_sessionCloseHandler = std::move(closeHandler);
		_state = State::Idle;
		return true;
	}

	void TcpClientService::Tick()
	{
		if (_state == State::Idle)
			return;
		else if (_state == State::Connecting)
		{
			TcpSocket socket{};
			TcpConnector::ConnectResult result = _connector.TryTakeSocket(socket);
			if (result == TcpConnector::ConnectResult::InProgress)
				return;
			else if (result == TcpConnector::ConnectResult::Failed)
			{
				Disconnect();
				return;
			}

			_session.emplace(ClientDefaultSessionId, std::move(socket), _packetHandler);
			_state = State::Connected;
		}

		fd_set readFDS{};
		fd_set writeFDS{};

		const SOCKET socket = _session->GetSocket().GetNativeSocket();

		FD_SET(socket, &readFDS);
		if (_session->GetPendingSend())
			FD_SET(socket, &writeFDS);

		timeval timeout{};
		const int selectResult = select(0, &readFDS, &writeFDS, nullptr, &timeout);

		if (selectResult == SOCKET_ERROR)
		{
			CloseSession(TcpSession::DisconnectReason::IoFailed);
			return;
		}

		const bool readable = FD_ISSET(socket, &readFDS) != 0;
		const bool writable = FD_ISSET(socket, &writeFDS) != 0;

		const TcpSession::PollResult result = _session->Tick(readable, writable);
		if (result == TcpSession::PollResult::Closed)
			CloseSession(TcpSession::DisconnectReason::PeerClosed);
		else if (result == TcpSession::PollResult::Failed)
			CloseSession(TcpSession::DisconnectReason::IoFailed);
		else if (result == TcpSession::PollResult::Invalid)
			CloseSession(TcpSession::DisconnectReason::InvalidPacket);
	}

	bool TcpClientService::Send(std::uint16_t packetId, std::span<const std::byte> payload)
	{
		if (_state != State::Connected)
			return false;

		if (_session->Send(packetId, payload) == false)
			return false;

		return true;
	}

	bool TcpClientService::Connect(const Ipv4Endpoint& endpoint)
	{
		if (_state != State::Idle)
			return false;

		if (_connector.StartConnect(endpoint) == false)
			return false;
		
		_state = State::Connecting;
		return true;
	}

	void TcpClientService::Disconnect()
	{
		if (_session.has_value())
		{
			CloseSession(TcpSession::DisconnectReason::LocalRequest);
			return;
		}

		_state = State::Idle;
	}

	void TcpClientService::CloseSession(TcpSession::DisconnectReason reason)
	{
		const TcpSession::SessionId sessionId = _session->GetSessionId();
		_session.reset();
		_state = State::Idle;
		_sessionCloseHandler(sessionId, reason);
	}
}
