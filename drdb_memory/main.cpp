
#include <stdio.h>
#include <csignal>
#include "Listener.h"
SourceNodeContext myContext;


// Signal handler function
void signalHandler(int signal) {
	std::cout << "Caught signal " << signal << " (Ctrl+C triggered)." << std::endl;

	// Do your stuff here
	std::cout << "Performing cleanup..." << std::endl;
	closesocket(myContext.CommandListenSocket);
	WSACleanup();
	// Exit the program
	exit(signal);
}

/// <summary>
///  This is the main thread of the application.
/// </summary>
/// <param name="argc"></param>
/// <param name="argv"></param>
/// <returns></returns>
/// 
int main(int argc, char* argv[]) {

	bool opts = false;
	bool bDebug = false;
	std::signal(SIGINT, signalHandler);
	// loop through the command line arguments (future)
	for (int i = 1; i < argc; ++i) {
		std::string arg = argv[i];
		if (arg == "-d") {
			bDebug = true;
			std::cout << "Option " << arg << " was discovered!" << std::endl;
		}
	}

	//initalize Listening Socket
	NTSTATUS res = initializeListenSocket(&myContext);
	if (res != ERROR_SUCCESS) {
		printf("Failed to initailizeClientSocket.\n Error %s, %lu\n", __FILE__, __LINE__);
		WSACleanup();
		return res;
	}

	if (listen(myContext.CommandListenSocket, SOMAXCONN) == SOCKET_ERROR) {
		printf("Listen failed with error: %ld\n", WSAGetLastError());
		closesocket(myContext.CommandListenSocket);
		WSACleanup();
		return 1;
	}

	while (1) {
		std::cout << "Waiting for incoming connections..." << std::endl;

		// Step 5: Accept a connection
		SOCKET ClientSocket = accept(myContext.CommandListenSocket, NULL, NULL);
		if (ClientSocket == INVALID_SOCKET) {
			std::cerr << "Accept failed: " << WSAGetLastError() << std::endl;
			closesocket(myContext.CommandListenSocket);
			WSACleanup();
			return 1;
		}
		std::cout << "Client connected." << std::endl
			;
		res = handleCommandListenSocket(&myContext, &ClientSocket);

		// Step 7: Cleanup
		closesocket(ClientSocket);
	}
	
	shutdownListenSocket(&myContext);

	return 0;

}