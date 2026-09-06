#include "LeaveConfirmWidget.h"

#include "GMEngine/Application.h"
#include "GMEngine/Border.h"
#include "GMEngine/Button.h"
#include "GMEngine/CanvasPanel.h"
#include "GMEngine/TextBlock.h"
#include "GMEngine/Input.h"

namespace gm
{
	std::unique_ptr<Widget> LeaveConfirmWidget::BuildWidgetTree()
	{
		auto root = CreateNamedRootWidget<CanvasPanel>(L"LeaveConfirmRoot");

		const float viewportWidth = static_cast<float>(APPLICATION.GetWidth());
		const float viewportHeight = static_cast<float>(APPLICATION.GetHeight());
		root->SetSize(Vector2{ viewportWidth, viewportHeight });
		root->SetPosition(Vector2{ viewportWidth * 0.5f, viewportHeight * 0.5f });

		Border* panel = root->AddNamedChild<Border>(L"LeaveConfirmPanel");
		panel->SetSize(Vector2{ 420.f, 180.f });
		panel->SetBackgroundColor(ColorFromRGBA(30, 30, 30, 230));
		panel->SetOutlineColor(Colors::White);

		TextBlock* promptText = panel->AddNamedChild<TextBlock>(L"LeavePromptText");
		promptText->SetText(L"종료하시겠습니까?");
		promptText->SetGeometry(Vector2{ 0.f, -40.f }, Vector2{ 380.f, 50.f });
		promptText->SetHorizonAlign(TextHorizontalAlignment::Center);
		promptText->SetVerticalAlign(TextVerticalAlignment::Center);
		promptText->SetSize(24.f);

		Button* yesButton = panel->AddNamedChild<Button>(L"YesButton");
		yesButton->SetGeometry(Vector2{ -80.f, 45.f }, Vector2{ 120.f, 42.f });
		yesButton->SetBackgroundColor(Colors::Green);
		yesButton->OnClicked.Subscribe(_yesButtonClickConnection, [this](const ButtonClickedEvent&) { OnLeaveConfirmed.Publish(LeaveConfirmedEvent{}); });

		TextBlock* yesText = yesButton->AddChild<TextBlock>();
		yesText->SetText(L"Yes");
		yesText->SetSizeRule(WidgetSizeRule::FillParent);
		yesText->SetHorizonAlign(TextHorizontalAlignment::Center);
		yesText->SetVerticalAlign(TextVerticalAlignment::Center);
		yesText->SetColor(Colors::Black);
		yesText->SetSize(20.f);

		Button* noButton = panel->AddNamedChild<Button>(L"NoButton");
		noButton->SetGeometry(Vector2{ 80.f, 45.f }, Vector2{ 120.f, 42.f });
		noButton->SetBackgroundColor(Colors::Red);
		noButton->OnClicked.Subscribe(_noButtonClickConnection, [this](const ButtonClickedEvent&) { SetVisible(false); });

		TextBlock* noText = noButton->AddChild<TextBlock>();
		noText->SetText(L"No");
		noText->SetSizeRule(WidgetSizeRule::FillParent);
		noText->SetHorizonAlign(TextHorizontalAlignment::Center);
		noText->SetVerticalAlign(TextVerticalAlignment::Center);
		noText->SetColor(Colors::Black);
		noText->SetSize(20.f);

		return root;
	}
}
