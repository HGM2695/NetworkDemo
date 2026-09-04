#include "NetworkDemoGameInstance.h"
#include "TitleScene.h"
#include "MainScene.h"

#include "GMEngine/Application.h"
#include "GMEngine/SceneManager.h"
#include "GMEngine/StringUtil.h"

#include "NetworkCore/Ipv4Endpoint.h"

#include "GameProtocol/GamePackets.h"

#include <utility>

namespace gm
{
	bool NetworkDemoGameInstance::RequestJoin(const std::wstring& address, std::uint16_t portNumber, const std::wstring& nickname)
	{
		if (_state != State::Idle)
			return false;

		std::string utf8Nickname = WideToUtf8(nickname);
		if (utf8Nickname.empty() || utf8Nickname.size() > MaxNicknameByteLength)
			return false;

		Ipv4Endpoint endpoint;
		if (endpoint.Assign(address, portNumber) == false)
			return false;

		if (_clientService.Connect(endpoint) == false)
			return false;

		_clientNickname = std::move(utf8Nickname);
		_state = State::Connecting;

		return true;
	}

	bool NetworkDemoGameInstance::OnInitialize()
	{
		if (_winsockRuntime.Initialize() == false)
			return false;

		if (_clientService.Initialize([this](PacketView packet) { HandlePacket(packet);}) == false)
			return false;

		SetupScenes();
		return true;
	}

	void NetworkDemoGameInstance::OnTick(float deltaTime)
	{
		_clientService.Tick();

		TcpClientService::State clientServiceState = _clientService.GetState();
		if (_state == State::Connecting && clientServiceState == TcpClientService::State::Connected)
		{
			if (SendJoinPacket() == true)
				_state = State::WaitingForJoinAccepted;
		}
		else if (_state == State::Connecting && clientServiceState == TcpClientService::State::Idle)
		{
			_state = State::Idle;
			_clientNickname.clear();
		}
	}

	void NetworkDemoGameInstance::SetupScenes()
	{
		SceneManager& sceneManager = APPLICATION.GetSceneManager();
		sceneManager.CreateScene<TitleScene>(L"TitleScene");
		sceneManager.CreateScene<MainScene>(L"MainScene");

		sceneManager.RequestSceneChange(L"TitleScene");
	}

	void NetworkDemoGameInstance::HandlePacket(PacketView packet)
	{
		GM_LOG("Packet");
	}

	bool NetworkDemoGameInstance::SendJoinPacket()
	{
		const std::span<const std::byte> view = std::as_bytes(std::span{ _clientNickname });
		return _clientService.Send(static_cast<uint16_t>(PacketId::C2S_JoinRequest), view);
	}
}
