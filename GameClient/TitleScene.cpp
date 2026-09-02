#include "TitleScene.h"
#include "ConnectWidget.h"

#include <array>

#include "GMEngine/GameObject.h"
#include "GMEngine/CameraComponent.h"
#include "GMEngine/CameraManager.h"
#include "GMEngine/Application.h"
#include "GMEngine/WidgetManager.h"
#include "GMEngine/PathUtil.h"
#include "GMEngine/Texture.h"
#include "GMEngine/Resources.h"
#include "GMEngine/SoundWave.h"
#include "GMEngine/IGraphicsResourceFactory.h"
#include "GMEngine/AudioStatics.h"
#include "GMEngine/Input.h"

#include "NetworkCore/WinsockRuntime.h"
#include "NetworkCore/Ipv4Endpoint.h"
#include "NetworkCore/TcpConnector.h"
#include "NetworkCore/TcpSocket.h"

namespace gm
{
	void TitleScene::OnInitialize()
	{
		GameObject* cameraObject = SpawnGameObject<GameObject>();
		CameraComponent* camera = cameraObject->AddComponent<CameraComponent>();

		const float width = static_cast<float>(APPLICATION.GetWidth());
		const float height = static_cast<float>(APPLICATION.GetHeight());

		camera->SetOrthographic(width, height);
		GetCameraManager()->RegisterCamera(L"TitleCamera", camera);

		LoadResources();
	}

	void TitleScene::OnEnter()
	{
		GetCameraManager()->SetActiveCamera(L"TitleCamera");
		WidgetManager& widgetManager = APPLICATION.GetWidgetManager();
		widgetManager.ClearViewportWidgets();
		widgetManager.AddUserWidget<ConnectWidget>();

		PlayBGM(L"Title.BG");
	}

	void TitleScene::OnExit()
	{
		APPLICATION.GetWidgetManager().ClearViewportWidgets();
		StopBGM();
	}

	void TitleScene::OnTick(float deltaTime)
	{
		static WinsockRuntime winsockRuntime{};
		static TcpConnector connector{};
		static Ipv4Endpoint serverEndpoint{};
		static TcpSocket socket{};
		static constexpr std::uint16_t port = 49900;

		if (connector.TryTakeSocket(socket) == TcpConnector::ConnectResult::Complete)
			GM_LOG("소켓 연결 성공");

		if (APPLICATION.GetInput().IsKeyDown(KeyCode::Space))
		{
			GM_ASSERT(winsockRuntime.Initialize(), "WinSock 초기화 실패");
			GM_ASSERT(serverEndpoint.Assign(L"127.0.0.1", port), "127.0.0.1 루프백 주소 할당 실패");
			GM_ASSERT(connector.StartConnect(serverEndpoint), "127.0.0.1 루프백 주소 연결 요청 실패");
		}
		else if (APPLICATION.GetInput().IsKeyDown(KeyCode::S) && socket.IsValid())
		{
			std::string message = "들리십니까? 여기는 클라이언트 입니다.";
			socket.TrySend(std::as_bytes(std::span{ message }));
		}

		std::array<std::byte, 1024> buffer{};
		const gm::TcpSocket::IoResult result = socket.TryReceive(buffer);
		if (result.status == gm::TcpSocket::IoStatus::Transferred)
		{
			const char* data = reinterpret_cast<const char*>(buffer.data());
			GM_LOG("서버로부터 받았습니다. %s", data);
		}
		else if (result.status == gm::TcpSocket::IoStatus::Closed)
		{
			GM_LOG("서버가 종료되었습니다.");
		}
	}

	void TitleScene::LoadResources()
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

		constexpr std::array<CueInfo, 1> CueInfos =
		{
			{ L"Title.BG", L"Resources/Sound/Title.mp3" },
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
}
