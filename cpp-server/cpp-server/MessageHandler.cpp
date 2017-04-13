#include "MessageHandler.h"
#include <thread>
#include "ConnectionManager.h"
#include "Config.h"

using std::lock_guard;
using std::mutex;

MessageHandler::MessageHandler(void)
{
}


MessageHandler::~MessageHandler(void)
{
}

void MessageHandler::start()
{
	std::thread th(& MessageHandler::_startHandleMessage, this);
	th.detach();
}

void MessageHandler::_startHandleMessage()
{
	while (true)
	{
		if(_messageQueue.size()>0)
		{		
			lock_guard<mutex> lk(_messageQueueMutex);
			Message msg = _messageQueue.front();
			_handleMessage(msg);
			_messageQueue.pop();
		}
	}
}

std::queue<Message>& MessageHandler::messageQueue()
{
	return _messageQueue;
}

void MessageHandler::_handleMessage(Message& msg )
{
	
	printf("type=%d, content=%s\n",msg.getType(),msg.getContent().c_str());
	switch (msg.getType())
	{
	case MT_Broadcast:

		_broadcastMessage(msg);
		break;

	default:
		break;
	}
}

void MessageHandler::_broadcastMessage( Message& msg )
{

	char* data = (char*)malloc(4 + sizeof(char)*message_max_length);
	for (HSocket skt : ConnectionManager::getInstance()->clients())
	{
		Message _msg;
		_msg.setType(0);
		_msg.setContent(msg.getContent());
		int len = _msg.serializeToString(data+4);
		memcpy(data,&len,4);
		long ret = send(skt,data,len+4,0);

		if(ret < 0)
		{
			printf("broadcast error to client %d", skt);
		}
	}
	free(data);
}
