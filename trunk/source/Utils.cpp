#include "stdafx.h"

#include "Utils.h"

#include <stdio.h>
#include <string>

#if defined (WIN32)
#include <Windows.h>
#include <Winsock2.h>
#endif

#if defined (__APPLE__) || defined (__unix)
#include <sys/time.h>
#endif

using namespace std;

namespace Utils
{
#if defined (WIN32)
    // http://stackoverflow.com/questions/10905892/equivalent-of-gettimeday-for-windows
	int gettimeofday(struct timeval *tp, struct timezone *tzp)
	{
		// Note: some broken versions only have 8 trailing zero's, the correct epoch has 9 trailing zero's
        static const uint64_t EPOCH = ((uint64_t) 116444736000000000ULL);

        SYSTEMTIME  system_time;
        FILETIME    file_time;
        uint64_t    time;

        GetSystemTime( &system_time );
        SystemTimeToFileTime( &system_time, &file_time );
        time =  ((uint64_t)file_time.dwLowDateTime )      ;
        time += ((uint64_t)file_time.dwHighDateTime) << 32;

        tp->tv_sec  = (long) ((time - EPOCH) / 10000000L);
        tp->tv_usec = (long) (system_time.wMilliseconds * 1000);
        return 0;
    }
#endif

    uint64_t GetCurrentMilliSec()
    {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        return (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000;
    }
    
    string ConvertSizeToShortSizeStr(uint64_t size, bool conv1KSmaller)
    {
        string strSize("");

        char buff[1024] = {0};

        if(size > 1024)
        {
            double k_size = size / 1024.0f;
            if(k_size > 1024)
            {
                double m_size = k_size / 1024;
                if(m_size > 1024)
                {
                    double g_size = m_size / 1024;
#if defined (WIN32)
                    sprintf_s(buff, 1024, "%.2f GB", g_size);
#else
                    sprintf(buff, "%.2f GB", g_size);
#endif
                }
                else
                {
#if defined (WIN32)
                    sprintf_s(buff, 1024, "%.2f MB", m_size);
#else
                    sprintf(buff, "%.2f MB", m_size);
#endif
                }
            }
            else
            {
#if defined (WIN32)
                sprintf_s(buff, 1024, "%.2f KB", k_size);
#else
                sprintf(buff, "%.2f KB", k_size);
#endif
            }
        }
        else if(conv1KSmaller)
        {
#if defined (WIN32)
            sprintf_s(buff, 1024, "%.2f B", (double)size);
#else
            sprintf(buff, "%.2f B", (double)size);
#endif
        }

        strSize = buff;

        return strSize;
    }
    
    

}

