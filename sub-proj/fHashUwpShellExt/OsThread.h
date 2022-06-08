/*
 * OsThread header file
 * Author: Sun Junwen
 * Version: 0.5
 * Provider Mutex.
 */
#ifndef _OS_THREAD_H_
#define _OS_THREAD_H_

#include <stdlib.h>
#include <stdint.h>

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
