#pragma once
#include "hardware.h"
#include "resource.h"

typedef struct ReadTreadData {
    CDialog *pSpectrDlg;
	// ����� ������
	SHORT *AdcBuffer;
	// ��������� ���������� ������ ��� ������
	ADC_PARS_E2010 ap;
	// ������ ������� �� ���� ������ ReadData()
	DWORD DataStep;
	// ��������� ��������� �������� ����� ������
	DATA_STATE_E2010 DataState;
	// ���������� ����������
	HANDLE ModuleHandle;
	// ������������� �����
    HANDLE hFile;
} READTHREADDATA, *PREADTHREADDATA;


class CE2010 :private CHardware
{
public:
	CE2010(void);
	virtual ~CE2010(void);

	// ������������� ������ ����� ������
	HANDLE m_hReadThread;
	DWORD m_ReadTid;
	// ��������� �� ��������� ������ ��� ������ ReadThrea
	PREADTHREADDATA m_pReadThreadData;
	// ������� ������ ����� ������ � ���
	static DWORD WINAPI ServiceReadThread(LPVOID lpParameters);
	// ������� ���������� ������� ��������� ������
//	static WORD GetPulseFrequency(WORD channel, short aBuffer[], double &frequency);
	
	// ������������� �2010 � �������� ���������� ���
	int Initialise(void);
	// ������� ��� ��������� ��������� ����� �2010
	int Stop(void);
private:
	BYTE m_UsbSpeed;
	WORD m_TurbineStatusTTLinPin;
	WORD m_HSMControlChannel;
	// �������� ������
	char m_ModuleName[7];
	// ��������� � ������ ����������� � ������
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

