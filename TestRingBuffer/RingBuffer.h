#pragma once

class RingBuffer
{
public:
	RingBuffer();
	RingBuffer(unsigned __int64 nSize);
	~RingBuffer();

	bool setSize(unsigned __int64);
	unsigned __int64 size();
	unsigned __int64 freeSize();
	unsigned __int64 dataSize();
	void clear();
	unsigned __int64 read(void* pData, unsigned __int64 iSize);
	unsigned __int64 write(const void* pData, unsigned __int64 iSize);

private:
	unsigned __int64 m_nSize;
	char* m_pData;
	unsigned __int64 m_nFront;
	unsigned __int64 m_nRear;
};

