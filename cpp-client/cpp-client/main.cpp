// cpp-socket.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include "UIDisplayer.h"
#include "CommunicationManager.h"

#ifdef WIN32
#include <conio.h>
#elif __APPLE__
#include <curses.h>
#endif



int main()
{

	UIDisplayer* ui = UIDisplayer::getInstance();
	ui->init();
	CommunicationManager* cm = CommunicationManager::getInstance();
	cm->init();

#ifdef __APPLE__

    initscr();
#endif
    
	while(true)
	{
		ui->handleInput(getch());
	}

#ifdef __APPLE__
    
    endwin();
#endif

	return 0;
}

