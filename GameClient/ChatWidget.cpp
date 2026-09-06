#include "ChatWidget.h"

#include "GMEngine/Application.h"
#include "GMEngine/CanvasPanel.h"
#include "GMEngine/Input.h"
#include "GMEngine/InputTextBox.h"

namespace gm
{
	void ChatWidget::SetInputActive(bool isActive)
	{
		if (_chatInput == nullptr)
			return;

		_chatInput->SetActive(isActive);
		if (isActive == false)
			_chatInput->ClearText();
	}

	bool ChatWidget::IsInputActive() const
	{
		if (_chatInput == nullptr)
			return false;

		return _chatInput->IsActive();
	}

	std::unique_ptr<Widget> ChatWidget::BuildWidgetTree()
	{
		auto root = CreateNamedRootWidget<CanvasPanel>(L"ChatRoot");

		const float viewportWidth = static_cast<float>(APPLICATION.GetWidth());
		const float viewportHeight = static_cast<float>(APPLICATION.GetHeight());
		constexpr float inputWidth = 840.f;
		constexpr float inputHeight = 36.f;
		constexpr float viewportMargin = 30.f;

		root->SetSize(Vector2{ viewportWidth, viewportHeight });
		root->SetPosition(Vector2{ viewportWidth * 0.5f, viewportHeight * 0.5f });

		_chatInput = root->AddNamedChild<InputTextBox>(L"ChatInput");
		_chatInput->SetGeometry(Vector2{ -viewportWidth * 0.5f + viewportMargin + inputWidth * 0.5f, viewportHeight * 0.5f - viewportMargin - inputHeight * 0.5f }, Vector2{ inputWidth, inputHeight });
		_chatInput->SetBackgroundColor(ColorFromRGBA(0, 0, 0, 220));
		_chatInput->SetOutlineColor(Colors::White);

		return root;
	}

	void ChatWidget::OnTick(float)
	{
		if (_chatInput == nullptr)
			return;

		if (_chatInput->IsActive() == false && APPLICATION.GetInput().IsKeyDown(KeyCode::Enter))
		{
			_chatInput->SetActive(true);
		}
	}
}
