// main.cpp : Defines the entry point for the console application.
//

#include "Config.h"
#include "SocketServer.h"

int main()
{

	SocketServer* ss = SocketServer::getInstance();
	ss->startServer();

	return 0;
}

