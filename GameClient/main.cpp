// GameClient.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "GMEngine/WindowsCore.h"
#include "GMEngine/Application.h"
#include "GMEngine/GraphicsTypes.h"

#include "NetworkDemoGameInstance.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR    lpCmdLine, _In_ int       nCmdShow)
{
    gm::ApplicationDesc desc;
    desc.instance = hInstance;
    desc.title = L"Maple Chat";
    desc.className = L"MapleChatWindowClass";
    desc.width = 1600;
    desc.height = 900;
    desc.showCommand = nCmdShow;

    GM_ASSERT_RETURN_VAL(APPLICATION.Initialize(desc, std::make_unique<gm::NetworkDemoGameInstance>()), -1, "Application 초기화에 실패했습니다.");
    APPLICATION.Run();

    return 0;
}
