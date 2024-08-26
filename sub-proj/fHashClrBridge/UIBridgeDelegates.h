#pragma once

#include "ResultDataNet.h"

namespace FilesHashWUI
{
	public delegate void CalcEventHandler();
	public delegate void ResultEventHandler(ResultDataNet);
	public delegate void ResultHashEventHandler(ResultDataNet, bool);
	public delegate void CalcProgEventHandler(System::Int32);

	public ref class UIBridgeDelegates sealed
	{
	public:
		UIBridgeDelegates();

		System::Int32 GetProgMax();

		void PreparingCalc();
		void RemovePreparingCalc();
		void CalcStop();
		void CalcFinish();

		void ShowFileName(ResultDataNet resultDataNet);
		void ShowFileMeta(ResultDataNet resultDataNet);
		void ShowFileHash(ResultDataNet resultDataNet, bool uppercase);
		void ShowFileErr(ResultDataNet resultDataNet);

		void UpdateProgWhole(System::Int32 value);

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
