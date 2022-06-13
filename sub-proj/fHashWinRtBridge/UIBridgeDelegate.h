#pragma once

#include "ResultDataNet.h"

namespace FilesHashUwp
{
	public delegate void CalcEventHandler();
	public delegate void ResultEventHandler(ResultDataNet);
	public delegate void ResultHashEventHandler(ResultDataNet, Platform::Boolean);
	public delegate void CalcProgEventHandler(int32);

	public ref class UIBridgeDelegate sealed
	{
	public:
		UIBridgeDelegate();

		event CalcEventHandler^ PreparingCalcHandler;
		event CalcEventHandler^ RemovePreparingCalcHandler;
		event CalcEventHandler^ CalcStopHandler;
		event CalcEventHandler^ CalcFinishHandler;

		event ResultEventHandler^ ShowFileNameHandler;
		event ResultEventHandler^ ShowFileMetaHandler;
		event ResultHashEventHandler^ ShowFileHashHandler;
		event ResultEventHandler^ ShowFileErrHandler;

		event CalcProgEventHandler^ UpdateProgHandler;
		event CalcProgEventHandler^ UpdateProgWholeHandler;
	};
}
