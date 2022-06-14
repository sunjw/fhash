#include "stdafx.h"
#include "UIBridgeDelegate.h"

using namespace Platform;
using namespace FilesHashUwp;

UIBridgeDelegate::UIBridgeDelegate()
{
}

int32 UIBridgeDelegate::GetProgMax()
{
	return 100;
}

void UIBridgeDelegate::PreparingCalc()
{
	PreparingCalcHandler();
}

void UIBridgeDelegate::RemovePreparingCalc()
{
	RemovePreparingCalcHandler();
}

void UIBridgeDelegate::CalcStop()
{
	CalcStopHandler();
}

void UIBridgeDelegate::CalcFinish()
{
	CalcFinishHandler();
}

void UIBridgeDelegate::ShowFileName(ResultDataNet resultDataNet)
{
	ShowFileNameHandler(resultDataNet);
}

void UIBridgeDelegate::ShowFileMeta(ResultDataNet resultDataNet)
{
	ShowFileMetaHandler(resultDataNet);
}

void UIBridgeDelegate::ShowFileHash(ResultDataNet resultDataNet, Boolean uppercase)
{
	ShowFileHashHandler(resultDataNet, uppercase);
}

void UIBridgeDelegate::ShowFileErr(ResultDataNet resultDataNet)
{
	ShowFileErrHandler(resultDataNet);
}

void UIBridgeDelegate::UpdateProgWhole(int32 value)
{
	UpdateProgWholeHandler(value);
}
