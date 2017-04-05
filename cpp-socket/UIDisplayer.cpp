#include "UIDisplayer.h"
#include <stdlib.h>
#include "Config.h"
#include <thread>
#include "CommunicationManager.h"

#ifdef WIN32
#include <windows.h> 
#endif

using std::string;
using std::queue;
using std::deque;
using std::thread;
using std::vector;

UIDisplayer* UIDisplayer::_instance = nullptr;

UIDisplayer::UIDisplayer(void)
{
	_messageQueue = queue<Message>();
	_messagesWithFormat = deque<string>();
	_input = vector<int>();
}


UIDisplayer::~UIDisplayer(void)
{
	while(!_messageQueue.empty())
	{
		_messageQueue.pop();
	}
	_messagesWithFormat.clear();
	_input.clear();
	
}


UIDisplayer* UIDisplayer::getInstance()
{

	if(nullptr == _instance)
	{
		_instance = new UIDisplayer();
	}
	return _instance;

}

void UIDisplayer::destroyInstance()
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

void UIDisplayer::_clearScreen()
{
#ifdef WIN32
	system("cls");

#else
	system("clear");

#endif
}


void UIDisplayer::_seperate()
{
	int width = 30;
	while(width>0)
	{
		printf("-");
		width--;
	}
	printf("\n");
	
}

queue<Message>& UIDisplayer::getMessageQueue()
{
	return _messageQueue;
}

void UIDisplayer::_display()
{
	while(true)
	{
		_clearScreen();
		_displayMessage();
		_seperate();
		_displayInput();
	}
}



bool UIDisplayer::init()
{
	hideCursor();
	thread tInput(&UIDisplayer::_display,this);
	tInput.detach();

	return true;
}

void UIDisplayer::_displayMessage()
{
	int messageCount = _messagesWithFormat.size();
	for(int i = 0 ; i < messageCount ; i ++)
	{
		printf("%s\n",_messagesWithFormat.at(i).c_str());
	}
}

void UIDisplayer::_displayInput()
{
	int length = _input.size();
	for(int i = 0 ; i <length ; i ++)
	{
		printf("%c",_input[i]);
	}
	printf("\n");
}


void UIDisplayer::handleInput( int input )
{

	if(input == 8 && _input.size() != 0) {
		_input.pop_back();
	} 
	else if(input == 13 && _input.size() != 0)
	{
		std::string str = "";
		int length = _input.size();
		for(int i = 0 ; i < length ; i++)
		{
			str.push_back((char)(_input[i]));
		}
		CommunicationManager::getInstance()->sendMessage(0, str );
		

		_input.clear();
	}
	else if(input != 8){
		_input.push_back(input);
	}

}

void UIDisplayer::hideCursor()
{

#ifdef WIN32
	HANDLE hout=GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursor_info={1,0};
	SetConsoleCursorInfo(hout, &cursor_info);
#endif


}

void UIDisplayer::appendMessage( std::string str )
{
	
	if(_messagesWithFormat.size() > max_message_amount_display)
	{
		_messagesWithFormat.pop_front();
	}
	_messagesWithFormat.push_back(str);

}


