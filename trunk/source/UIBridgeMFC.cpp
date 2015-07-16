#include "stdafx.h"

#include "UIBridgeMFC.h"

#include <stdlib.h>
#include <Windows.h>

#include "strhelper.h"
#include "OsUtils/OsThread.h"

using namespace sunjwbase;

extern OsMutex g_mainMtx;

UIBridgeMFC::UIBridgeMFC(HWND hWnd)
:m_hWnd(hWnd)
{
}

UIBridgeMFC::~UIBridgeMFC()
{
}

void UIBridgeMFC::preparingCalc()
{

}

void UIBridgeMFC::removePreparingCalc()
{

}

void UIBridgeMFC::calcStop()
{

}

void UIBridgeMFC::calcFinish()
{

}

void UIBridgeMFC::showFileName(const tstring& tstrFileName)
{

}

void UIBridgeMFC::showFileMeta(const tstring& tstrFileSize,
							const tstring& tstrShortSize,
							const tstring& tstrLastModifiedTime,
							const tstring& tstrFileVersion)
{

}

void UIBridgeMFC::showFileHash(const tstring& tstrFileMD5,
							const tstring& tstrFileSHA1,
							const tstring& tstrFileSHA256,
							const tstring& tstrFileCRC32)
{

}

void UIBridgeMFC::showFileErr(const tstring& tstrErr)
{

}

int UIBridgeMFC::getProgMax()
{
	return 100;
}

void UIBridgeMFC::updateProg(int value)
{
	
}

void UIBridgeMFC::updateProgWhole(int value)
{

}

void UIBridgeMFC::fileCalcFinish()
{

}

void UIBridgeMFC::fileFinish()
{

}
