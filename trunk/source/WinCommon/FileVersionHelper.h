#pragma once

#include <Windows.h>

#include "OsUtils/OsFile.h"
#include "Common/strhelper.h"

typedef const BYTE* LPCBYTE;

namespace WindowsComm
{
    class FileVersionHelper
    {
    public:
        FileVersionHelper(sunjwbase::OsFile& osFile);
        virtual ~FileVersionHelper();

        sunjwbase::tstring Find();

    private:
        void Seek(LPCBYTE pointer);

        BYTE ReadBYTE(LPCBYTE pointer);
        WORD ReadWORD(LPCBYTE pointer);
        DWORD ReadDWORD(LPCBYTE pointer);
        void ReadSize(LPCBYTE pointer, DWORD size, LPBYTE buf);

        LPCBYTE FindVersion();
        int FindVsFixedFileInfo(LPCBYTE version, int offs, LPVOID fixedFileInfo);

        sunjwbase::OsFile& m_osFile;
    };
}
