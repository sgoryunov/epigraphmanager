#pragma once
#include "hardware.h"
#include "resource.h"

typedef struct ReadTreadData {
    CDialog *pSpectrDlg;
	// буфер данных
	SHORT *AdcBuffer;
	// структура параметров работы АЦП модуля
	ADC_PARS_E2010 ap;
	// размер запроса на сбор данных ReadData()
	DWORD DataStep;
	// структура состояния процесса сбора данных
	DATA_STATE_E2010 DataState;
	// дескриптор устройства
	HANDLE ModuleHandle;
	// идентификатор файла
    HANDLE hFile;
} READTHREADDATA, *PREADTHREADDATA;


class CE2010 :private CHardware
{
public:
	CE2010(void);
	virtual ~CE2010(void);

	// идентификатор потока сбора данных
	HANDLE m_hReadThread;
	DWORD m_ReadTid;
	// указатель на структуру данных для потока ReadThrea
	PREADTHREADDATA m_pReadThreadData;
	// функция потока ввода данных с АЦП
	static DWORD WINAPI ServiceReadThread(LPVOID lpParameters);
	// функция вычисления частоты импульсов канале
//	static WORD GetPulseFrequency(WORD channel, short aBuffer[], double &frequency);
	
	// инициализация Е2010 с заданием параметров АЦП
	int Initialise(void);
	// функция для коректной остановки блока Е2010
	int Stop(void);
private:
	BYTE m_UsbSpeed;
	WORD m_TurbineStatusTTLinPin;
	WORD m_HSMControlChannel;
	// название модуля
	char m_ModuleName[7];
	// структура с полной информацией о модуле
	MODULE_DESCRIPTION_E2010 m_ModuleDescription;
	static int m_FirstPulse[ADC_CHANNELS_QUANTITY_E2010];
	static short m_BuffersBwPulses[ADC_CHANNELS_QUANTITY_E2010];
	static short m_NextDelta[ADC_CHANNELS_QUANTITY_E2010];
	static ADC_PARS_E2010 *m_pAp;
	static double m_KadrRate;
protected:
	static int m_AdcRate;
	static DWORD m_DataStep;
	static short m_ChopperPulseChannel;
	static short m_HSMPulseChannel;
	static short m_MonitorPulseChannel;
	static short m_DetectorPulseChannel;
	static short m_PulseLevel;
	static WORD m_TurbineOpenValue;
	static WORD m_ChannelsQuantity;
	

	
};

