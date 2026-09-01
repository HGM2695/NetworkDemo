#include "NetworkDemoGameInstance.h"
#include "TitleScene.h"
#include "MainScene.h"

#include "GMEngine/Application.h"
#include "GMEngine/SceneManager.h"

namespace gm
{
	bool NetworkDemoGameInstance::OnInitialize()
	{
		SetupScenes();

		return true;
	}

	void NetworkDemoGameInstance::SetupScenes()
	{
		SceneManager& sceneManager = APPLICATION.GetSceneManager();
		sceneManager.CreateScene<TitleScene>(L"TitleScene");
		sceneManager.CreateScene<MainScene>(L"MainScene");

		sceneManager.RequestSceneChange(L"TitleScene");
	}
}
