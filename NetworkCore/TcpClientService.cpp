#include "TcpClientService.h"

#include <utility>

namespace gm
{
	static constexpr TcpSession::SessionId ClientDefaultSessionId = 1;

	bool TcpClientService::Initialize(ClientPacketHandler packetHandler)
	{
		if (packetHandler == nullptr)
			return false;

		_packetHandler = std::move(packetHandler);
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

			_session.emplace(ClientDefaultSessionId, std::move(socket),
				[packetHandler = _packetHandler](TcpSession::SessionId, PacketView packet)
				{
					packetHandler(packet);
				});
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
			Disconnect();
			return;
		}

		const bool readable = FD_ISSET(socket, &readFDS) != 0;
		const bool writable = FD_ISSET(socket, &writeFDS) != 0;

		if (_session->Tick(readable, writable) != TcpSession::PollResult::Alive)
			Disconnect();
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
		_session.reset();
		_state = State::Idle;
	}
}