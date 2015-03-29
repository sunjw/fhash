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
		sunjwbase::tstring	m_filePath;
		void				*m_fileData;
	};

}

#endif // _OS_FILE_H_

