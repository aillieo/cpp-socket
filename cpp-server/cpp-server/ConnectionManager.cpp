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

const size_t ConnectionManager::addClient(HSocket client)
{
    _clients.push_back(client);
    printf("current users: %ld\n", _clients.size());
    return _clients.size();
}


const size_t ConnectionManager::removeClient(HSocket client)
{
    auto disconnectClient = find(_clients.begin(),_clients.end(),client);
    _clients.erase(disconnectClient);
    printf("current users: %ld\n", _clients.size());
    return _clients.size();
}


