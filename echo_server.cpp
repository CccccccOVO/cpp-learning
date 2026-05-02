#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#pragma comment(lib, "ws2_32.lib")

void handleClient(SOCKET clientSocket) {
    char buffer[1024];
    while(true){
        //receive message from client
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if(bytesReceived<=0){
            std::cout<<"Client disconnected." << std::endl;
            break;
        }
        buffer[bytesReceived] = '\0'; // Null-terminate the received message
        std::cout<<"Received from client: " << buffer << std::endl;
        //echo message back to client
        send(clientSocket, buffer, bytesReceived, 0);
    }
    closesocket(clientSocket);
}
int main() {
    // Initialize Winsock
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // Create a socket
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cout<<"Failed to create socket."<<std::endl;
        return 1;
    }
    std::cout<<"Socket created successfully." <<std::endl;

    // Bind the socket to an IP address and port
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8888);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cout<<"Bind failed." << std::endl;
        return 1;
    }
    std::cout<<"Socket bound to port 8888." << std::endl;

    // Start listening
    if(listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cout<<"Listen failed." << std::endl;
        return 1;
    }
    std::cout<<"Listening has started." << std::endl;

    std::cout<<"Starting Echo Server..." << std::endl;

    while(true) {
    //Accept client connections
    SOCKET clientSocket= accept(serverSocket, NULL, NULL);
    if(clientSocket == INVALID_SOCKET) {
        std::cout<<"Accept failed." << std::endl;
        return 1;
    }
    std::cout<<"Client connected." << std::endl;

    std::thread t(handleClient, clientSocket);
    t.detach();
    }
    closesocket(serverSocket);
    WSACleanup();
    return 0;
}