#pragma once
#include <queue>
#include "Message.h"
#include <mutex>

class MessageHandler
{
public:
	MessageHandler(void);
	~MessageHandler(void);


	void start();

	std::queue<Message>& messageQueue();

private:

	void _startHandleMessage();

	void _handleMessage(Message& msg);

	std::queue<Message> _messageQueue;
	std::mutex _messageQueueMutex;





	void _broadcastMessage(Message& msg);

};

