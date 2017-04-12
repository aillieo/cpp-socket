#include "SocketServer.h"
#include "ConnectionManager.h"
#include <thread>

SocketServer* SocketServer::_instance = nullptr;

SocketServer::SocketServer(void)
{
}


SocketServer::~SocketServer(void)
{
}

bool SocketServer::startServer(unsigned short port)
{

	ConnectionManager* cm = ConnectionManager::getInstance();
	cm ->init();

	if (_socket != 0)
	{
		this->closeConnect(_socket);
	}


#ifdef WIN32
	WORD wVersionRequested;
	wVersionRequested = MAKEWORD(2, 0);
	WSADATA wsaData;
	int nRet = WSAStartup(wVersionRequested, &wsaData);
	if (nRet != 0) 
	{
		fprintf(stderr, "init error!\n");
		return false;
	}
#endif


	_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (error(_socket))
	{
		printf("socket error!\n");
		_socket = 0;
		return false;
	}

	do 
	{
		struct sockaddr_in sockAddr;
		memset(&sockAddr, 0, sizeof(sockAddr));
		_port = port;

		sockAddr.sin_family = AF_INET;
		sockAddr.sin_port = htons(_port);
		sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		int ret = 0;
		ret = bind(_socket, (const sockaddr*)&sockAddr, sizeof(sockAddr));
		if (ret < 0)
		{
			printf("bind error!");
			break;
		}

		ret = listen(_socket, 5);
		if (ret < 0)
		{
			printf("listen error!");
			break;
		}
		
		char hostName[256];
		gethostname(hostName, sizeof(hostName));
		struct hostent* hostInfo = gethostbyname(hostName);
		char* ip = inet_ntoa(*(struct in_addr *)*hostInfo->h_addr_list);
		
		std::thread th(& SocketServer::_acceptClient, this);
		th.detach();
		

		printf("start server!\n");
		return true;

	} while (0);

	this->closeConnect(_socket);
	_socket = 0;

	return true;
}

SocketServer* SocketServer::getInstance()
{
	if(nullptr == _instance)
	{
		_instance = new SocketServer();
	}
	return _instance;
}

void SocketServer::destroyInstance()
{
	if(!_instance)
	{
		return;
	}
	else
	{
		delete _instance;
	}
}

void SocketServer::closeConnect( HSocket socket )
{

#ifdef WIN32
		closesocket(socket);
#elif __APPLE__
		close(socket);
#endif

}


void SocketServer::_acceptClient()
{

	int len = sizeof(sockaddr);
	struct sockaddr_in sockAddr;
	while (true)
	{	
		HSocket clientSock = accept(_socket, (sockaddr*)&sockAddr, &len);
		if (error(clientSock))
		{
			printf("accept error!\n");
		}
		else
		{
			_onNewClientConnected(clientSock);
		}
	}

}


void SocketServer::cleanup()
{

}



bool SocketServer::error( HSocket socket )
{


	return false;
}



void SocketServer::_onNewClientConnected(HSocket socket)
{
	printf("new connect!\n");

	// new thread

}