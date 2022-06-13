#include "stdafx.h"

#include "UIBridgeUwp.h"
#include "Common/Global.h"

using namespace Platform;
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
	resultDataNet.Path = ref new String(result.tstrPath.c_str());
	resultDataNet.Size = result.ulSize;
	resultDataNet.ModifiedDate = ref new String(result.tstrMDate.c_str());
	resultDataNet.Version = ref new String(result.tstrVersion.c_str());
	resultDataNet.MD5 = ref new String(result.tstrMD5.c_str());
	resultDataNet.SHA1 = ref new String(result.tstrSHA1.c_str());
	resultDataNet.SHA256 = ref new String(result.tstrSHA256.c_str());
	resultDataNet.SHA512 = ref new String(result.tstrSHA512.c_str());
	resultDataNet.Error = ref new String(result.tstrError.c_str());

	return resultDataNet;
}
