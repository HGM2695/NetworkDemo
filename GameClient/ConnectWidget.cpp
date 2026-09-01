#include "ConnectWidget.h"

#include "GMEngine/CanvasPanel.h"
#include "GMEngine/Image.h"
#include "GMEngine/Application.h"
#include "GMEngine/Border.h"
#include "GMEngine/TextBlock.h"
#include "GMEngine/InputTextBox.h"

namespace gm
{
	namespace
	{
		constexpr wchar_t BGTextureKey[] = L"BG";
	}

	std::unique_ptr<Widget> ConnectWidget::BuildWidgetTree()
	{
		auto root = CreateNamedRootWidget<CanvasPanel>(L"ConnectRoot");

		const float width = static_cast<float>(APPLICATION.GetWidth());
		const float height = static_cast<float>(APPLICATION.GetHeight());

		root->SetSize(Vector2{ width, height });
		root->SetPosition(Vector2{ width * 0.5f, height * 0.5f });

		Image* backGround = root->AddChild<Image>(BGTextureKey);
		backGround->SetSizeRule(WidgetSizeRule::FillParent);

		Border* panel = root->AddChild<Border>();
		panel->SetSize(Vector2{ 400.f, 200.f });
		panel->SetBackgroundColor(Colors::White);
		panel->SetOutlineColor(Colors::Black);

		TextBlock* addressText = panel->AddChild<TextBlock>();
		addressText->SetText(L"접속할 주소를 입력해주세요");
		addressText->SetPosition(Vector2{ 0.f, -85.f });
		addressText->SetHorizonAlign(TextHorizontalAlignment::Center);
		addressText->SetColor(Colors::Black);
		addressText->SetSize(20.f);

		InputTextBox* addressInput = panel->AddNamedChild<InputTextBox>(L"addressInput");
		addressInput->SetGeometry(Vector2{ 0.f, -35.f }, Vector2{ 300.f, 30.f });
		addressInput->SetOutlineColor(Colors::Black);

		TextBlock* nameText = panel->AddChild<TextBlock>();
		nameText->SetText(L"닉네임을 입력해주세요");
		nameText->SetPosition(Vector2{ 0.f, 0.f });
		nameText->SetHorizonAlign(TextHorizontalAlignment::Center);
		nameText->SetColor(Colors::Black);
		nameText->SetSize(20.f);

		InputTextBox* nameInput = panel->AddNamedChild<InputTextBox>(L"nameInput");
		nameInput->SetGeometry(Vector2{ 0.f, 50.f }, Vector2{ 300.f, 30.f });
		nameInput->SetOutlineColor(Colors::Black);

		return root;
	}
}
