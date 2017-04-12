#pragma once
class DataManager
{
public:

	static DataManager* getInstance();
	static void destroyInstance();

private:

	DataManager(void);
	~DataManager(void);

	static DataManager* _instance;

};

