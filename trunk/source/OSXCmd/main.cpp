//
//  main.cpp
//  fHashCmd
//
//  Created by Sun Junwen on 15/5/9.
//  Copyright (c) 2015年 Sun Junwen. All rights reserved.
//

#include <stdio.h>
#include <string>

#include "Global.h"
#include "OsThread.h"
#include "HashEngine.h"

using namespace std;
using namespace sunjwbase;

OsMutex g_mainMtx;

int main(int argc, char *argv[])
{
    ThreadData thData;
    
    thData.threadWorking = false;
    thData.stop = false;
    thData.uppercase = false;
    
    string strPath = "/Users/sunjw/temp/com.microsoft.office.officehub-2.apk";
    tstring tstrPath = strtotstr(strPath);
    thData.fullPaths.push_back(tstrPath);
    thData.nFiles = 1;
    
    HashThreadFunc(&thData);
    
    printf("DONE\n");
    
    return 0;
}
