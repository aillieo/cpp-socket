#pragma once
#include "Config.h"
#include <queue>
#include <deque>
#include <string>
#include <vector>
#include "Message.h"


class UIDisplayer
{
public:

	static UIDisplayer* getInstance();
	static void destroyInstance();

	std::queue<Message>& getMessageQueue();
	void handleInput(int input);
	bool init();
	
	void appendMessage(std::string str);

private:


	UIDisplayer(void);
	~UIDisplayer(void);

	static UIDisplayer* _instance;

	void _display();

	std::deque<std::string> _messagesWithFormat;
	std::queue<Message> _messageQueue;
	std::vector<int> _input;

	void _clearScreen();
	void _seperate();

	void _displayMessage();
	void _displayInput();


	void hideCursor();

};

