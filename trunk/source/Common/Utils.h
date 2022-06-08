#ifndef _UTILS_H_
#define _UTILS_H_

#include <string>
#include <stdlib.h>
#include <stdint.h>

namespace Utils
{
    uint64_t GetCurrentMilliSec();
    
    std::string ConvertSizeToShortSizeStr(uint64_t size, bool conv1KSmaller = false);
    
}

#endif
