#include <iostream>
#include <cstring>
#include <sys/types.h>
#ifdef _WIN32
    #include <winsock2.h>
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <unistd.h>
#endif

const int PORT = 54000;

int main() {
#ifdef _WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2,2), &wsaData);
#endif

    // Create socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        std::cerr << "Error creating socket\n";
        return 1;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    // Bind socket
    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Binding failed\n";
        return 1;
    }

    listen(serverSocket, 5);
    std::cout << "Server listening on port " << PORT << "...\n";

    sockaddr_in clientAddr{};
    socklen_t clientLen = sizeof(clientAddr);
    int clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientLen);

    char buffer[1024];
    std::memset(buffer, 0, sizeof(buffer));
    recv(clientSocket, buffer, sizeof(buffer), 0);
    std::cout << "Client says: " << buffer << "\n";

    std::string reply = "Hello from server!";
    send(clientSocket, reply.c_str(), reply.size(), 0);

#ifdef _WIN32
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
#else
    close(clientSocket);
    close(serverSocket);
#endif

    return 0;
}
