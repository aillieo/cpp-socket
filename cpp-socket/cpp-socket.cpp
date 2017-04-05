// cpp-socket.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include "UIDisplayer.h"
#include <conio.h>
#include "CommunicationManager.h"


int main()
{

	UIDisplayer* ui = UIDisplayer::getInstance();
	ui->init();
	CommunicationManager* cm = CommunicationManager::getInstance();
	cm->init();

	while(true)
	{
		ui->handleInput(getch());
	}



	return 0;
}

