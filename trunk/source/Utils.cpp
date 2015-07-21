#include "stdafx.h"

#include "Utils.h"

#include <stdio.h>
#include <string>

using namespace std;

namespace Utils
{
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

