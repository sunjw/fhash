#import "UIBridgeMacUI.h"

#include <stdlib.h>
#include <string>

#include "strhelper.h"
#include "Global.h"
#include "Utils.h"

using namespace std;
using namespace sunjwbase;

UIBridgeMacUI::UIBridgeMacUI(MainViewController *mainViewController,
                             OsMutex *mainMtx)
:_mainViewControllerPtr(mainViewController), _mainMtx(mainMtx)
{
}

UIBridgeMacUI::~UIBridgeMacUI()
{
}

void UIBridgeMacUI::lockData()
{
    _mainMtx->lock();
}

void UIBridgeMacUI::unlockData()
{
    _mainMtx->unlock();
}

void UIBridgeMacUI::preparingCalc()
{
    
}

void UIBridgeMacUI::removePreparingCalc()
{
}

void UIBridgeMacUI::calcStop()
{
}

void UIBridgeMacUI::calcFinish()
{
}

void UIBridgeMacUI::showFileName(const ResultData& result)
{
    
}

void UIBridgeMacUI::showFileMeta(const ResultData& result)
{
    char chSizeBuff[1024] = {0};
    sprintf(chSizeBuff, "%llu", result.ulSize);
    
    string strShortSize = Utils::ConvertSizeToShortSizeStr(result.ulSize);
    
    
}

void UIBridgeMacUI::showFileHash(const ResultData& result, bool uppercase)
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
    
    
}

void UIBridgeMacUI::showFileErr(const ResultData& result)
{
    
}

int UIBridgeMacUI::getProgMax()
{
    return 100;
}

void UIBridgeMacUI::updateProg(int value)
{
    
}

void UIBridgeMacUI::updateProgWhole(int value)
{
    
}

void UIBridgeMacUI::fileCalcFinish()
{
    
}

void UIBridgeMacUI::fileFinish()
{
    
}
