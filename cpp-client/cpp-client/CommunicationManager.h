#pragma once
#include <string>
#include "Message.h"
#include "SocketClient.h"
#include <mutex>

class CommunicationManager
{
public:

	static CommunicationManager* getInstance();
	static void destroyInstance();


	bool init();

	void sendMessage(int type, std::string str);
	void receive(const char* data, int length);
	

private:

	CommunicationManager(void);
	~CommunicationManager(void);

	static CommunicationManager* _instance;



	SocketClient* _socket;


	void _handleMessage(Message msg);


	std::mutex _mutex;

};

