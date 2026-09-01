#pragma once

#include "Border.h"
#include "Event.h"

#include <string>

namespace gm
{
	class TextBlock;

	struct InputTextSubmittedEvent final : EventType
	{
		std::wstring text{};
	};

	class InputTextBox final : public Border
	{
	public:
		InputTextBox();

		void				SetText(const std::wstring& text);
		const std::wstring& GetText() const { return _text; }
		void				ClearText();

		EventPublisher<InputTextBox, InputTextSubmittedEvent> OnTextSubmitted;

	protected:
		void				OnTick(float deltaTime) override;
		void				OnRender(const WidgetGeometry& geometry) override;

	private:
		bool				ContainsPoint(const Vector2& point) const;
		void				ProcessCommittedTextInput();
		void				ProcessCommittedCharacter(wchar_t character);
		void				CommitCurrentTextInput();
		void				EraseLastCharacter();
		void				UpdateDisplayedText();
		void				UpdateCursor(float deltaTime, bool isFocus);
		void				ResetCursorBlink();

	private:
		TextBlock*			_textBlock = nullptr;
		Border*				_cursor = nullptr;
		std::wstring		_text{};
		WidgetGeometry		_cachedGeometry{};
		std::size_t			_maxLength = 256;
		float				_horizontalPadding = 8.f;
		float				_displayedTextWidth = 0.f;
		float				_cursorBlinkElapsed = 0.f;
		float				_cursorBlinkInterval = 0.5f;
		bool				_hadFocus = false;
	};
}
