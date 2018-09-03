#ifndef _HASH_ENGINE_H_
#define _HASH_ENGINE_H_

#include "Common/Global.h"

#if defined (WIN32)
#include <WinDef.h>
#endif

int WINAPI HashThreadFunc(void *param);

#endif
