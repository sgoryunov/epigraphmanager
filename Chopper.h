#pragma once
#include "Hardware.h"

class CChopper:private CHardware
{
public:
	CChopper(void);
	CChopper(int sl);
	virtual ~CChopper(void);
	// функция разгона и остановки прерывателя 
	int SetFrequency(int modFrequency);
	int Stop(void);
	// функция инициализации параметров
	int Initialise(void);
private:
	int slot;
	double m_genFrequencyStep;
	int m_TimeDelay;
	int m_WindowsRotorNumber;
	int m_GearRatio;
	int m_InverseStepResolution;
	int m_modCarrentFrequency;

	int SignalDataInit(int mode, double signal_length, float **ppData, int *pSamples, double *pTimeBase );
	int SignalDataDeinit( float **ppData );
	
	double ModToGenFrequency(int modFrequency)
	{
		return double(modFrequency*200*m_InverseStepResolution/m_WindowsRotorNumber/m_GearRatio);
	}
	

};

