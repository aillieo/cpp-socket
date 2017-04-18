#include "Buffer.h"
#include "Config.h"


Buffer::Buffer(void)
{
	_bufferSize = message_max_length;
	_writtenSize = 0;
	_buffer = new char[_bufferSize];
}


Buffer::~Buffer(void)
{
	if(_buffer)
	{
		delete[] _buffer;
	}
}

size_t Buffer::getSize()
{
	return _writtenSize;
}

void* Buffer::getBuffer()
{
	return &_buffer[_writtenSize];
}

size_t Buffer::getSpace()
{
	return _bufferSize - _writtenSize;
}

void Buffer::addWrittenSize( int len )
{
	_writtenSize += len;
}


