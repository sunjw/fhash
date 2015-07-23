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
        struct timeval tv;
        gettimeofday(&tv, NULL);
        return (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000;
    }
    
    string ConvertSizeToShortSizeStr(uint64_t size)
    {
        string strSize("");

        char buff[1024] = {0};

        if(size > 1024)
        {
            float k_fsize = size / 1024.0f;
            if(k_fsize > 1024)
            {
                float M_fsize = k_fsize / 1024;
                if(M_fsize > 1024)
                {
                    float g_fsize = M_fsize / 1024;
#if defined (WIN32)
                    sprintf_s(buff, 1024, "%.2f GB", g_fsize);
#else
                    sprintf(buff, "%.2f GB", g_fsize);
#endif
                }
                else
                {
#if defined (WIN32)
                    sprintf_s(buff, 1024, "%.2f MB", M_fsize);
#else
                    sprintf(buff, "%.2f MB", M_fsize);
#endif
                }
            }
            else
            {
#if defined (WIN32)
                sprintf_s(buff, 1024, "%.2f KB", k_fsize);
#else
                sprintf(buff, "%.2f KB", k_fsize);
#endif
            }
        }

        strSize = buff;

        return strSize;
    }
    
    

}

