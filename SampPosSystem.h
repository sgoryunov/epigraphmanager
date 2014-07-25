#pragma once
#include "e2010.h"
class CSampPosSystem :
	public CE2010
{
public:
	CSampPosSystem(void);
	CSampPosSystem(CWnd *,CWnd *);
	virtual ~CSampPosSystem(void);
	// функция установки каретки в нужной точке
	bool SetPosition(WORD position);
	static short m_EstCarriagePosition, m_MeasCarriagePosition;
private:
	short m_PulseOrUpPulseTTLoutPin;
	short m_DirOrDownPulseTTLoutPin;
	bool m_IsPulseSignalSys;
	
	WORD m_NumberOfDevisions;
	WORD m_Calibration;
	WORD m_TimeDelay;
	WORD m_MinDistanceBwFilters;
	WORD m_CarriageWorkRange;
	WORD m_StepsInMillimetre;
	WORD m_LuftInMillimetre;
	WORD m_CurCarriagePosInSteps;
	CWnd *m_pEditForEstPos;
	CWnd *m_pEditForMeasPos;
	void KillThelLuft(void);
};

