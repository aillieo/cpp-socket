#pragma once
#include <vector>

#ifdef WIN32
#include <WinSock2.h>
#define HSocket SOCKET
#elif __APPLE__
#include <sys/socket.h>
#define HSocket int
#endif

class ConnectionManager
{
public:
	static ConnectionManager* getInstance();
	static void destroyInstance();

	bool init();

	std::vector<HSocket>& clients();

    const size_t addClient(HSocket client);
    const size_t removeClient(HSocket client);
    
private:

	ConnectionManager(void);
	~ConnectionManager(void);

	static ConnectionManager* _instance;


	std::vector<HSocket> _clients;


};

