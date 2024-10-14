#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>

#define MEMORY_ROWS 100
#define MEMORY_COLS 100

struct ClientMirrors {
	SOCKET clientSocket;
	SOCKET* next;
};

struct SourceNodeContext {
	SOCKET CommandListenSocket;
	ClientMirrors mirrorList;
	int** diskSectors;
};


