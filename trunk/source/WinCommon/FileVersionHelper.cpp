#include "stdafx.h"

#include "FileVersionHelper.h"

#include <stdio.h>
#include <string>
#include <Windows.h>

#include "Common/strhelper.h"

using namespace std;
using namespace sunjwbase;
using namespace WindowsComm;

#define RSRC_NAME ".rsrc"
#define VS_VERSION_INFO_NAME "VS_VERSION_INFO"

#define PAD(x) (((x) + 3) & 0xFFFFFFFC)

FileVersionHelper::FileVersionHelper(OsFile& osFile)
    :m_osFile(osFile)
{
}

FileVersionHelper::~FileVersionHelper()
{
}

void FileVersionHelper::Seek(LPCBYTE pointer)
{
    uint64_t offset = pointer - (LPCBYTE)0;
    m_osFile.seek(offset, OsFile::OsFileSeekFrom::OF_SEEK_BEGIN);
}

BYTE FileVersionHelper::ReadBYTE(LPCBYTE pointer)
{
    Seek(pointer);
    BYTE byteOut = 0;
    m_osFile.read(&byteOut, sizeof(BYTE));
    return byteOut;
}

WORD FileVersionHelper::ReadWORD(LPCBYTE pointer)
{
    Seek(pointer);
    WORD wordOut = 0;
    m_osFile.read(&wordOut, sizeof(WORD));
    return wordOut;
}

DWORD FileVersionHelper::ReadDWORD(LPCBYTE pointer)
{
    Seek(pointer);
    DWORD dwordOut = 0;
    m_osFile.read(&dwordOut, sizeof(DWORD));
    return dwordOut;
}

void FileVersionHelper::ReadSize(LPCBYTE pointer, DWORD size, LPBYTE buf)
{
    Seek(pointer);
    m_osFile.read(buf, size);
}

LPCBYTE FileVersionHelper::FindVersion()
{
    LPCBYTE buf = 0;
    //buf is a IMAGE_DOS_HEADER
    if (ReadWORD(buf) != 0x5A4D) //MZ signature
        return NULL;
    //pe is a IMAGE_NT_HEADERS32
    LPCBYTE pe = buf + ReadDWORD(buf + 0x3C);
    if (ReadWORD(pe) != 0x4550) //PE signature
        return NULL;
    //coff is a IMAGE_FILE_HEADER
    LPCBYTE coff = pe + 4;

    WORD numSections = ReadWORD(coff + 2);
    WORD optHeaderSize = ReadWORD(coff + 16);
    if (numSections == 0 || optHeaderSize == 0)
        return NULL;
    //optHeader is a IMAGE_OPTIONAL_HEADER32
    LPCBYTE optHeader = coff + 20;
    WORD magic = ReadWORD(optHeader);
    if (magic != 0x10b && magic != 0x20b) //Optional header magic (32 bits)
        return NULL;

    //if (magic == 0x10b)
    //    printf("PE32 ");
    //else
    //    printf("PE32+ ");
    //dataDir is an array of IMAGE_DATA_DIRECTORY
    LPCBYTE dataDir = optHeader + (magic == 0x10b ? 96 : 112);
    DWORD vaRes = ReadDWORD(dataDir + 8 * 2);

    //secTable is an array of IMAGE_SECTION_HEADER
    LPCBYTE secTable = optHeader + optHeaderSize;
    int i;
    for (i = 0; i < numSections; ++i)
    {
        //sec is a IMAGE_SECTION_HEADER*
        LPCBYTE sec = secTable + 40 * i;
        char secName[9] = { 0 };
        ReadSize(sec, 8, (LPBYTE)secName); //memcpy(secName, sec, 8);

        secName[8] = 0;

        if (strcmp(secName, RSRC_NAME) != 0)
            continue;
        DWORD vaSec = ReadDWORD(sec + 12);
        LPCBYTE raw = buf + ReadDWORD(sec + 20);
        LPCBYTE resSec = raw + (vaRes - vaSec);
        WORD numNamed = ReadWORD(resSec + 12);
        WORD numId = ReadWORD(resSec + 14);

        int j;
        for (j = 0; j < numNamed + numId; ++j)
        {
            //resSec is a IMAGE_RESOURCE_DIRECTORY followed by an array
            // of IMAGE_RESOURCE_DIRECTORY_ENTRY
            LPCBYTE res = resSec + 16 + 8 * j;
            DWORD name = ReadDWORD(res);
            if (name != 16) //RT_VERSION
                continue;
            DWORD offs = ReadDWORD(res + 4);
            if ((offs & 0x80000000) == 0) //is a dir resource?
                return NULL;
            //verDir is another IMAGE_RESOURCE_DIRECTORY and 
            // IMAGE_RESOURCE_DIRECTORY_ENTRY array
            LPCBYTE verDir = resSec + (offs & 0x7FFFFFFF);
            numNamed = ReadWORD(verDir + 12);
            numId = ReadWORD(verDir + 14);
            if (numNamed == 0 && numId == 0)
                return NULL;
            res = verDir + 16;
            offs = ReadDWORD(res + 4);
            if ((offs & 0x80000000) == 0) //is a dir resource?
                return NULL;
            //and yet another IMAGE_RESOURCE_DIRECTORY, etc.
            verDir = resSec + (offs & 0x7FFFFFFF);
            numNamed = ReadWORD(verDir + 12);
            numId = ReadWORD(verDir + 14);
            if (numNamed == 0 && numId == 0)
                return NULL;
            res = verDir + 16;
            offs = ReadDWORD(res + 4);
            if ((offs & 0x80000000) != 0) //is a dir resource?
                return NULL;
            verDir = resSec + offs;


            DWORD verVa = ReadDWORD(verDir);
            LPCBYTE verPtr = raw + (verVa - vaSec);
            return verPtr;
        }
        return NULL;
    }
    return NULL;
}

