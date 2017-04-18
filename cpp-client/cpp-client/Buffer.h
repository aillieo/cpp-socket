#pragma once
class Buffer
{
public:
	Buffer(void);
	~Buffer(void);

	size_t getSize();

	void* getBuffer();
	size_t getSpace();

	void addWrittenSize(int len);

private:

	char* _buffer;
	int _bufferSize;
	int _writtenSize;
};

