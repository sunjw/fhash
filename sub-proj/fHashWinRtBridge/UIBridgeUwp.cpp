#include "stdafx.h"

#include "UIBridgeUwp.h"
#include "Common/Global.h"

using namespace FilesHashUwp;

UIBridgeUwp::UIBridgeUwp(UIBridgeDelegate^ uiBridgeDelegate)
	:m_uiBridgeDelegate(uiBridgeDelegate)
{
}

UIBridgeUwp::~UIBridgeUwp()
{
}

void UIBridgeUwp::lockData()
{
}

void UIBridgeUwp::unlockData()
{
}

void UIBridgeUwp::preparingCalc()
{
}

void UIBridgeUwp::removePreparingCalc()
{
}

void UIBridgeUwp::calcStop()
{
}

void UIBridgeUwp::calcFinish()
{
}

void UIBridgeUwp::showFileName(const ResultData& result)
{
}

void UIBridgeUwp::showFileMeta(const ResultData& result)
{
}

void UIBridgeUwp::showFileHash(const ResultData& result, bool uppercase)
{
}

void UIBridgeUwp::showFileErr(const ResultData& result)
{
}

int UIBridgeUwp::getProgMax()
{
	return 100;
}

void UIBridgeUwp::updateProg(int value)
{
}

void UIBridgeUwp::updateProgWhole(int value)
{
}

void UIBridgeUwp::fileCalcFinish()
{
}

void UIBridgeUwp::fileFinish()
{
}

ResultDataNet^ UIBridgeUwp::ConvertResultDataToNet(const ResultData& result)
{
	ResultDataNet^ resultDataNet = ref new ResultDataNet();

	return resultDataNet;
}
