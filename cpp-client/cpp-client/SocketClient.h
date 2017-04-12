#pragma once
#include "Config.h"
#include <thread>
#include <mutex>
#ifdef WIN32
#include <WinSock2.h>
#pragma comment(lib, "WS2_32.lib")
#define HSocket SOCKET
#elif __APPLE__
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define HSocket int
#endif

class SocketClient
{
public:
	SocketClient(void);
	~SocketClient(void);



	bool connectServer(const char* serverIP, unsigned short port);

	void sendMsg(const char* data, int length);
	void receiveMsg();



private:
	
	void destroy();

	bool init();

	HSocket _socekt;

	void closeConnect(HSocket socket);
	bool error(HSocket socket);
	
	std::mutex _mutex;

	bool _inited;

	void cleanup();

};

