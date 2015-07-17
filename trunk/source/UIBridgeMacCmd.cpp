#include "stdafx.h"

#include "UIBridgeMacCmd.h"

#include <stdlib.h>
#include <stdio.h>

#include "strhelper.h"
#include "Global.h"

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

void UIBridgeMacCmd::showFileName(const tstring& tstrFileName)
{
    printf("%s\n", tstrtostr(tstrFileName).c_str());
}

void UIBridgeMacCmd::showFileMeta(const tstring& tstrFileSize,
							const tstring& tstrShortSize,
							const tstring& tstrLastModifiedTime,
							const tstring& tstrFileVersion)
{
    printf("File Size: %s Byte(s)%s\n",
           tstrtostr(tstrFileSize).c_str(),
           tstrtostr(tstrShortSize).c_str());
    printf("Modified Date: %s\n",
           tstrtostr(tstrLastModifiedTime).c_str());
}

void UIBridgeMacCmd::showFileHash(const tstring& tstrFileMD5,
							const tstring& tstrFileSHA1,
							const tstring& tstrFileSHA256,
							const tstring& tstrFileCRC32)
{
    printf("MD5: %s\n", tstrtostr(tstrFileMD5).c_str());
    printf("SHA1: %s\n", tstrtostr(tstrFileSHA1).c_str());
    printf("SHA256: %s\n", tstrtostr(tstrFileSHA256).c_str());
    printf("CRC32: %s\n", tstrtostr(tstrFileCRC32).c_str());
}

void UIBridgeMacCmd::showFileErr(const tstring& tstrErr)
{
    printf("%s\n", tstrtostr(tstrErr).c_str());
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
