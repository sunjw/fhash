/*
 * OsThread header file
 * Author: Sun Junwen
 * Version: 0.5
 * Provider Mutex.
 */
#ifndef _OS_THREAD_H_
#define _OS_THREAD_H_

#include <stdlib.h>
#if defined (WIN32)
#include "WinMFC/stdint.h"
#else
#include <stdint.h>
#endif

namespace sunjwbase
{
	class OsMutex
	{
	public:
		explicit OsMutex();
		~OsMutex();

		void lock();
		void unlock();

	private:
		void	*_osMtxData;

	};
}


#endif
