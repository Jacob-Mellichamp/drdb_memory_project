/* 
* This File Will contain functionality and structures for the listener node.
* 
*   [ source ] ----> data ----> [ listener ]
* 
*/

#pragma once
#include <iostream>
#include <stdlib.h>
#include "Source.h"
// WIN32_LEAN_AND_MEAN  not needed here -- define this macro if Windows.h is used. 


/*
	Ensure that the build environment links to the Winsock Library file Ws2_32.lib.
	Applications that use Winsock must be linked with the Ws2_32.lib library file.
	The #pragma comment indicates to the linker that the Ws2_32.lib file is needed.
*/
#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "27015"


// ---------------------------------------------------------- //
// ------------- Declare Listening Functions ---------------- //
// ---------------------------------------------------------- //

// initialize socket for connecting
NTSTATUS initializeListenSocket(SourceNodeContext*);

// shutdown socket
NTSTATUS shutdownListenSocket(SourceNodeContext*);

// send network command
NTSTATUS handleCommandListenSocket(SourceNodeContext*, SOCKET*);