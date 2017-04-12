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
#include <mutex>
#include <queue>
#include "Message.h"

class SocketServer
{

public:

	static SocketServer* getInstance();
	static void destroyInstance();


	bool initServer();
	
	void startServer();
	
	void closeConnect(HSocket socket);

	void cleanup();

private:

	SocketServer(void);
	~SocketServer(void);

	static SocketServer* _instance;

	HSocket _socketServer;

	std::vector<HSocket> _clients;

	bool error(HSocket socket);

	void _acceptClient();

	void _onNewClientConnected(HSocket socket);

	void _handleClientConnection(HSocket socket);

	void _broadcastMessage();

	std::mutex _messageQueueMutex;
	std::mutex _mutex;

	std::queue<Message> _messageQueue;

};

