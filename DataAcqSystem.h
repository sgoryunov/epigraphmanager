#pragma once
#include "e2010.h"
#include "SpectrDlg.h"
#include <fstream>

using std::ofstream;
using std::ios;

struct PERIODS_COUNTING_DATA
{
	WORD periodsNumber;
	long long sumOfPeriods;
	long long lastPulseTime;
	DWORD partOfPeriod;
};

class CDataAcqSystem :
	private CE2010
{
public:
	CDataAcqSystem(void);
	CDataAcqSystem(CString,CWnd *graphDlg);
	virtual ~CDataAcqSystem(void);
	
	// ������� ������� ��������� �������� �������
	void SetTurbineFlag(bool isTurbineOpen);
	// ������� ������ ������
	WORD WriteData(short * buffer, WORD requestNumber);
	
	
	// ������� �������� ��������� �������� �������
	void CDataAcqSystem::SendTurbineOpenFlag(bool flag, WORD requestNumber)
	{
		if(m_IsTurbineOpen[0]!=m_IsTurbineOpen[1]) m_IsTurbineStatusChanged = true;
		if(!m_IsTurbineOpen[requestNumber] && m_IsTurbineOpen[requestNumber^1]) m_IsChamberFilling = true;
		m_IsTurbineOpen[requestNumber] = flag;
	}
	// ������ ������ ������ ���������� ������
	bool IsChamberFalling(void){return m_IsChamberFilling;}
	// ������� ������ ��������
	static void ResetTheTimer(void)
	{ 
		m_IsTimerReseted = true;
		CSpectrDlg::m_CurMeasTime = 0;
	}
	// ������� �������� ���������� ������� ����������
	void SendControlSysParam(int estPos, int measPos, bool isSysInMeasCond, WORD requestNumber)
	{
		if(m_IsSysInMeasConditions[requestNumber^1] && !m_IsSysInMeasConditions[requestNumber]) m_CreateNewFile = true ;
		m_IsSysInMeasConditions[requestNumber] = isSysInMeasCond;
		m_CarriageEstPos[requestNumber] = long long(estPos);
		m_CarriageMeasPos[requestNumber] = long long(measPos);
	}
	// ������� ��������� �������� �� ������
	CString GetSignalFrequency(WORD channel, double & frequency);
private:
	CWnd *m_pSpectrDlg;
	long long m_aRisingEdge[ADC_CHANNELS_QUANTITY_E2010];
	long long m_LimitOfTimeInFileInADCCount;
	WORD m_SecondsNumberInFile;
	short m_aNextDelta[ADC_CHANNELS_QUANTITY_E2010];
	bool m_IsTurbineOpen[2];
	bool m_CreateNewFile;
	bool m_IsTurbineStatusChanged;
	bool m_IsChamberFilling;
	bool m_IsSysInMeasConditions[2];
	long long m_CarriageEstPos[2], m_CarriageMeasPos[2];	
	CString m_DataDirectory;
	CString m_ConstPartOfDataFile;
	CString m_DataFileName;
	CString m_ExacutableFileDir;
	DWORD m_FileSize;
	// ������� �������
	WORD m_FileNumber;
	// ����� ����� ������
	WORD m_requestNumber;
	// ������� �������� ������� � �����
	long long m_GoodCollectionTime;
	// ����� ���������� ������ � �������� ��� �������������� 10 ���
	DWORD m_ChamberFillingTimeInADCCounts;
	DWORD m_ChamberFillingCounter;
	// ������ ��������� � ������� ������
	short m_EdgeLevel[2];
	short m_CurEdgeLevel[ADC_CHANNELS_QUANTITY_E2010];
	// ������������� ������
	ofstream m_DataFile;
	//������
	static long long m_Timer;
	static bool m_IsTimerReseted;
	// ������� ��������� ������ �����
	WORD ProcessFile(CString inRawDataFileName);
	void GetProcessedFileName(WORD modFr, WORD hsmFr, WORD carriagePos, CString *);
	// ������� ������ �����, ������� ������������ � �������
	// ��� �����
	bool WriteGraphFile(WORD pointNumber, WORD * point, DWORD * detPulseNumber, WORD * timeInPointInSec);
	// � ��� ��������� 
	bool WriteGraphFile(WORD frequency, WORD cannelNumber, DWORD * destribution, WORD measTime);
	void PutEventInDestrib(DWORD value, DWORD valueIn, DWORD valueFin, const WORD channelNumber, DWORD * destribArray);
	//��� ������� ���������� ������� �������
	// ������� �������� ��� ���������� �������� �������
	// �� �������� ������� � �����������
	PERIODS_COUNTING_DATA m_PeriodsCountingData[2];
	// �������� �������� � ��������� � ��������
	WORD m_DetectorCounter;
	// ������� ������� ��� ����������� �������� ������ ����������
	// 0 - ������� � �����
	// 1 - ������ ������ �������� � �����
	DWORD m_CountRateTimingData[2];
	DWORD m_MonitorCounter;
	inline void DoWhenEdgeHasBeenFound(void);
protected:
	inline CString  GetFileNameAsDirName(CString dirName);
	// ������� ������ ������
	inline bool WriteEvent(BYTE id, long long * value);
	
};

