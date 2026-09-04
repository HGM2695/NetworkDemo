#include "TitleScene.h"
#include "ConnectWidget.h"
#include "NetworkDemoGameInstance.h"

#include <array>
#include <cstdint>
#include <string>

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
#include "GMEngine/Button.h"
#include "GMEngine/InputTextBox.h"

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
		_connectWidget = widgetManager.AddUserWidget<ConnectWidget>();
		Button* button = _connectWidget->FindWidget<Button>(L"connectButton");
		button->OnClicked.Subscribe(_buttonClickConnection, [this](const ButtonClickedEvent&)
			{
				this->OnConnectButtonClicked();
			});

		PlayBGM(L"Title.BG");
	}

	void TitleScene::OnExit()
	{
		APPLICATION.GetWidgetManager().ClearViewportWidgets();
		StopBGM();
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

	void TitleScene::OnConnectButtonClicked()
	{
		if (_connectWidget == nullptr)
			return;

		const std::wstring& address = _connectWidget->FindWidget<InputTextBox>(L"addressInput")->GetText();
		const std::wstring& port = _connectWidget->FindWidget<InputTextBox>(L"portInput")->GetText();
		const std::wstring& name = _connectWidget->FindWidget<InputTextBox>(L"nameInput")->GetText();

		if (address.empty() || port.empty() || name.empty())
			return;

		const std::uint16_t portNumber = static_cast<std::uint16_t>(std::stoul(port));
		static_cast<NetworkDemoGameInstance&>(APPLICATION.GetGameInstance()).RequestJoin(address, portNumber, name);
	}
}
