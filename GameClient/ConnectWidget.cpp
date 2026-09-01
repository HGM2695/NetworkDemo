#include "ConnectWidget.h"
#include "GMEngine/CanvasPanel.h"
#include "GMEngine/Image.h"
#include "GMEngine/Application.h"

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

		return root;
	}
}
