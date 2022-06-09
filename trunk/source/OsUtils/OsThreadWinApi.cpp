/*
 * OsThread Windows API implementation file
 * Author: Sun Junwen
 * Version: 0.5
 * Provider Mutex.
 */
#include "stdafx.h"

#include "OsThread.h"

#include <stdlib.h>
#include <stdint.h>

#include "Windows.h"

using namespace sunjwbase;

#define GET_CRITSEC_FROM_POINTER(pointer) ((CRITICAL_SECTION *)(pointer))

OsMutex::OsMutex()
{
	CRITICAL_SECTION *critSec = new CRITICAL_SECTION();
	InitializeCriticalSection(critSec);

	_osMtxData = (void *)critSec;
}

OsMutex::~OsMutex()
{
	CRITICAL_SECTION *critSec = GET_CRITSEC_FROM_POINTER(_osMtxData);
	DeleteCriticalSection(critSec);

	delete critSec;
}

void OsMutex::lock()
{
	CRITICAL_SECTION *critSec = GET_CRITSEC_FROM_POINTER(_osMtxData);
	EnterCriticalSection(critSec);
}

void OsMutex::unlock()
{
	CRITICAL_SECTION *critSec = GET_CRITSEC_FROM_POINTER(_osMtxData);
	LeaveCriticalSection(critSec);
}

