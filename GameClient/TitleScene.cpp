#include "TitleScene.h"
#include "ConnectWidget.h"
#include "NetworkDemoGameInstance.h"

#include <cstdint>
#include <string>

#include "GMEngine/GameObject.h"
#include "GMEngine/CameraComponent.h"
#include "GMEngine/CameraManager.h"
#include "GMEngine/Application.h"
#include "GMEngine/WidgetManager.h"
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
