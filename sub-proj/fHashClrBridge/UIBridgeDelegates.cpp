#include "stdafx.h"
#include "UIBridgeDelegates.h"

using namespace System;
using namespace FilesHashWUI;

UIBridgeDelegates::UIBridgeDelegates()
{
}

Int32 UIBridgeDelegates::GetProgMax()
{
	return 100;
}

void UIBridgeDelegates::PreparingCalc()
{
	PreparingCalcHandler();
}

void UIBridgeDelegates::RemovePreparingCalc()
{
	RemovePreparingCalcHandler();
}

void UIBridgeDelegates::CalcStop()
{
	CalcStopHandler();
}

void UIBridgeDelegates::CalcFinish()
{
	CalcFinishHandler();
}

void UIBridgeDelegates::ShowFileName(ResultDataNet resultDataNet)
{
	ShowFileNameHandler(resultDataNet);
}

void UIBridgeDelegates::ShowFileMeta(ResultDataNet resultDataNet)
{
	ShowFileMetaHandler(resultDataNet);
}

void UIBridgeDelegates::ShowFileHash(ResultDataNet resultDataNet, bool uppercase)
{
	ShowFileHashHandler(resultDataNet, uppercase);
}

void UIBridgeDelegates::ShowFileErr(ResultDataNet resultDataNet)
{
	ShowFileErrHandler(resultDataNet);
}

void UIBridgeDelegates::UpdateProgWhole(Int32 value)
{
	UpdateProgWholeHandler(value);
}
