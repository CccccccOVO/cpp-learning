#include <iostream>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

int main() {
    // Initialize Winsock
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // Create a socket
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cout << "Failed to create socket." << std::endl;
        return 1;
    }

    // Set the server address
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8888);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to the server
    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cout << "Failed to connect to the server." << std::endl;
        return 1;
    }
    std::cout << "Connected to the server." << std::endl;

    // Send and receive messages
    while(true) {
    // Send message
    std::string message;
    std::cout << "Enter message: ";
    std::getline(std::cin, message);
    if(message == "quit") {
        break;
    }

    send(clientSocket, message.c_str(), message.size(), 0);

    // Receive reply
    char buffer[1024];
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        std::cout << "Server reply: " << buffer << std::endl;
    }
    else {
        std::cout << "Server disconnected." << std::endl;
        break;
    }
 }
    // Close the socket
    closesocket(clientSocket);
    WSACleanup();
    return 0;
}