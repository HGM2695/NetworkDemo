#include <iostream>
#include <vector>
#include <array>

#include "GMEngine/WindowsCore.h"
#include "NetworkCore/WinsockRuntime.h"
#include "NetworkCore/TcpListener.h"
#include "NetworkCore/Ipv4Endpoint.h"
#include "NetworkCore/TcpSocket.h"

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    gm::WinsockRuntime winsockRuntime{};
    if (winsockRuntime.Initialize() == false)
    {
        std::cout << "winSock 초기화 실패" << std::endl;
        return 0;
    }

    std::cout << "WinSock 초기화 성공" << std::endl;

    constexpr std::uint16_t port = 49900;
    gm::Ipv4Endpoint endpoint{};
    endpoint.AssignAny(port);

    gm::TcpListener listener{};
    if (listener.StartListening(endpoint) == false)
    {
        std::cout << "listener 초기화 실패" << std::endl;
        return 0;
    }

    float accTime = 0.f;
    ULONGLONG prevTick = GetTickCount64();

    std::vector<gm::TcpSocket> v;
    while (true)
    {
        if (GetAsyncKeyState(VK_ESCAPE) < 0)
            break;

        ULONGLONG curTick = GetTickCount64();
        accTime += static_cast<float>(curTick - prevTick) / 1000.f;
        prevTick = curTick;

        if (accTime >= 0.5f)
        {
            accTime = 0.f;
            for (gm::TcpSocket& acceptedSocket : v)
            {
                std::string message = "들리십니까? 여기는 서버입니다.";
                acceptedSocket.TrySend(std::as_bytes(std::span{ message }));
            }
        }

        gm::TcpSocket socket{};
        if (listener.TryAccept(socket) == gm::TcpListener::AcceptResult::Accepted)
            v.push_back(std::move(socket));

        for (gm::TcpSocket& acceptedSocket : v)
        {
            std::array<std::byte, 1024> buffer{};
            const gm::TcpSocket::IoResult result = acceptedSocket.TryReceive(buffer);

            if (result.status == gm::TcpSocket::IoStatus::Transferred)
            {
                const char* data = reinterpret_cast<const char*>(buffer.data());

                std::cout << data << '\n';
            }
        }
    }
}
