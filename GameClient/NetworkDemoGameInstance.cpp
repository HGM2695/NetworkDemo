#include "NetworkDemoGameInstance.h"
#include "TitleScene.h"
#include "MainScene.h"

#include "GMEngine/Application.h"
#include "GMEngine/IGraphicsResourceFactory.h"
#include "GMEngine/PathUtil.h"
#include "GMEngine/Resources.h"
#include "GMEngine/SceneManager.h"
#include "GMEngine/SoundWave.h"
#include "GMEngine/SpriteAnimationClip.h"
#include "GMEngine/StringUtil.h"
#include "GMEngine/Texture.h"

#include "NetworkCore/Ipv4Endpoint.h"

#include "GameProtocol/GamePackets.h"

#include <array>
#include <cstring>
#include <memory>
#include <span>
#include <string>
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

		_playerNickname = std::move(utf8Nickname);
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
			_playerNickname.clear();
		}
	}

	void NetworkDemoGameInstance::HandlePacket(PacketView packet)
	{
		PacketId packetId = static_cast<PacketId>(packet.header.packetId);

		switch (packetId)
		{
		case gm::PacketId::S2C_JoinAccepted:
		{
			if (packet.payload.size() != sizeof(S2CJoinAccepted))
				return;

			S2CJoinAccepted joinAccepted{};
			memcpy(&joinAccepted, packet.payload.data(), sizeof(S2CJoinAccepted));

			_playerId = joinAccepted.playerId;
			_state = State::Joined;
			APPLICATION.GetSceneManager().RequestSceneChange(L"MainScene");

			break;
		}
		case gm::PacketId::S2C_PlayerJoined:
		{
			auto view = packet.payload;
			const std::size_t payloadSize = view.size();
			const std::size_t prefixSize = sizeof(S2CPlayerJoinedPrefix);
			if (payloadSize < prefixSize)
				return;

			const std::size_t nickNameSize = payloadSize - prefixSize;
			if (nickNameSize > MaxNicknameByteLength)
				return;

			S2CPlayerJoinedPrefix prefix{};
			memcpy(&prefix, packet.payload.data(), sizeof(prefix));

			std::string utf8NickName;
			utf8NickName.resize(nickNameSize);
			memcpy(utf8NickName.data(), view.last(nickNameSize).data(), nickNameSize);

			_mainScene->SpawnPlayer(prefix.playerId, Vector2{ prefix.positionX, prefix.positionY }, Utf8ToWide(utf8NickName.data()));

			break;
		}
		case gm::PacketId::S2C_PlayerLeft:
		{

			break;
		}
		case gm::PacketId::S2C_PlayerMoved:
		{
			break;

		}
		case gm::PacketId::S2C_ChatBroadcast:
		{

			break;
		}
		default:
			break;
		}
	}

	bool NetworkDemoGameInstance::SendJoinPacket()
	{
		const std::span<const std::byte> view = std::as_bytes(std::span{ _playerNickname });
		return _clientService.Send(static_cast<uint16_t>(PacketId::C2S_JoinRequest), view);
	}

	void NetworkDemoGameInstance::SetupScenes()
	{
		SceneManager& sceneManager = APPLICATION.GetSceneManager();
		sceneManager.CreateScene<TitleScene>(L"TitleScene");
		_mainScene = static_cast<MainScene*>(sceneManager.CreateScene<MainScene>(L"MainScene"));

		sceneManager.RequestSceneChange(L"TitleScene");
	}

	void NetworkDemoGameInstance::LoadResources()
	{
		constexpr std::array texturePaths =
		{
			L"Resources/Title/BG.png",
			L"Resources/Character/Idle.png",
			L"Resources/Character/Jump.png",
			L"Resources/Character/Walk.png",
			L"Resources/BG/florida.png"
		};

		Resources& resources = APPLICATION.GetResources();
		IGraphicsResourceFactory& resourceFactory = APPLICATION.GetGraphicsResourceFactory();

		for (const std::wstring& texturePath : texturePaths)
		{
			const std::wstring textureKey = GetFileNameWithoutExtension(texturePath);

			TextureLoadDesc desc{};
			desc.path = texturePath;
			desc.colorSpace = TextureColorSpace::SRGB;

			std::shared_ptr<Texture> texture = resourceFactory.LoadTexture(desc);
			GM_ASSERT_RETURN(texture, "%ls Texture 로드에 실패했습니다.", texturePath.c_str());
			GM_ASSERT_RETURN(resources.Add(textureKey, texture), "%ls Texture 등록에 실패했습니다.", textureKey.c_str());
		}

		struct SpriteAnimationInfo
		{
			const wchar_t*	resourceKey = nullptr;
			const wchar_t*	textureKey = nullptr;
			uint32			frameCount = 0;
			float			frameDuration = 0.f;
			bool			isLoop = true;
		};

		constexpr int32 SpriteFrameSize = 300;
		constexpr std::array<SpriteAnimationInfo, 3> SpriteAnimationInfos =
		{
			SpriteAnimationInfo{ L"Player.Idle", L"Idle", 3, 0.5f, true },
			SpriteAnimationInfo{ L"Player.Jump", L"Jump", 1, 0.1f, false },
			SpriteAnimationInfo{ L"Player.Walk", L"Walk", 4, 0.2f, true }
		};

		for (const SpriteAnimationInfo& info : SpriteAnimationInfos)
		{
			std::shared_ptr<Texture> texture = resources.Find<Texture>(info.textureKey);
			GM_ASSERT_RETURN(texture, "%ls Texture를 찾을 수 없습니다.", info.textureKey);

			SpriteAnimationClipDesc desc{};
			std::shared_ptr<SpriteAnimationClip> clip = SpriteAnimationClip::Create(desc);
			GM_ASSERT_RETURN(clip, "%ls SpriteAnimationClip 생성에 실패했습니다.", info.resourceKey);

			clip->SetTexture(texture);
			clip->SetLoop(info.isLoop);
			for (uint32 frameIndex = 0; frameIndex < info.frameCount; ++frameIndex)
				clip->AddFrame(SpriteFrame{ IntRect{ static_cast<int32>(frameIndex) * SpriteFrameSize, 0, SpriteFrameSize, SpriteFrameSize }, info.frameDuration });

			GM_ASSERT_RETURN(resources.Add(info.resourceKey, clip), "%ls SpriteAnimationClip 등록에 실패했습니다.", info.resourceKey);
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
			SoundWaveDesc desc{};
			desc.path = info.filePath;
			std::shared_ptr<SoundWave> sound = SoundWave::Create(desc);
			GM_ASSERT_RETURN(sound, "%ls sound 로드에 실패했습니다.", info.filePath);
			GM_ASSERT_RETURN(resources.Add(info.resourceKey, sound), "%ls sound 등록에 실패했습니다.", info.resourceKey);
		}
	}
}
