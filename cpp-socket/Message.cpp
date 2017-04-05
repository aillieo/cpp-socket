#include "Message.h"
#include "Config.h"
using std::string;

Message::Message(void)
{
	_type = 0;
	_length = 0;
}


Message::~Message(void)
{
}

int Message::serializeToString(char* data )
{
	memset(data,0,sizeof(char)*message_max_length);
	_length = 4 + strlen(_content.c_str());
	memcpy(data,&_type,4);
	memcpy(data+4,_content.c_str(),_length - 4 + 1);
	return _length;
}

void Message::ParseFromString( const char* data , int length)
{
	if(!data){return;}
	if(length<4){return;}
	_length = length-4;

	memcpy(&_type,data,4);
	char* _contentData = (char*)malloc(sizeof(char)*message_max_length);
	memset(_contentData,0,sizeof(char)*message_max_length);
	memcpy(_contentData,data+4,_length);
	_content.assign(_contentData);

}

int Message::getType()
{
	return _type;
}

std::string  Message::getContent()
{
	return _content;
}

void Message::setType( int tp )
{
	_type = tp;
}

void Message::setContent( std::string content )
{
	_content = content;
}
