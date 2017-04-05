#include "SocketClient.h"
#include "Message.h"
#include "CommunicationManager.h"
#ifdef WIN32
#pragma comment(lib, "wsock32")
#endif

SocketClient::SocketClient(void):
_socekt(0),
_inited(false)
{
}


SocketClient::~SocketClient(void)
{
	cleanup();
}

void SocketClient::sendMsg( const char* data, int length )
{

	if (_socekt != 0)
	{
		int ret = send(_socekt, data, length, 0);
		if (ret < 0)
		{
			//printf("send error!");
		}
	}

}

void SocketClient::receiveMsg()
{

	char recvBuf[message_max_length];
	int ret = 0;
	while (true)
	{
		ret = recv(_socekt, recvBuf, sizeof(recvBuf), 0);
		if (ret < 0)
		{
			//printf("recv error!");
			break;
		}
		if (ret > 0)
		{
			CommunicationManager::getInstance()->receive(recvBuf, sizeof(recvBuf));
		}
	}
	_mutex.lock();
	this->closeConnect(_socekt);

	_socekt = 0;
	_mutex.unlock();

	//CommunicationManager::getInstance()->receive(data, length);
}

void SocketClient::closeConnect( HSocket socket )
{
#ifdef WIN32
	closesocket(socket);
#endif
}

bool SocketClient::error( HSocket socket )
{
#ifdef WIN32
	return socket == SOCKET_ERROR;
#endif
}

bool SocketClient::init()
{
	


	cleanup();
#ifdef WIN32
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2,1), &wsadata);
#endif 
	_socekt = socket(AF_INET, SOCK_STREAM, 0);
	if (error(_socekt))
	{
		//printf("init client error!");
		_socekt = 0;
		_inited = false;
	}
	else
	{
		_inited = true;
	}

	return _inited;

}




bool SocketClient::connectServer(const char* serverIP, unsigned short port)
{

	if (!this->init())
	{
		return false;
	}

	struct sockaddr_in serverAddr;
	memset(&serverAddr, 0, sizeof(struct sockaddr_in));

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = inet_addr(serverIP);

	int ret = 0;
	ret = connect(_socekt, (struct sockaddr*)&serverAddr, sizeof(struct sockaddr));
	if (ret < 0)
	{
		_socekt = 0;
		return false;
	}

	std::thread tRecv(&SocketClient::receiveMsg, this);
	tRecv.detach();

	return true;

}

void SocketClient::cleanup()
{

	if (_socekt != 0)
	{
		_mutex.lock();
		this->closeConnect(_socekt);
		_mutex.unlock();
	}

	
}
