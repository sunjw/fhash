#include "stdafx.h"

#include "UIBridgeWUI.h"

#include "Common/Global.h"
#include "ClrHelper.h"

using namespace System;
using namespace FilesHashWUI;
using namespace sunjwbase;

static ResultDataNet ConvertResultDataToNet(const ResultData& result)
{
	ResultDataNet resultDataNet;
	switch (result.enumState)
	{
	case ResultState::RESULT_NONE:
		resultDataNet.EnumState = ResultStateNet::ResultNone;
		break;
	case ResultState::RESULT_PATH:
		resultDataNet.EnumState = ResultStateNet::ResultPath;
		break;
	case ResultState::RESULT_META:
		resultDataNet.EnumState = ResultStateNet::ResultMeta;
		break;
	case ResultState::RESULT_ALL:
		resultDataNet.EnumState = ResultStateNet::ResultAll;
		break;
	case ResultState::RESULT_ERROR:
		resultDataNet.EnumState = ResultStateNet::ResultError;
		break;
	}

	resultDataNet.Path = ConvertTstrToSystemString(result.tstrPath.c_str());
	resultDataNet.Size = result.ulSize;
	resultDataNet.ModifiedDate = ConvertTstrToSystemString(result.tstrMDate.c_str());
	resultDataNet.Version = ConvertTstrToSystemString(result.tstrVersion.c_str());
	resultDataNet.MD5 = ConvertTstrToSystemString(result.tstrMD5.c_str());
	resultDataNet.SHA1 = ConvertTstrToSystemString(result.tstrSHA1.c_str());
	resultDataNet.SHA256 = ConvertTstrToSystemString(result.tstrSHA256.c_str());
	resultDataNet.SHA512 = ConvertTstrToSystemString(result.tstrSHA512.c_str());
	resultDataNet.Error = ConvertTstrToSystemString(result.tstrError.c_str());

	return resultDataNet;
}

UIBridgeWUI::UIBridgeWUI(UIBridgeDelegate^ uiBridgeDelegate)
	:m_uiBridgeDelegate(uiBridgeDelegate)
{
}

UIBridgeWUI::~UIBridgeWUI()
{
}

void UIBridgeWUI::lockData()
{
	// No need here.
}

void UIBridgeWUI::unlockData()
{
	// No need here.
}

void UIBridgeWUI::preparingCalc()
{
	m_uiBridgeDelegate->PreparingCalc();
}

void UIBridgeWUI::removePreparingCalc()
{
	m_uiBridgeDelegate->RemovePreparingCalc();
}

void UIBridgeWUI::calcStop()
{
	m_uiBridgeDelegate->CalcStop();
}

void UIBridgeWUI::calcFinish()
{
	m_uiBridgeDelegate->CalcFinish();
}

void UIBridgeWUI::showFileName(const ResultData& result)
{
	ResultDataNet resultDataNet = ConvertResultDataToNet(result);
	m_uiBridgeDelegate->ShowFileName(resultDataNet);
}

void UIBridgeWUI::showFileMeta(const ResultData& result)
{
	ResultDataNet resultDataNet = ConvertResultDataToNet(result);
	m_uiBridgeDelegate->ShowFileMeta(resultDataNet);
}

void UIBridgeWUI::showFileHash(const ResultData& result, bool uppercase)
{
	ResultDataNet resultDataNet = ConvertResultDataToNet(result);
	m_uiBridgeDelegate->ShowFileHash(resultDataNet, uppercase);
}

void UIBridgeWUI::showFileErr(const ResultData& result)
{
	ResultDataNet resultDataNet = ConvertResultDataToNet(result);
	m_uiBridgeDelegate->ShowFileErr(resultDataNet);
}

int UIBridgeWUI::getProgMax()
{
	return m_uiBridgeDelegate->GetProgMax();
}

void UIBridgeWUI::updateProg(int value)
{
}

void UIBridgeWUI::updateProgWhole(int value)
{
	m_uiBridgeDelegate->UpdateProgWhole(value);
}

void UIBridgeWUI::fileCalcFinish()
{
}

void UIBridgeWUI::fileFinish()
{
}
