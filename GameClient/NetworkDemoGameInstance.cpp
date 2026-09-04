#include "NetworkDemoGameInstance.h"
#include "TitleScene.h"
#include "MainScene.h"

#include "GMEngine/Application.h"
#include "GMEngine/SceneManager.h"

namespace gm
{
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

	}
}
