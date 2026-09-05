#include "NameTagWidget.h"

#include "GMEngine/Border.h"
#include "GMEngine/CanvasPanel.h"
#include "GMEngine/TextBlock.h"

namespace gm
{
	NameTagWidget::NameTagWidget(const std::wstring& nickname) : _nickname(nickname)
	{
	}

	void NameTagWidget::SetNickname(const std::wstring& nickname)
	{
		_nickname = nickname;
		if (_nicknameText != nullptr)
			_nicknameText->SetText(_nickname);
	}

	std::unique_ptr<Widget> NameTagWidget::BuildWidgetTree()
	{
		auto root = CreateNamedRootWidget<CanvasPanel>(L"NameTagRoot");
		root->SetSize(Vector2{ 180.f, 26.f });

		Border* background = root->AddChild<Border>();
		background->SetSizeRule(WidgetSizeRule::FillParent);
		background->SetBackgroundColor(ColorFromRGBA(20, 20, 20, 180));
		background->SetOutlineColor(Colors::Black);

		_nicknameText = background->AddNamedChild<TextBlock>(L"NicknameText");
		_nicknameText->SetText(_nickname);
		_nicknameText->SetHorizonAlign(TextHorizontalAlignment::Center);
		_nicknameText->SetVerticalAlign(TextVerticalAlignment::Center);
		_nicknameText->SetColor(Colors::White);
		_nicknameText->SetSize(18.f);

		return root;
	}

}
