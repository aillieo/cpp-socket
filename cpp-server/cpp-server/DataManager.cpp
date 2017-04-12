#include "DataManager.h"


DataManager* DataManager::_instance = nullptr;

DataManager::DataManager(void)
{
}


DataManager::~DataManager(void)
{
}

DataManager* DataManager::getInstance()
{
	if(nullptr == _instance)
	{
		_instance = new DataManager();
	}
	return _instance;
}

void DataManager::destroyInstance()
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
