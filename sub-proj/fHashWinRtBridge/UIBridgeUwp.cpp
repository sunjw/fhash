#include "stdafx.h"

#include "UIBridgeUwp.h"
#include "Common/Global.h"
#include "CxHelper.h"

using namespace Platform;
using namespace FilesHashUwp;
using namespace sunjwbase;

UIBridgeUwp::UIBridgeUwp(UIBridgeDelegate^ uiBridgeDelegate)
	:m_uiBridgeDelegate(uiBridgeDelegate)
{
}

UIBridgeUwp::~UIBridgeUwp()
{
}

void UIBridgeUwp::lockData()
{
	// No need here.
}

void UIBridgeUwp::unlockData()
{
	// No need here.
}

void UIBridgeUwp::preparingCalc()
{
	m_uiBridgeDelegate->PreparingCalc();
}

void UIBridgeUwp::removePreparingCalc()
{
	m_uiBridgeDelegate->RemovePreparingCalc();
}

void UIBridgeUwp::calcStop()
{
	m_uiBridgeDelegate->CalcStop();
}

void UIBridgeUwp::calcFinish()
{
	m_uiBridgeDelegate->CalcFinish();
}

void UIBridgeUwp::showFileName(const ResultData& result)
{
	ResultDataNet resultDataNet = ConvertResultDataToNet(result);
	m_uiBridgeDelegate->ShowFileName(resultDataNet);
}

void UIBridgeUwp::showFileMeta(const ResultData& result)
{
	ResultDataNet resultDataNet = ConvertResultDataToNet(result);
	m_uiBridgeDelegate->ShowFileMeta(resultDataNet);
}

void UIBridgeUwp::showFileHash(const ResultData& result, bool uppercase)
{
	ResultDataNet resultDataNet = ConvertResultDataToNet(result);
	m_uiBridgeDelegate->ShowFileHash(resultDataNet, uppercase);
}

void UIBridgeUwp::showFileErr(const ResultData& result)
{
	ResultDataNet resultDataNet = ConvertResultDataToNet(result);
	m_uiBridgeDelegate->ShowFileErr(resultDataNet);
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
	m_uiBridgeDelegate->UpdateProgWhole(value);
}

void UIBridgeUwp::fileCalcFinish()
{
}

void UIBridgeUwp::fileFinish()
{
}

ResultDataNet UIBridgeUwp::ConvertResultDataToNet(const ResultData& result)
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
	resultDataNet.Path = ConvertTstrToPlatStr(result.tstrPath);
	resultDataNet.Size = result.ulSize;
	resultDataNet.ModifiedDate = ConvertTstrToPlatStr(result.tstrMDate);
	resultDataNet.Version = ConvertTstrToPlatStr(result.tstrVersion);
	resultDataNet.MD5 = ConvertTstrToPlatStr(result.tstrMD5);
	resultDataNet.SHA1 = ConvertTstrToPlatStr(result.tstrSHA1);
	resultDataNet.SHA256 = ConvertTstrToPlatStr(result.tstrSHA256);
	resultDataNet.SHA512 = ConvertTstrToPlatStr(result.tstrSHA512);
	resultDataNet.Error = ConvertTstrToPlatStr(result.tstrError);

	return resultDataNet;
}
