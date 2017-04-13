#include "ConnectionManager.h"


ConnectionManager* ConnectionManager::_instance = nullptr;

ConnectionManager::ConnectionManager(void)
{
}


ConnectionManager::~ConnectionManager(void)
{
}

ConnectionManager* ConnectionManager::getInstance()
{
	if(nullptr == _instance)
	{
		_instance = new ConnectionManager();
	}
	return _instance;
}

void ConnectionManager::destroyInstance()
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

std::vector<HSocket>& ConnectionManager::clients()
{
	return _clients;
}

bool ConnectionManager::init()
{



	return true;
}


