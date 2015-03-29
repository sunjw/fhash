/*
 * OsFile header file
 * Author: Sun Junwen
 * Version: 0.5
 * Provider basic open/close, read, write and 
 * attributes functions of file.
 */
#ifndef _OS_FILE_H_
#define _OS_FILE_H_

#include <stdlib.h>
#include <stdint.h>

#include "strhelper.h"

namespace sunjwbase
{
	class OsFile
	{
	public:
		explicit OsFile(sunjwbase::tstring filePath);
		~OsFile();

		// Open
		bool openRead();

		// Attributes
		uint64_t getLength();
		bool getModifiedTime(void *modifiedTime);

		// Operation
		uint32_t read(void *readBuffer, uint32_t bytes);

		// Close
		void close();

	private:
		sunjwbase::tstring	_filePath;
		void				*_osfileData;
	};

}

#endif // _OS_FILE_H_

