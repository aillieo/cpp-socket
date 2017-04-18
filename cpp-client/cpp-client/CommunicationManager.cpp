#include "CommunicationManager.h"
#include "UIDisplayer.h"
using std::mutex;

CommunicationManager* CommunicationManager::_instance = nullptr;

CommunicationManager::CommunicationManager(void)
{
}


CommunicationManager::~CommunicationManager(void)
{
}


CommunicationManager* CommunicationManager::getInstance()
{

	if(nullptr == _instance)
	{
		_instance = new CommunicationManager();
	}
	return _instance;

}

void CommunicationManager::destroyInstance()
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

bool CommunicationManager::init()
{
	_socket = new SocketClient();
	bool ret = _socket->connectServer(server, port);
	return ret;
}

void CommunicationManager::sendMessage( int type, std::string str )
{
	Message msg;
	msg.setType(0);
	msg.setContent(str);
	char* data = (char*)malloc(4 + sizeof(char)*message_max_length);
	int len = msg.serializeToString(data+4);
	memcpy(data,&len,4);
	_socket->sendMsg(data,len+4);
	free(data);

}

void CommunicationManager::receive( const char* data, int length )
{
	Message msg;
	msg.ParseFromString(data,length);
	_handleMessage(msg );
}


void CommunicationManager::_handleMessage( Message msg )
{
	
	if(msg.getType() == 0)
	{
		_mutex.lock();

		UIDisplayer::getInstance()->appendMessage(msg.getContent());

		_mutex.unlock();

	}


}


