#pragma once
#include <string>
#include <stdlib.h>

enum MessageType
{
	MT_Broadcast = 0,
};


class Message
{
public:
	Message(void);
	~Message(void);

	int serializeToString(char* data);
	void ParseFromString(const char* data, int length);


	int getType();
	std::string getContent();

	void setType(int tp);
	void setContent(std::string content);

private:

	int _length;
	int _type;
	std::string _content;

};

