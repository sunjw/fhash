#ifndef _HASH_ENGINE_H_
#define _HASH_ENGINE_H_

#include "Global.h"

#if defined (WIN32)

#include <WinDef.h>

#endif


class DataBuffer
{
public:
	DataBuffer();
	~DataBuffer();

	static unsigned int preflen;

	unsigned int datalen;
	unsigned char *data;
};

int WINAPI HashThreadFunc(void *param);

#endif