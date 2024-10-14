
#include <iostream>
#include <stdlib.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>

#define LISTEN_PORT "27015"
#define BUFF_LENGTH 1024

// initialize socket for connecting
NTSTATUS initializeClientSocket(SOCKET* ConnectSocket, char* servername,const  char* port);

// shutdown socket
NTSTATUS shutdownClientSocket(SOCKET* ConnectSocket, char* recvbuf, int recvbuflen);

// send network command
NTSTATUS sendCommandClientSocket(SOCKET* ConnectSocket, const char* command, int sendBufferLen);