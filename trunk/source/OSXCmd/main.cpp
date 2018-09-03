//
//  main.cpp
//  fHashCmd
//
//  Created by Sun Junwen on 15/5/9.
//  Copyright (c) 2015 Sun Junwen. All rights reserved.
//

#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <sys/time.h>
#include <string>

#include "Common/Global.h"
#include "version.h"
#include "OsThread.h"
#include "Common/strhelper.h"
#include "Common/Utils.h"
#include "Common/HashEngine.h"
#include "UIBridgeMacCmd.h"

using namespace std;
using namespace sunjwbase;

OsMutex g_mainMtx;

static int64_t GetCurMillSecond()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    
    int64_t timeMillSec = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000;
    
    return timeMillSec;
}

int main(int argc, char *argv[])
{
    printf("fhash - A files hash calculator\n");
    printf("Version: " STR_VERSION "\n");
    printf("\n");
    
    UIBridgeMacCmd uiBridgeMacCmd;
    
    ThreadData thData;
    
    thData.uiBridge = &uiBridgeMacCmd;
    thData.threadWorking = false;
    thData.stop = false;
    thData.uppercase = false;
    thData.nFiles = 0;
    
    if (argc == 1)
    {
        printf("Usage: fhash [file1] [file2] [file3] ...\n");
        exit(0);
    }
    
    for (int i = 1; i < argc; ++i)
    {
        string strPath;
        tstring tstrPath;
        
        strPath = string(argv[i]);
        tstrPath = strtotstr(strPath);
        thData.fullPaths.push_back(tstrPath);
        thData.nFiles++;
    }
    
    uint64_t beginTime = GetCurMillSecond();
    
    pthread_t ptHash;
    pthread_create(&ptHash, NULL,
                   (void *(*)(void *))HashThreadFunc, &thData);
    
    pthread_join(ptHash, NULL);
    
    uint64_t endTime = GetCurMillSecond();
    uint64_t durTime = endTime - beginTime;
    
    double calcSpeed = thData.totalSize / (double)durTime * 1000;
    
    string strMeasure;
    if((calcSpeed / 1024) > 1)
    {
        calcSpeed /= 1024;
        strMeasure = "KB/s";
        if((calcSpeed / 1024) > 1)
        {
            calcSpeed /= 1024;
            strMeasure = "MB/s";
        }
    }
    
    double durTimeSec = durTime / 1000.0;
    
    printf("Finished in %4.2fs", durTimeSec);
    if (strMeasure != "")
    {
        printf(", %4.2f %s", calcSpeed, strMeasure.c_str());
    }
    printf("\n");
    
    /*
    ResultList::const_iterator resItr;
    for (resItr = thData.resultList.begin();
         resItr != thData.resultList.end();
         ++resItr)
    {
        const ResultData& result = *resItr;
        
        printf("File: %s\n", tstrtostr(result.tstrPath).c_str());
        
        if (result.bDone)
        {
            printf("Size: %llu Bytes%s\n",
                   result.ulSize, ConvertSizeToStr(result.ulSize).c_str());
            printf("Modified Date: %s\n", tstrtostr(result.tstrMDate).c_str());
            
            if (thData.uppercase)
            {
                printf("MD5: %s\n", tstrtostr(result.tstrMD5).c_str());
                printf("SHA1: %s\n", tstrtostr(result.tstrSHA1).c_str());
                printf("SHA256: %s\n", tstrtostr(result.tstrSHA256).c_str());
                printf("CRC32: %s\n", tstrtostr(result.tstrCRC32).c_str());
            }
            else
            {
                printf("MD5: %s\n", str_lower(tstrtostr(result.tstrMD5)).c_str());
                printf("SHA1: %s\n", str_lower(tstrtostr(result.tstrSHA1)).c_str());
                printf("SHA256: %s\n", str_lower(tstrtostr(result.tstrSHA256)).c_str());
                printf("CRC32: %s\n", str_lower(tstrtostr(result.tstrCRC32)).c_str());
            }
        }
        else
        {
            printf("Failed\n");
        }
        
        printf("\n");
    }
    */
    
    return 0;
}
