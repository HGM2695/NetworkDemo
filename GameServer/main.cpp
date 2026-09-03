#include <iostream>
#include "GameServerApplication.h"

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    gm::GameServerApplication app;
    if (app.Initialize() == false)
    {
        std::cout << "ServerService 초기화 실패" << std::endl;
        return -1;
    }

    std::cout << "WinSock 초기화 성공" << std::endl;

    app.Run();

    return 0;
}
