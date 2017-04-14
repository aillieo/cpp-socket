// main.cpp : Defines the entry point for the console application.
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

	CommunicationManager* cm = CommunicationManager::getInstance();
	bool connect = cm->init();
	if(!connect)
	{
		printf("cannot connect to server!\n");
#ifdef WIN32
        system("pause");
#endif
		return 0;
	}

	UIDisplayer* ui = UIDisplayer::getInstance();
	ui->init();

#ifdef __APPLE__

    initscr();
#endif
    
	while(true)
	{
		ui->handleInput(getch());
	}

#ifdef __APPLE__
    
    //endwin();
#endif

	return 0;
}

