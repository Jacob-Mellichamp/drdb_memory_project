#include "client.h"


NTSTATUS initializeClientSocket(SOCKET* ConnectSocket, char* servername,const  char* port) {

    std::cout << "Connecting to\n\tserver: " << servername << "\n\tport: " << port << "\n" << std::endl;
    WSADATA wsaData;
    int iResult;
    *ConnectSocket = INVALID_SOCKET;
    struct addrinfo* result = NULL,* ptr = NULL, hints;


    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return iResult;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo(servername, port, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return iResult;
    }

    // Attempt to connect to an address until one succeeds
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

        // Create a SOCKET for connecting to server
        *ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
            ptr->ai_protocol);
        if (*ConnectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return WSAGetLastError();
        }

        // Connect to server.
        iResult = connect(*ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(*ConnectSocket);
            *ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);
    return ERROR_SUCCESS;
}

NTSTATUS shutdownClientSocket(SOCKET* ConnectSocket, char* recvbuf, int recvbuflen) {
    int iResult;
    memset(recvbuf, '\0', recvbuflen);
    // shutdown the connection since no more data will be sent
    iResult = shutdown(*ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(*ConnectSocket);
        WSACleanup();
        return 1;
    }

    // Receive until the peer closes the connection
    do {

        iResult = recv(*ConnectSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0)
            printf("Bytes received: %d\n", iResult);
        else if (iResult == 0)
            printf("Connection closed\n");
        else
            printf("recv failed with error: %d\n", WSAGetLastError());

    } while (iResult > 0);

    // cleanup
    closesocket(*ConnectSocket);
    WSACleanup();
    return ERROR_SUCCESS;
}

NTSTATUS sendCommandClientSocket(SOCKET* ConnectSocket, const char* command, int sendBufferLen) {
    // Send a command
    int iResult;
    iResult = send(*ConnectSocket, command, (int)strlen(command), 0);
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(*ConnectSocket);
        WSACleanup();
        return 1;
    }

    printf("Bytes Sent: %ld\n", iResult);
    return ERROR_SUCCESS;
}