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

		int32 GetProgMax();

		void PreparingCalc();
		void RemovePreparingCalc();
		void CalcStop();
		void CalcFinish();

		void ShowFileName(ResultDataNet resultDataNet);
		void ShowFileMeta(ResultDataNet resultDataNet);
		void ShowFileHash(ResultDataNet resultDataNet, Platform::Boolean uppercase);
		void ShowFileErr(ResultDataNet resultDataNet);

		void UpdateProgWhole(int32 value);

		event CalcEventHandler^ PreparingCalcHandler;
		event CalcEventHandler^ RemovePreparingCalcHandler;
		event CalcEventHandler^ CalcStopHandler;
		event CalcEventHandler^ CalcFinishHandler;

		event ResultEventHandler^ ShowFileNameHandler;
		event ResultEventHandler^ ShowFileMetaHandler;
		event ResultHashEventHandler^ ShowFileHashHandler;
		event ResultEventHandler^ ShowFileErrHandler;

		event CalcProgEventHandler^ UpdateProgWholeHandler;
	};
}
