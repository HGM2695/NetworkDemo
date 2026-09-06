#include "ChatBallonWidget.h"

#include "GMEngine/Border.h"
#include "GMEngine/CanvasPanel.h"
#include "GMEngine/TextBlock.h"

#include <algorithm>
#include <cstddef>
#include <utility>

namespace gm
{
	namespace
	{
		constexpr float			BodyWidth = 320.f;
		constexpr float			MinimumBodyHeight = 48.f;
		constexpr float			LineHeight = 24.f;
		constexpr float			VerticalPadding = 12.f;
		constexpr std::size_t	MaxLineCharacterCount = 16;
	}

	std::wstring ChatBallonWidget::WrapMessage(const std::wstring& message) const
	{
		std::wstring result{};
		for (std::size_t offset = 0; offset < message.size(); offset += MaxLineCharacterCount)
		{
			if (offset > 0)
				result += L'\n';

			result.append(message, offset, MaxLineCharacterCount);
		}

		return result;
	}

	void ChatBallonWidget::SetMessage(const std::wstring& message)
	{
		_message = message;
		_accTime = 0.f;
		UpdateLayout();
	}

	std::unique_ptr<Widget> ChatBallonWidget::BuildWidgetTree()
	{
		auto root = CreateNamedRootWidget<CanvasPanel>(L"ChatBallonRoot");
		_root = root.get();

		_body = root->AddNamedChild<Border>(L"ChatBallonBody");
		_body->SetSizeRule(WidgetSizeRule::FillParent);
		_body->SetBackgroundColor(ColorFromRGBA(20, 20, 20, 230));
		_body->SetOutlineColor(Colors::White);

		_messageText = _body->AddNamedChild<TextBlock>(L"ChatBallonMessage");
		_messageText->SetSizeRule(WidgetSizeRule::FillParent);
		_messageText->SetHorizonAlign(TextHorizontalAlignment::Center);
		_messageText->SetVerticalAlign(TextVerticalAlignment::Center);
		_messageText->SetColor(Colors::White);
		_messageText->SetSize(18.f);

		UpdateLayout();
		return root;
	}

	void ChatBallonWidget::OnTick(float deltaTime)
	{
		if (IsVisible() == false)
			return;

		_accTime += deltaTime;
		if (_accTime >= _visibleDuration)
		{
			SetVisible(false);
			_accTime = 0.f;
		}
	}

	void ChatBallonWidget::UpdateLayout()
	{
		if (_root == nullptr || _body == nullptr || _messageText == nullptr)
			return;

		std::wstring wrappedMessage = WrapMessage(_message);
		const std::size_t lineCount = std::count(wrappedMessage.begin(), wrappedMessage.end(), L'\n') + 1;
		const float bodyHeight = std::max(MinimumBodyHeight, static_cast<float>(lineCount) * LineHeight + VerticalPadding * 2.f);

		_root->SetSize(Vector2{ BodyWidth, bodyHeight });
		_root->SetPosition(Vector2{ 0.f, -bodyHeight * 0.5f });
		_messageText->SetText(std::move(wrappedMessage));
	}
}