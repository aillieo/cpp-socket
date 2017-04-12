#pragma once

#include "Config.h"

#ifdef WIN32
#include <WinSock2.h>
#pragma comment(lib, "WS2_32.lib")
#define HSocket SOCKET
#elif __APPLE__
#include <sys/socket.h>
#define HSocket int
#endif

class SocketServer
{

public:

	static SocketServer* getInstance();
	static void destroyInstance();


	bool startServer(unsigned short port);
	void closeConnect(HSocket socket);

	void cleanup();

private:

	SocketServer(void);
	~SocketServer(void);

	static SocketServer* _instance;

	HSocket _socket;
	unsigned short _port;

	bool error(HSocket socket);

	void _acceptClient();

	void _onNewClientConnected(HSocket socket);

};

