#include "stdafx.h"
#include "UIBridgeDelegates.h"

using namespace System;
using namespace FilesHashWUI;

UIBridgeDelegate::UIBridgeDelegate()
{
}

Int32 UIBridgeDelegate::GetProgMax()
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

void UIBridgeDelegate::ShowFileHash(ResultDataNet resultDataNet, bool uppercase)
{
	ShowFileHashHandler(resultDataNet, uppercase);
}

void UIBridgeDelegate::ShowFileErr(ResultDataNet resultDataNet)
{
	ShowFileErrHandler(resultDataNet);
}

void UIBridgeDelegate::UpdateProgWhole(Int32 value)
{
	UpdateProgWholeHandler(value);
}
