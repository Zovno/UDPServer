#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <WS2tcpip.h>
#include <cstring>

#pragma comment(lib, "ws2_32.lib")

void countLetters(const char* input, char* output) {
    // Инициализируем массив счетчиков букв в нули
    int counts[6] = { 0 };

    // Перебираем символы во входной строке
    for (int i = 0; input[i] != '\0'; ++i) {
        switch (input[i]) {
        case 'w':
            counts[0]++;
            break;
        case 'i':
            counts[1]++;
            break;
        case 'n':
            counts[2]++;
            break;
        case 'd':
            counts[3]++;
            break;
        case 'o':
            counts[4]++;
            break;
        case 's':
            counts[5]++;
            break;
            // Другие символы игнорируем
        }
    }

    // Записываем результат в выходной массив
    std::sprintf(output, "%dw %di %dn %dd %do %ds", counts[0], counts[1], counts[2], counts[3], counts[4], counts[5]);

}

int main() {
    // Initialize Winsock
    WSADATA wsData;
    WORD ver = MAKEWORD(2, 2);

    if (WSAStartup(ver, &wsData) != 0) {
        std::cerr << "Can't initialize winsock" << std::endl;
        return 1;
    }

    // Create a socket
    SOCKET udpSocket = socket(AF_INET, SOCK_DGRAM, 0);

    if (udpSocket == INVALID_SOCKET) {
        std::cerr << "Can't create socket" << std::endl;
        return 1;
    }

    // Bind the socket to an address and port
    sockaddr_in serverHint;
    serverHint.sin_addr.S_un.S_addr = ADDR_ANY;
    serverHint.sin_family = AF_INET;
    serverHint.sin_port = htons(54000);  // Указываем порт, на котором сервер будет слушать

    if (bind(udpSocket, (sockaddr*)&serverHint, sizeof(serverHint)) == SOCKET_ERROR) {
        std::cerr << "Can't bind socket" << std::endl;
        return 1;
    }

    sockaddr_in client;
    int clientLength = sizeof(client);
    char buf[1024];
    char rez[1024];

    while (true) {
        ZeroMemory(&client, clientLength);
        ZeroMemory(buf, 1024);

        // Wait for message
        int bytesIn = recvfrom(udpSocket, buf, 1024, 0, (sockaddr*)&client, &clientLength);
        if (bytesIn == SOCKET_ERROR) {
            std::cerr << "Error receiving from client" << std::endl;
            continue;
        }

        // Display message and client info
        char clientIP[256];
        ZeroMemory(clientIP, 256);

        inet_ntop(AF_INET, &client.sin_addr, clientIP, 256);

        std::cout << "Message received from " << clientIP << " : " << buf << std::endl;

        countLetters(buf, rez);
        // Echo message back to client
        sendto(udpSocket, rez , 18, 0, (sockaddr*)&client, clientLength);
    }

    // Close socket
    closesocket(udpSocket);

    // Cleanup Winsock
    WSACleanup();

    return 0;
}