int FileVersionHelper::FindVsFixedFileInfo(LPCBYTE version, int offs, LPVOID fixedFileInfo)
{
    offs = PAD(offs);
    WORD len = ReadWORD(version + offs);
    offs += 2;
    WORD valLen = ReadWORD(version + offs);
    offs += 2;
    WORD type = ReadWORD(version + offs);
    offs += 2;
    CHAR info[1024];
    int i;
    for (i = 0; i < 200; ++i)
    {
        WORD c = ReadWORD(version + offs);
        offs += 2;

        info[i] = c;
        if (!c)
            break;
    }
    offs = PAD(offs);

    if (type == 0)
    {
        if (strcmp((LPCSTR)info, VS_VERSION_INFO_NAME) == 0)
        {
            //fixed is a VS_FIXEDFILEINFO
            LPCBYTE fixed = version + offs;
            *((LPCBYTE *)fixedFileInfo) = fixed;
        }
        offs += valLen;
    }

    while (offs < len)
        offs = FindVsFixedFileInfo(version, offs, fixedFileInfo);

    return PAD(offs);
}

tstring FileVersionHelper::Find()
{
    LPCBYTE version = FindVersion();
    if (!version)
    {
        // wprintf(L"No version\n");
    }
    else
    {
        LPVOID fixed = NULL;
        FindVsFixedFileInfo(version, 0, (LPVOID)&fixed);
        if (fixed != NULL)
        {
            // wprintf(L"fixed: %p\n", fixed);
            VS_FIXEDFILEINFO pvsf;
            ReadSize((LPCBYTE)fixed, sizeof(VS_FIXEDFILEINFO), (LPBYTE)&pvsf); //memcpy(&pvsf, pbuf, sizeof(VS_FIXEDFILEINFO));

            // 将版本号转换为数字 //
            unsigned int MVer, SVer, LVer, BVer;
            MVer = pvsf.dwFileVersionMS / 65536;
            SVer = pvsf.dwFileVersionMS - 65536 * MVer;
            LVer = pvsf.dwFileVersionLS / 65536;
            BVer = pvsf.dwFileVersionLS - 65536 * LVer;
            string strVer("");
            strVer = strappendformat(strVer, ("%d.%d.%d.%d"), MVer, SVer, LVer, BVer);
            return strtotstr(strVer);
        }
    }

    return tstring();
}
