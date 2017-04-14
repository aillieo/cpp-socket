#include "SocketServer.h"
#include <thread>
#include "ConnectionManager.h"

#ifdef __APPLE__
#define gethostname gethostbyname2
#endif

using std::lock_guard;
using std::mutex;
using std::vector;

SocketServer* SocketServer::_instance = nullptr;

SocketServer::SocketServer(void)
{
}


SocketServer::~SocketServer(void)
{
	cleanup();
}

bool SocketServer::initServer()
{

	if (_socketServer != 0)
	{
		this->closeConnect(_socketServer);
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


	_socketServer = socket(AF_INET, SOCK_STREAM, 0);
	if (error(_socketServer))
	{
		printf("socket error!\n");
		_socketServer = 0;
		return false;
	}

	do 
	{
		struct sockaddr_in sockAddr;
		memset(&sockAddr, 0, sizeof(sockAddr));


		sockAddr.sin_family = AF_INET;
		sockAddr.sin_port = htons(port);
		sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		int ret = 0;
		ret = bind(_socketServer, (const sockaddr*)&sockAddr, sizeof(sockAddr));
		if (ret < 0)
		{
			printf("bind error!");
			break;
		}

		ret = listen(_socketServer, 5);
		if (ret < 0)
		{
			printf("listen error!");
			break;
		}

        /*
		char hostName[256];
        gethostname(hostName, sizeof(hostName));
		struct hostent* hostInfo = gethostbyname(hostName);
		char* ip = inet_ntoa(*(struct in_addr *)*hostInfo->h_addr_list);
		*/
        
		return true;

	} while (0);

	this->closeConnect(_socketServer);
	_socketServer = 0;

	return true;
}

void SocketServer::startServer()
{
	if(!initServer())
	{
		printf("server init error");
		return;
	}

	ConnectionManager* cm = ConnectionManager::getInstance();
	cm ->init();

	_messageHandler = new MessageHandler();
	_messageHandler->start();

	std::thread th(& SocketServer::_acceptClient, this);
	printf("start server port %u ...\n",port);
	th.join();

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
#endif

}


void SocketServer::_acceptClient()
{

	socklen_t len = sizeof(sockaddr);
	struct sockaddr_in sockAddr;
	while (true)
	{	
		HSocket clientSock = accept(_socketServer, (sockaddr*)&sockAddr, &len);
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
#ifdef WIN32
	WSACleanup();
#endif
}



bool SocketServer::error( HSocket socket )
{

#ifdef WIN32
	return socket == SOCKET_ERROR;
#elif __APPLE__
	return socket < 0;
#endif

}



void SocketServer::_onNewClientConnected(HSocket socket)
{
	printf("%d connect!\n",socket);

	ConnectionManager::getInstance()->addClient(socket);

	std::thread th(&SocketServer::_handleClientConnection, this, socket);
	th.detach();

}

void SocketServer::_handleClientConnection( HSocket socket )
{
	char buff[message_max_length];
	ssize_t ret = 0;

	while (true)
	{
		ret = recv(socket, buff, sizeof(buff), 0);
		if (ret < 0)
		{
			//printf("recv(%d) error!", socket);
			break;
		}
		else
		{
			if (ret > 0)
			{
				lock_guard<mutex> lk(_messageQueueMutex);
				int len = 0;
				memcpy(&len, buff, 4);
				if(len>0)
				{
					Message msg;
					msg.ParseFromString(buff+4,len);
					_messageHandler->messageQueue().push(msg);
				}
			}
		}
	}

	_mutex.lock();
    ConnectionManager::getInstance()->removeClient(socket);
	closeConnect(socket);
	printf("%d disconnect! \n", socket);
	_mutex.unlock();
}



