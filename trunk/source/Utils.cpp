#include "stdafx.h"

#include "Utils.h"

#include <stdio.h>
#include <string>

#if defined (__APPLE__) || defined (__unix)
#include <sys/time.h>
#endif

using namespace std;

namespace Utils
{
    uint64_t GetCurrentMilliSec()
    {
#if defined (__APPLE__) || defined (__unix)
        struct timeval tv;
        gettimeofday(&tv, NULL);
        return (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000;
#endif
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

