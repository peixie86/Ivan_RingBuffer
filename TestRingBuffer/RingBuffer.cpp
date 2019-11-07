#include "RingBuffer.h"
#include <memory.h>


RingBuffer::RingBuffer()
	: m_nSize(0)
	, m_pData(nullptr)
	, m_nFront(0)
	, m_nRear(0)
{ 
}

RingBuffer::RingBuffer(unsigned __int64 nSize)
	: m_nSize(0)
	, m_pData(nullptr)
	, m_nFront(0)
	, m_nRear(0)
{
	m_pData = new char[nSize + 1];
	if (m_pData != nullptr)
	{
		m_nSize = nSize;
	}
}

RingBuffer::~RingBuffer()
{
	m_nSize = 0;
	if (nullptr != m_pData)
	{
		delete[] m_pData;
		m_pData = nullptr;
	}
	m_nFront = 0;
	m_nRear = 0;
}

bool RingBuffer::setSize(unsigned __int64 nSize)
{
	if (nullptr != m_pData)
	{
		delete[] m_pData;
		m_pData = new char[nSize + 1];
		if (m_pData != nullptr)
		{
			m_nSize = nSize;
			return true;
		}
	}
	return false;
}

unsigned __int64 RingBuffer::size()
{
	return m_nSize;
}

unsigned __int64 RingBuffer::freeSize()
{
	if (nullptr == m_pData)
	{
		return 0;
	}
	return m_nSize - (m_nRear >= m_nFront ? m_nRear - m_nFront : m_nRear + m_nSize - m_nFront + 1);
}

unsigned __int64 RingBuffer::dataSize()
{
	if (nullptr == m_pData)
	{
		return 0;
	}
	return m_nRear >= m_nFront ? m_nRear - m_nFront : m_nRear + m_nSize - m_nFront + 1;
}

void RingBuffer::clear()
{
	m_nRear = m_nFront = 0;
}

unsigned __int64 RingBuffer::read(void* pData, unsigned __int64 iSize)
{
	unsigned __int64 iReadSize = iSize < dataSize() ? iSize : dataSize();
	if (iReadSize != 0)
	{
		if (m_nRear > m_nFront)
		{
			memcpy(pData, m_pData + m_nFront, iReadSize);
			m_nFront += iReadSize;
		}
		else
		{
			if (m_nSize - m_nFront >= iReadSize)
			{
				memcpy(pData, m_pData + m_nFront, iReadSize);
				m_nFront += iReadSize;
			}
			else
			{
				memcpy(pData, m_pData + m_nFront, m_nSize - m_nFront + 1);
				__int64 nResidue = (iReadSize + m_nFront) % (m_nSize + 1);
				memcpy((char*)pData + (m_nSize - m_nFront + 1), m_pData, nResidue);
				m_nFront = nResidue;
			}
		}
	}
	return iReadSize;
}

unsigned __int64 RingBuffer::write(const void* pData, unsigned __int64 iSize)
{
	unsigned __int64 iWriteSize = iSize < freeSize() ? iSize : freeSize();
	if (iWriteSize != 0)
	{
		if (m_nSize - m_nRear >= iWriteSize)
		{
			memcpy(m_pData + m_nRear, pData, iWriteSize);
			m_nRear += iWriteSize;
		}
		else
		{
			memcpy(m_pData + m_nRear, pData, m_nSize - m_nRear + 1);
			__int64 nResidue = (iWriteSize + m_nRear) % (m_nSize + 1);
			memcpy(m_pData, (char*)pData + (m_nSize - m_nRear + 1), nResidue);
			m_nRear = nResidue;
		}
	}
	return iWriteSize;
}