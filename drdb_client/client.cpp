#include "client.h"
// WIN32_LEAN_AND_MEAN  not needed here -- define this macro if Windows.h is used. 


/*
	Ensure that the build environment links to the Winsock Library file Ws2_32.lib.
	Applications that use Winsock must be linked with the Ws2_32.lib library file.
	The #pragma comment indicates to the linker that the Ws2_32.lib file is needed.
*/
#pragma comment(lib, "Ws2_32.lib")


/// <summary>
///  This is the main thread of the application.
/// </summary>
/// <param name="argc"></param>
/// <param name="argv"></param>
/// <returns></returns>
int main(int argc, char* argv[])
{

    SOCKET ConnectSocket = INVALID_SOCKET;
    char recvbuf[BUFF_LENGTH];
    int recvbuflen = BUFF_LENGTH;

    int iResult;
    NTSTATUS res;

    // Validate the parameters
    if (argc != 2) {
        printf("usage: %s server-name\n", argv[0]);
        return 1;
    }

    // initializeSocket
    res = initializeClientSocket(&ConnectSocket, argv[1], LISTEN_PORT);
    if (res != ERROR_SUCCESS) {
        printf("Failed to initailizeClientSocket.\n Error %s, %lu\n", __FILE__, __LINE__);
        WSACleanup();
        return res;
    }
    // send a ping!
    const char* sendbuf = "ping";
    res = sendCommandClientSocket(&ConnectSocket, sendbuf, (int)strlen(sendbuf));
    if (res != ERROR_SUCCESS) {
        printf("Failed when Calling sendCommand -- %s.\n Error %s, %lu\n", sendbuf, __FILE__, __LINE__);
        WSACleanup();
        return res;
    }

    // shutdownSocket
    res = shutdownClientSocket(&ConnectSocket, recvbuf, recvbuflen);

    if (recvbuf) {
        printf("Shutdown Received: %s\n", recvbuf);
    }
    return 0;
}