#include <iostream>
#include <cstring>
#include <sys/types.h>
#ifdef _WIN32
    #include <winsock2.h>
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
#endif

const int PORT = 54000;

int main() {
#ifdef _WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2,2), &wsaData);
#endif

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "Error creating socket\n";
        return 1;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Localhost

    if (connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Connection failed\n";
        return 1;
    }

    std::string message = "Hello from client!";
    send(sock, message.c_str(), message.size(), 0);

    char buffer[1024];
    std::memset(buffer, 0, sizeof(buffer));
    recv(sock, buffer, sizeof(buffer), 0);
    std::cout << "Server reply: " << buffer << "\n";

#ifdef _WIN32
    closesocket(sock);
    WSACleanup();
#else
    close(sock);
#endif

    return 0;
}
