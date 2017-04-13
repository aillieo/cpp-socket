#include "UIDisplayer.h"
#include <stdlib.h>
#include "Config.h"
#include <thread>
#include "CommunicationManager.h"

#ifdef WIN32
#include <windows.h> 
#define CHAR_ENTER '\r'
#define NEW_LINE "\r\n"
#elif __APPLE__
#define CHAR_ENTER '\r'
#define NEW_LINE "\r"
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
	printf(NEW_LINE);
	
}

queue<Message>& UIDisplayer::getMessageQueue()
{
	return _messageQueue;
}

void UIDisplayer::_display()
{
	while(true)
	{
		_multex.lock();
		if(_needRefresh)
		{
			_clearScreen();
			_displayMessage();
			_seperate();
			_displayInput();
		}
		_needRefresh = false;
		_multex.unlock();
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
	size_t messageCount = _messagesWithFormat.size();
	for(int i = 0 ; i < messageCount ; i ++)
	{
		printf("%s",_messagesWithFormat.at(i).c_str());
        printf(NEW_LINE);
	}
}

void UIDisplayer::_displayInput()
{
	size_t length = _input.size();
	for(int i = 0 ; i <length ; i ++)
	{
		printf("%c",_input[i]);
	}
	printf(NEW_LINE);
}


void UIDisplayer::handleInput( int input )
{

	if(input == 8 && _input.size() != 0) {
		_input.pop_back();
	} 
	else if(input == CHAR_ENTER && _input.size() != 0)
	{
		std::string str = "";
		size_t length = _input.size();
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

	_needRefresh = true;
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

	_needRefresh = true;

}


