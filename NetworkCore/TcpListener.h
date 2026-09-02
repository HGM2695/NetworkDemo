#pragma once

#include "TcpSocket.h"
#include "Ipv4Endpoint.h"

namespace gm
{
	class TcpListener
	{
	public:
		enum class AcceptResult
		{
			Accepted,
			WouldBlock,
			Failed
		};

		~TcpListener();

		bool			StartListening(const Ipv4Endpoint& endpoint, int backlog = SOMAXCONN);
		void			StopListening();

		AcceptResult	TryAccept(TcpSocket& outSocket);

		bool			IsListening() const { return _isListening; }

	private:
		bool			_isListening = false;
		TcpSocket		_listenSocket{};
	};
}
