/*
 * OsThread posix (darwin) implementation file
 * Author: Sun Junwen
 * Version: 0.5
 * Provider Mutex.
 */
#include "stdafx.h"

#include "OsThread.h"

#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>

using namespace sunjwbase;

#define GET_PTMUTEX_FROM_POINTER(pointer) ((pthread_mutex_t *)(pointer))

OsMutex::OsMutex()
{
    pthread_mutex_t *ptMutex = new pthread_mutex_t();
    pthread_mutex_init(ptMutex, NULL);

    _osMtxData = (void *)ptMutex;
}

OsMutex::~OsMutex()
{
    pthread_mutex_t *ptMutex = GET_PTMUTEX_FROM_POINTER(_osMtxData);
    pthread_mutex_destroy(ptMutex);

    delete ptMutex;
}

void OsMutex::lock()
{
    pthread_mutex_t *ptMutex = GET_PTMUTEX_FROM_POINTER(_osMtxData);
    pthread_mutex_lock(ptMutex);
}

void OsMutex::unlock()
{
    pthread_mutex_t *ptMutex = GET_PTMUTEX_FROM_POINTER(_osMtxData);
    pthread_mutex_unlock(ptMutex);
}

