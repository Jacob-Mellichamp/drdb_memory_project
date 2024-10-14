#include "Listener.h"
#include "Source.h"

NTSTATUS initializeListenSocket(SourceNodeContext* SN_context) {
	// Winsock Initialize
// Info for winsock Coding: https://learn.microsoft.com/en-us/windows/win32/WinSock/windows-sockets-start-page-2

	WSADATA wsaData;
	int result;

	result = WSAStartup(MAKEWORD(2, 2), &wsaData); // version 2.2 socket
	if (result != 0) {
		printf("WSAStartup failed: %d\n", result);
		return 1L;
	}

	struct addrinfo* res = NULL, * ptr = NULL, hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;


	// Resolve the local address and port to be used by the server
	result = getaddrinfo(NULL, DEFAULT_PORT, &hints, &res);
	if (result != 0) {
		printf("getaddrinfo failed: %d\n", result);
		WSACleanup();
		return 1L;
	}

	SN_context->CommandListenSocket = INVALID_SOCKET;
	// Create a SOCKET for the server to listen for client connections
	SN_context->CommandListenSocket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (SN_context->CommandListenSocket == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(res);
		WSACleanup();
		return 1L;
	}

	// Setup the TCP listening socket
	result = bind(SN_context->CommandListenSocket, res->ai_addr, (int)res->ai_addrlen);
	if (result == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(res);
		closesocket(SN_context->CommandListenSocket);
		WSACleanup();
		return 1L;
	}

	freeaddrinfo(res);
	return ERROR_SUCCESS;
}

// shutdown the node
NTSTATUS shutdownListenSocket(SourceNodeContext* SN_context) {
	closesocket(SN_context->CommandListenSocket);
	WSACleanup();
	return ERROR_SUCCESS;
}

//
//  Handle an incoming command request.
NTSTATUS handleCommandListenSocket(SourceNodeContext* SN_context, SOCKET* ClientSocket) {
	// Step 6: Receive data from the client




	//// Receive until the peer shuts down the connection
	//do {

	//	iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
	//	if (iResult > 0) {
	//		printf("Bytes received: %d\n", iResult);

	//		// Echo the buffer back to the sender
	//		iSendResult = send(ClientSocket, recvbuf, iResult, 0);
	//		if (iSendResult == SOCKET_ERROR) {
	//			printf("send failed with error: %d\n", WSAGetLastError());
	//			closesocket(ClientSocket);
	//			WSACleanup();
	//			return 1;
	//		}
	//		printf("Bytes sent: %d\n", iSendResult);
	//	}
	//	else if (iResult == 0)
	//		printf("Connection closing...\n");
	//	else {
	//		printf("recv failed with error: %d\n", WSAGetLastError());
	//		closesocket(ClientSocket);
	//		WSACleanup();
	//		return 1;
	//	}

	//} while (iResult > 0);


	int result;
	char recvbuf[512];
	int recvbuflen = 512;
	result = recv(*ClientSocket, recvbuf, recvbuflen, 0);
	if (result > 0) {
		std::cout << "Bytes received: " << result << std::endl;
		std::cout << "Message: " << std::string(recvbuf, result) << std::endl;
	}

	else if (result == 0) {
		std::cout << "Connection closed." << std::endl;
	}
	else {
		std::cerr << "Recv failed: " << WSAGetLastError() << std::endl;
		return WSAGetLastError();
	}

	return ERROR_SUCCESS;
}