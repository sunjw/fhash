#include "stdafx.h"

#include "UIBridgeMacCmd.h"

#include <stdlib.h>
#include <stdio.h>
#include <string>

#include "strhelper.h"
#include "Global.h"
#include "Functions.h"

using namespace std;
using namespace sunjwbase;

UIBridgeMacCmd::UIBridgeMacCmd()
:m_oldProgPos(0)
{
}

UIBridgeMacCmd::~UIBridgeMacCmd()
{
}

void UIBridgeMacCmd::lockData()
{
}

void UIBridgeMacCmd::unlockData()
{
}

void UIBridgeMacCmd::preparingCalc()
{
    printf("Prepare to start calculation.\n\n");
}

void UIBridgeMacCmd::removePreparingCalc()
{
}

void UIBridgeMacCmd::calcStop()
{
}

void UIBridgeMacCmd::calcFinish()
{
}

void UIBridgeMacCmd::showFileName(const ResultData& result)
{
    printf("%s\n", tstrtostr(result.tstrPath).c_str());
}

void UIBridgeMacCmd::showFileMeta(const ResultData& result)
{
    char chSizeBuff[1024] = {0};
    sprintf(chSizeBuff, "%llu", result.ulSize);
    
    string strShortSize = ConvertSizeToStr(result.ulSize);
    
    printf("File Size: %s Byte(s)%s\n",
           chSizeBuff, strShortSize.c_str());
    printf("Modified Date: %s\n",
           tstrtostr(result.tstrMDate).c_str());
}

void UIBridgeMacCmd::showFileHash(const ResultData& result, bool uppercase)
{
    string strFileMD5, strFileSHA1, strFileSHA256, strFileCRC32;
    
    if (uppercase)
    {
        strFileMD5 = str_upper(tstrtostr(result.tstrMD5));
        strFileSHA1 = str_upper(tstrtostr(result.tstrSHA1));
        strFileSHA256 = str_upper(tstrtostr(result.tstrSHA256));
        strFileCRC32 = str_upper(tstrtostr(result.tstrCRC32));
    }
    else
    {
        strFileMD5 = str_lower(tstrtostr(result.tstrMD5));
        strFileSHA1 = str_lower(tstrtostr(result.tstrSHA1));
        strFileSHA256 = str_lower(tstrtostr(result.tstrSHA256));
        strFileCRC32 = str_lower(tstrtostr(result.tstrCRC32));
    }
    
    printf("MD5: %s\n", strFileMD5.c_str());
    printf("SHA1: %s\n", strFileSHA1.c_str());
    printf("SHA256: %s\n", strFileSHA256.c_str());
    printf("CRC32: %s\n", strFileCRC32.c_str());
}

void UIBridgeMacCmd::showFileErr(const ResultData& result)
{
    printf("%s\n", tstrtostr(result.tstrError).c_str());
}

int UIBridgeMacCmd::getProgMax()
{
	return 40;
}

void UIBridgeMacCmd::updateProg(int value)
{
    if (value < m_oldProgPos)
        m_oldProgPos = 0; // reset
    
    for (int i = 0; i < (value - m_oldProgPos); ++i)
    {
        printf("#");
        fflush(stdout);
    }
    
    m_oldProgPos = value;
}

void UIBridgeMacCmd::updateProgWhole(int value)
{
	
}

void UIBridgeMacCmd::fileCalcFinish()
{
    printf("\n");
}

void UIBridgeMacCmd::fileFinish()
{
    printf("\n");
}
