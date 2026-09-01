#include "TitleScene.h"
#include "ConnectWidget.h"

#include <array>

#include "GMEngine/GameObject.h"
#include "GMEngine/CameraComponent.h"
#include "GMEngine/CameraManager.h"
#include "GMEngine/Application.h"
#include "GMEngine/UIManager.h"
#include "GMEngine/PathUtil.h"
#include "GMEngine/Texture.h"
#include "GMEngine/Resources.h"
#include "GMEngine/IGraphicsResourceFactory.h"

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
		UIManager& uiManager = APPLICATION.GetUIManager();
		uiManager.ClearViewportWidgets();
		uiManager.AddUserWidget<ConnectWidget>();
	}

	void TitleScene::OnExit()
	{
		APPLICATION.GetUIManager().ClearViewportWidgets();
	}

	void TitleScene::LoadResources()
	{
		constexpr std::array texturePaths =
		{
			L"Resources/Title/BG.png"
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
	}
}
