#ifndef _HASH_ENGINE_H_
#define _HASH_ENGINE_H_

#include <WinDef.h>

#include "Global.h"

class DataBuffer
{
public:
	DataBuffer();
	~DataBuffer();

	static unsigned int preflen;

	unsigned int datalen;
	unsigned char *data;
};

DWORD WINAPI md5_file(LPVOID pParam);

#endif