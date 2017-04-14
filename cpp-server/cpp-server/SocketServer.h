#pragma once

#include "Config.h"

#ifdef WIN32
#include <WinSock2.h>
#pragma comment(lib, "WS2_32.lib")
#define HSocket SOCKET
#define socklen_t int
#elif __APPLE__
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#define HSocket int
#endif
#include <mutex>
#include <queue>
#include "Message.h"
#include "MessageHandler.h"

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



	bool error(HSocket socket);

	void _acceptClient();

	void _onNewClientConnected(HSocket socket);

	void _handleClientConnection(HSocket socket);

	MessageHandler* _messageHandler;


	std::mutex _mutex;

	std::mutex _messageQueueMutex;


};

