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
