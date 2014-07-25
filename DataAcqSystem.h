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
	
	// функция задания параметра открытия турбины
	void SetTurbineFlag(bool isTurbineOpen);
	// функция записи данных
	WORD WriteData(short * buffer, WORD requestNumber);
	
	
	// функция передачи параметра открытия турбины
	void CDataAcqSystem::SendTurbineOpenFlag(bool flag, WORD requestNumber)
	{
		if(m_IsTurbineOpen[0]!=m_IsTurbineOpen[1]) m_IsTurbineStatusChanged = true;
		if(!m_IsTurbineOpen[requestNumber] && m_IsTurbineOpen[requestNumber^1]) m_IsChamberFilling = true;
		m_IsTurbineOpen[requestNumber] = flag;
	}
	// фунция запрса режима заполнения камеры
	bool IsChamberFalling(void){return m_IsChamberFilling;}
	// функция сброса счетчика
	static void ResetTheTimer(void)
	{ 
		m_IsTimerReseted = true;
		CSpectrDlg::m_CurMeasTime = 0;
	}
	// функция передачи параметров системы управления
	void SendControlSysParam(int estPos, int measPos, bool isSysInMeasCond, WORD requestNumber)
	{
		if(m_IsSysInMeasConditions[requestNumber^1] && !m_IsSysInMeasConditions[requestNumber]) m_CreateNewFile = true ;
		m_IsSysInMeasConditions[requestNumber] = isSysInMeasCond;
		m_CarriageEstPos[requestNumber] = long long(estPos);
		m_CarriageMeasPos[requestNumber] = long long(measPos);
	}
	// функция получения частотты по каналу
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
	// счетсик файлаов
	WORD m_FileNumber;
	// номер части буфера
	WORD m_requestNumber;
	// счетчик хорошего времени в точке
	long long m_GoodCollectionTime;
	// время наполнения камеры в отсчетах АЦП соответсвующее 10 сек
	DWORD m_ChamberFillingTimeInADCCounts;
	DWORD m_ChamberFillingCounter;
	// уровни переднего и заднего фронта
	short m_EdgeLevel[2];
	short m_CurEdgeLevel[ADC_CHANNELS_QUANTITY_E2010];
	// идентификатор потока
	ofstream m_DataFile;
	//таймер
	static long long m_Timer;
	static bool m_IsTimerReseted;
	// функция обработки сырого файла
	WORD ProcessFile(CString inRawDataFileName);
	void GetProcessedFileName(WORD modFr, WORD hsmFr, WORD carriagePos, CString *);
	// Функция записи файла, который отображается в графике
	// для скана
	bool WriteGraphFile(WORD pointNumber, WORD * point, DWORD * detPulseNumber, WORD * timeInPointInSec);
	// и для осциляции 
	bool WriteGraphFile(WORD frequency, WORD cannelNumber, DWORD * destribution, WORD measTime);
	void PutEventInDestrib(DWORD value, DWORD valueIn, DWORD valueFin, const WORD channelNumber, DWORD * destribArray);
	//для функции вычисления частоты сигнала
	// массивы структур для вычисления периодов сигнала
	// от вращения решотки и прерывателя
	PERIODS_COUNTING_DATA m_PeriodsCountingData[2];
	// счетчики сигналов с детектора и монитора
	WORD m_DetectorCounter;
	// счетчик времени для вычичсления скорости счетав детекторов
	// 0 - счетчик в тиках
	// 1 - момент сброса счетчика в тиках
	DWORD m_CountRateTimingData[2];
	DWORD m_MonitorCounter;
	inline void DoWhenEdgeHasBeenFound(void);
protected:
	inline CString  GetFileNameAsDirName(CString dirName);
	// функция записи данных
	inline bool WriteEvent(BYTE id, long long * value);
	
};

