#include "NetworkDemoGameInstance.h"
#include "TitleScene.h"
#include "MainScene.h"

#include "GMEngine/Application.h"
#include "GMEngine/IGraphicsResourceFactory.h"
#include "GMEngine/PathUtil.h"
#include "GMEngine/Resources.h"
#include "GMEngine/SceneManager.h"
#include "GMEngine/SoundWave.h"
#include "GMEngine/StringUtil.h"
#include "GMEngine/Texture.h"

#include "NetworkCore/Ipv4Endpoint.h"

#include "GameProtocol/GamePackets.h"

#include <array>
#include <memory>
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

		LoadResources();
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

	void NetworkDemoGameInstance::LoadResources()
	{
		constexpr std::array texturePaths =
		{
			L"Resources/Title/BG.png",
			L"Resources/Character/Idle.png",
			L"Resources/Character/Jump.png",
			L"Resources/Character/Walk.png"
		};

		Resources& resources = APPLICATION.GetResources();
		IGraphicsResourceFactory& resourceFactory = APPLICATION.GetGraphicsResourceFactory();

		for (const std::wstring& texturePath : texturePaths)
		{
			const std::wstring textureKey = GetFileNameWithoutExtension(texturePath);
			if (resources.Find<Texture>(textureKey))
				continue;

			TextureLoadDesc desc{};
			desc.path = texturePath;
			desc.colorSpace = TextureColorSpace::SRGB;

			std::shared_ptr<Texture> texture = resourceFactory.LoadTexture(desc);
			GM_ASSERT_RETURN(texture, "%ls Texture 로드에 실패했습니다.", texturePath.c_str());
			GM_ASSERT_RETURN(resources.Add(textureKey, texture), "%ls Texture 등록에 실패했습니다.", textureKey.c_str());
		}

		struct CueInfo
		{
			const wchar_t* resourceKey = nullptr;
			const wchar_t* filePath = nullptr;
		};

		constexpr std::array<CueInfo, 3> CueInfos =
		{
			{	L"Title.BG", L"Resources/Sound/Title.mp3",
				L"Title.Click", L"Resources/Sound/BtMouseClick.mp3",
				L"Main.Jump", L"Resources/Sound/Jump.mp3"
			},
		};

		for (const CueInfo& info : CueInfos)
		{
			if (resources.Find<SoundWave>(info.resourceKey))
				continue;

			SoundWaveDesc desc{};
			desc.path = info.filePath;
			std::shared_ptr<SoundWave> sound = SoundWave::Create(desc);
			GM_ASSERT_RETURN(sound, "%ls sound 로드에 실패했습니다.", info.filePath);
			GM_ASSERT_RETURN(resources.Add(info.resourceKey, sound), "%ls sound 등록에 실패했습니다.", info.resourceKey);
		}
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
