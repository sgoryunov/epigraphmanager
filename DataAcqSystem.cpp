#include "StdAfx.h"
#include "DataAcqSystem.h"


#define EV_IN_CHOPPER_CHAN					0
#define EV_IN_HSM_CHAN						1
#define EV_IN_DETECTOR_CHAN					2
#define EV_IN_MONITOR_CHAN					3
#define EV_IN_TURBINE_OPEN_CHAN				4
#define EV_IN_TURBINE_CLOSE_CHAN			5
#define EV_IN_CARRIAGE_EST_POS_CHAN			6
#define EV_IN_CARRIAGE_MEAS_POS_CHAN		7

#define SCAN_FILENAME						0
#define OSCIL_FILENAME						1

#define RISING								0
#define FALLING								1

// ������� ���  ������� ����������� ���������� � �������� ������� � 
#define CHOPPER_SIGNAL						0
#define HSM_SIGNAL							1



long long CDataAcqSystem::m_Timer = 0ll;// ������� ������
bool CDataAcqSystem::m_IsTimerReseted = true;

using std::ifstream;


CDataAcqSystem::CDataAcqSystem(void)
{
}
CDataAcqSystem::CDataAcqSystem(CString str = _T(""),CWnd * pWnd = NULL)
	: m_DataDirectory(str), m_pSpectrDlg(pWnd)
{
	// ������� ����� ������ ���� ������ ������
	if(!m_DataDirectory.IsEmpty())
		m_ConstPartOfDataFile = m_DataDirectory+GetFileNameAsDirName(m_DataDirectory);
	else m_ConstPartOfDataFile = _T("default");
	m_DataFileName.Empty();
	// ������� ���� � ������� ����������
	CString filePath(_T(""));
	GetModuleFileName(NULL,filePath.GetBufferSetLength(_MAX_PATH),_MAX_PATH);
	m_ExacutableFileDir = CSpectrDlg::GetDirPathOfFile(filePath);
	// ������ ������ ���������� 0 � ������� 1 ��� �������
	m_EdgeLevel[RISING] = 4000;
	m_EdgeLevel[FALLING] = 1000;
	for(int i(0);i<ADC_CHANNELS_QUANTITY_E2010;i++) 
	{
		m_aRisingEdge[i] = -1;
		m_aNextDelta[i] = 0;
		m_CurEdgeLevel[i] = m_EdgeLevel[RISING];
	}
	//������������� ���������� ������� ���������
	m_IsTurbineStatusChanged = true;

	m_CarriageEstPos[0] = m_CarriageEstPos[1] = 0ll;
	m_CarriageMeasPos[0] = m_CarriageMeasPos[1] = 0ll;
	m_IsTurbineOpen[0] = m_IsTurbineOpen[1] = false;
	m_IsSysInMeasConditions[0] = m_IsSysInMeasConditions[1] = false;
	// ���� �������� ������ �����
	m_CreateNewFile = false;
	// ������� ������� ����
	m_FileNumber = 0;
	m_requestNumber = 0x0;
	// ������� ������
	ResetTheTimer();
	// ������� ��������� � ������ ��������� ���������
	m_DetectorCounter = m_MonitorCounter = 0;
	// ������� ������� ������. ��� ���������� �������
	ZeroMemory(m_PeriodsCountingData,2*sizeof(PERIODS_COUNTING_DATA));
	// ����� ���������� ������ 10 ���.
	m_ChamberFillingTimeInADCCounts = 10*m_AdcRate*1000;
	// ������� ������� ����������
	m_ChamberFillingCounter = 0;
	// ������ �������� ����������
	m_IsChamberFilling = false;
	// ��������� ����� ���������� � ����� �� 20���
	m_SecondsNumberInFile = 20;
	m_LimitOfTimeInFileInADCCount = long long(m_SecondsNumberInFile*m_AdcRate*1000);
	// ������� �������� ������� � ����� � ���
	m_GoodCollectionTime = 0.;
	// ������� ������� ������������ ������� � ������� ������ ������
	m_CountRateTimingData[0] = 0ul;
	// ������ ������ ��������� ������ 10 ���
	m_CountRateTimingData[1] = m_AdcRate*1000*10;
}


CDataAcqSystem::~CDataAcqSystem(void)
{
	bool  noError(true);
	if(m_DataFile.is_open())
	{
		if (m_IsTurbineOpen[m_requestNumber^1])
		{
			if(m_IsChamberFilling)
				{if(!WriteEvent(EV_IN_TURBINE_CLOSE_CHAN, &m_Timer)) noError = false;}
			else
				{if(!WriteEvent(EV_IN_TURBINE_OPEN_CHAN, &m_Timer)) noError = false;}
		}
		else
			{if(!WriteEvent(EV_IN_TURBINE_CLOSE_CHAN,&m_Timer)) noError = false;}
		m_DataFile.close();
	}
	if(!noError) CSpectrDlg::PrintError(32);
}

// ������� ������ ������
WORD CDataAcqSystem::WriteData(short * buffer, WORD requestNumber)
{
	short aLastDelta[ADC_CHANNELS_QUANTITY_E2010]={0};
	BYTE idOfEvent(0);
	m_requestNumber = requestNumber;
	// �������� ������� �� ������
	if(m_IsTimerReseted && m_IsSysInMeasConditions[requestNumber])
	{
		// ������� ���������� ���������� ��������� ������������������ ������� ������
		// ������ ��� ���������� ������� �������
		if(m_PeriodsCountingData[CHOPPER_SIGNAL].lastPulseTime != 0)
		{
			if(m_aRisingEdge[m_ChopperPulseChannel] == -1)
			{
				m_PeriodsCountingData[CHOPPER_SIGNAL].partOfPeriod = 
					m_Timer - m_PeriodsCountingData[CHOPPER_SIGNAL].lastPulseTime;
				m_PeriodsCountingData[CHOPPER_SIGNAL].lastPulseTime = 0ll;
				m_PeriodsCountingData[CHOPPER_SIGNAL].sumOfPeriods = 0ll;
				m_PeriodsCountingData[CHOPPER_SIGNAL].periodsNumber = 0u;
			}
			else
			{
				m_PeriodsCountingData[CHOPPER_SIGNAL].periodsNumber = 1;
				m_PeriodsCountingData[CHOPPER_SIGNAL].sumOfPeriods =
					m_aRisingEdge[m_ChopperPulseChannel] - m_PeriodsCountingData[CHOPPER_SIGNAL].lastPulseTime;
				m_PeriodsCountingData[CHOPPER_SIGNAL].partOfPeriod = m_Timer - m_aRisingEdge[m_ChopperPulseChannel];
				m_PeriodsCountingData[CHOPPER_SIGNAL].lastPulseTime = 0ll;
			}
		}
		if(m_PeriodsCountingData[HSM_SIGNAL].lastPulseTime != 0)
		{
			if(m_aRisingEdge[m_HSMPulseChannel] == -1)
			{
				m_PeriodsCountingData[HSM_SIGNAL].partOfPeriod = 
					m_Timer - m_PeriodsCountingData[HSM_SIGNAL].lastPulseTime;
				m_PeriodsCountingData[HSM_SIGNAL].lastPulseTime = 0ll;
				m_PeriodsCountingData[HSM_SIGNAL].sumOfPeriods = 0ll;
				m_PeriodsCountingData[HSM_SIGNAL].periodsNumber = 0u;
			}
			else
			{
				m_PeriodsCountingData[HSM_SIGNAL].periodsNumber = 1;
				m_PeriodsCountingData[HSM_SIGNAL].sumOfPeriods =
					m_aRisingEdge[m_HSMPulseChannel] - m_PeriodsCountingData[HSM_SIGNAL].lastPulseTime;
				m_PeriodsCountingData[HSM_SIGNAL].partOfPeriod = m_Timer - m_aRisingEdge[m_HSMPulseChannel];
				m_PeriodsCountingData[HSM_SIGNAL].lastPulseTime = 0ll;
			}
		}
		m_Timer = 0ll;
		m_GoodCollectionTime = 0.;
		m_IsTimerReseted = false;
		m_LimitOfTimeInFileInADCCount = long long(m_SecondsNumberInFile*m_AdcRate*1000);
		
		// ������� �������� ��������
		for(int i(0);i<ADC_CHANNELS_QUANTITY_E2010;i++) 
		{
			m_aRisingEdge[i] = -1;
			m_aNextDelta[i] = 0;
			m_CurEdgeLevel[i] = m_EdgeLevel[RISING];
		}
	}
	// �������� ����, ���� �����
	if(m_CreateNewFile)
	{
		// ������� ������ ����, ���� �����
		if(m_DataFile.is_open()) m_DataFile.close();
		// ������� �������� ��� ������ �����
		m_FileNumber++;
		if(m_FileNumber<10)
			m_DataFileName.Format(m_ConstPartOfDataFile + _T("_0000%i.dat"),m_FileNumber);
		if(10<=m_FileNumber && m_FileNumber<100)
			m_DataFileName.Format(m_ConstPartOfDataFile + _T("_000%i.dat"),m_FileNumber);
		if(100<=m_FileNumber && m_FileNumber<1000)
			m_DataFileName.Format(m_ConstPartOfDataFile + _T("_00%i.dat"),m_FileNumber);
		if(1000<=m_FileNumber && m_FileNumber<10000)
			m_DataFileName.Format(m_ConstPartOfDataFile + _T("_0%i.dat"),m_FileNumber);
		if(10000<=m_FileNumber && m_FileNumber<100000)
			m_DataFileName.Format(m_ConstPartOfDataFile + _T("_%i.dat"),m_FileNumber);
		//��������� ����� ����
		m_DataFile.open(m_DataFileName,ios::out | ios::binary);
		// �������� �������� �� �� ���������� �����
		if(!m_DataFile) return WORD(36);
		// �������� � ����� ����� ��������� ������� � ��������� �������, 
		//���� ���� ����������� ������ ������ ��������� � ��������� �������
		// ��������� �������
		if (m_IsTurbineOpen[requestNumber])
		{	
			// ���� ������ ����������� ������� ������� �������� ������� 
			// ���� ��� �������, ��� ������� ���
			if(m_IsChamberFilling)
			{
				if(!WriteEvent(EV_IN_TURBINE_CLOSE_CHAN,&m_Timer)) return 32;
				m_IsTurbineStatusChanged = true;
			}
			else
			{
				if(!WriteEvent(EV_IN_TURBINE_OPEN_CHAN, &m_Timer)) return 32;
				// ������� ������ ��������� ��������� �������
				m_IsTurbineStatusChanged = false;
			}
		}
		else
		{
			if(!WriteEvent(EV_IN_TURBINE_CLOSE_CHAN,&m_Timer)) return 32;
			// ������� ������ ��������� ��������� �������
			m_IsTurbineStatusChanged = false;
		}
		// ��������� �������
		if(!WriteEvent(EV_IN_CARRIAGE_EST_POS_CHAN,m_CarriageEstPos + requestNumber)) return 32;
		if(!WriteEvent(EV_IN_CARRIAGE_MEAS_POS_CHAN,m_CarriageMeasPos + requestNumber)) return 32;
		// ������� ���� �������� ������ �����
		m_CreateNewFile = false;
	}
	else
	{
		if(m_DataFile.is_open() && m_IsSysInMeasConditions[requestNumber])
		{
			// ���� ��������� ������� ���������� ����������� ���
			if(m_IsTurbineStatusChanged)
			{
				if (m_IsTurbineOpen[requestNumber])
				{
					// ���� ������ ����������� ������� ������� �������� ������� 
					if(!m_IsChamberFilling)
					{
						if(!WriteEvent(EV_IN_TURBINE_OPEN_CHAN, &m_Timer)) return 32;
						m_IsTurbineStatusChanged = false;
					}
				}
				else
				{
					if(!WriteEvent(EV_IN_TURBINE_CLOSE_CHAN,&m_Timer)) return 32;
					m_IsTurbineStatusChanged = false;
				}
			}
		}
		// ��������� ��������� � �����, ������� ���������������, ������� ���� � ������� �������� �������
		if(m_DataFile.is_open() && !m_IsSysInMeasConditions[requestNumber])
		{
			if (m_IsTurbineOpen[requestNumber])
			{
				if(m_IsChamberFilling)
					{if(!WriteEvent(EV_IN_TURBINE_CLOSE_CHAN, &m_Timer)) return 32;}
				else
					{if(!WriteEvent(EV_IN_TURBINE_OPEN_CHAN, &m_Timer)) return 32;}
			}
			else
				{if(!WriteEvent(EV_IN_TURBINE_CLOSE_CHAN,&m_Timer)) return 32;}
			m_DataFile.close();
		}
	}
	// ������� ������ ��������� ��� ������� ���������� �������
	for(WORD j(0);j<2;j++)
	{
		if(m_Timer-m_PeriodsCountingData[j].lastPulseTime > m_AdcRate*1500)
			ZeroMemory(m_PeriodsCountingData+j,sizeof(PERIODS_COUNTING_DATA));
		else
		{
			if(m_Timer != 0)
			{
				m_PeriodsCountingData[j].periodsNumber = 0u;
				m_PeriodsCountingData[j].sumOfPeriods = 0ll;
			}
		}
	}
	// ���� ���������� ������

	// �������� ������ �������� �����
	for(DWORD i(0);i<m_DataStep;i+=m_ChannelsQuantity)
	{
		for(WORD j(0);j<m_ChannelsQuantity;j++)
		{
			aLastDelta[j] = m_aNextDelta[j];
			m_aNextDelta[j] = buffer[i+j] - m_CurEdgeLevel[j];
			if(m_aNextDelta[j]*aLastDelta[j]<0)
			{
				if(m_aNextDelta[j]>0)
				{
					m_aRisingEdge[j] = i+j+m_Timer;
					m_CurEdgeLevel[j] = m_EdgeLevel[FALLING];
					m_aNextDelta[j] = buffer[i+j] - m_CurEdgeLevel[j];
				}
				else
				{
					if(m_aRisingEdge[j]!=-1ll)
					{
						if(j==m_ChopperPulseChannel)
						{
							idOfEvent = EV_IN_CHOPPER_CHAN;
							// �������� ������ ��� ���������� ������� �������
							if(m_PeriodsCountingData[CHOPPER_SIGNAL].partOfPeriod != 0)
							{
								m_PeriodsCountingData[CHOPPER_SIGNAL].periodsNumber++;									
								m_PeriodsCountingData[CHOPPER_SIGNAL].sumOfPeriods = 
									(m_aRisingEdge[j] + m_PeriodsCountingData[CHOPPER_SIGNAL].partOfPeriod);
								m_PeriodsCountingData[CHOPPER_SIGNAL].lastPulseTime = m_aRisingEdge[j];
								m_PeriodsCountingData[CHOPPER_SIGNAL].partOfPeriod = 0;
							}
							else
							{
								if(m_PeriodsCountingData[CHOPPER_SIGNAL].lastPulseTime == 0)
									m_PeriodsCountingData[CHOPPER_SIGNAL].lastPulseTime = m_aRisingEdge[j];
								else
								{
									m_PeriodsCountingData[CHOPPER_SIGNAL].periodsNumber++;									
									m_PeriodsCountingData[CHOPPER_SIGNAL].sumOfPeriods += 
										(m_aRisingEdge[j] - m_PeriodsCountingData[CHOPPER_SIGNAL].lastPulseTime);
									m_PeriodsCountingData[CHOPPER_SIGNAL].lastPulseTime = m_aRisingEdge[j];
								}
							}
						}
						if(j==m_HSMPulseChannel)
						{
							idOfEvent = EV_IN_HSM_CHAN;
							// �������� ������ ��� ���������� ������� �������
							if(m_PeriodsCountingData[HSM_SIGNAL].partOfPeriod != 0)
							{
								m_PeriodsCountingData[HSM_SIGNAL].periodsNumber++;									
								m_PeriodsCountingData[HSM_SIGNAL].sumOfPeriods = 
									(m_aRisingEdge[j] + m_PeriodsCountingData[HSM_SIGNAL].partOfPeriod);
								m_PeriodsCountingData[HSM_SIGNAL].lastPulseTime = m_aRisingEdge[j];
								m_PeriodsCountingData[HSM_SIGNAL].partOfPeriod = 0;
							}
							else
							{
								if(m_PeriodsCountingData[HSM_SIGNAL].lastPulseTime == 0)
									m_PeriodsCountingData[HSM_SIGNAL].lastPulseTime = m_aRisingEdge[j];
								else
								{
									m_PeriodsCountingData[HSM_SIGNAL].periodsNumber++;									
									m_PeriodsCountingData[HSM_SIGNAL].sumOfPeriods += 
										(m_aRisingEdge[j] - m_PeriodsCountingData[HSM_SIGNAL].lastPulseTime);
									m_PeriodsCountingData[HSM_SIGNAL].lastPulseTime = m_aRisingEdge[j];
								}
							}
						}
						if(j==m_MonitorPulseChannel)
						{
							idOfEvent = EV_IN_MONITOR_CHAN;
							m_MonitorCounter++;
						}
						if(j==m_DetectorPulseChannel)
						{
							idOfEvent = EV_IN_DETECTOR_CHAN;
							m_DetectorCounter++;
						}
						// ������� ������� ���� ������� � ��������� ���������
						if(m_DataFile.is_open() && m_IsSysInMeasConditions[requestNumber])
						{
							if(!WriteEvent(idOfEvent,m_aRisingEdge+j)) return 32;
						}
						m_aRisingEdge[j]=-1ll;
						m_CurEdgeLevel[j] = m_EdgeLevel[RISING];
						m_aNextDelta[j] = buffer[i+j] - m_CurEdgeLevel[j];
					}
				}
			}
			else
			{
				// if some tick exaclly equls m_CurEdgeLevel 
				if (m_aNextDelta[j]==0)
				{
					//rising edge
					if (aLastDelta[j]<0)
					{
						m_aRisingEdge[j] = i+j+m_Timer;
						m_CurEdgeLevel[j] = m_EdgeLevel[FALLING];
						m_aNextDelta[j] = buffer[i+j] - m_CurEdgeLevel[j];
					} 
					//falling edge
					else
					{
						if(m_aRisingEdge[j]!=-1ll)
						{
							if(j==m_ChopperPulseChannel)
							{
								idOfEvent = EV_IN_CHOPPER_CHAN;
								// �������� ������ ��� ���������� ������� �������
								if(m_PeriodsCountingData[CHOPPER_SIGNAL].partOfPeriod != 0)
								{
									m_PeriodsCountingData[CHOPPER_SIGNAL].periodsNumber++;									
									m_PeriodsCountingData[CHOPPER_SIGNAL].sumOfPeriods = 
										(m_aRisingEdge[j] + m_PeriodsCountingData[CHOPPER_SIGNAL].partOfPeriod);
									m_PeriodsCountingData[CHOPPER_SIGNAL].lastPulseTime = m_aRisingEdge[j];
									m_PeriodsCountingData[CHOPPER_SIGNAL].partOfPeriod = 0;
								}
								else
								{
									if(m_PeriodsCountingData[CHOPPER_SIGNAL].lastPulseTime == 0)
										m_PeriodsCountingData[CHOPPER_SIGNAL].lastPulseTime = m_aRisingEdge[j];
									else
									{
										m_PeriodsCountingData[CHOPPER_SIGNAL].periodsNumber++;									
										m_PeriodsCountingData[CHOPPER_SIGNAL].sumOfPeriods += 
											(m_aRisingEdge[j] - m_PeriodsCountingData[CHOPPER_SIGNAL].lastPulseTime);
										m_PeriodsCountingData[CHOPPER_SIGNAL].lastPulseTime = m_aRisingEdge[j];
									}
								}
							}
							if(j==m_HSMPulseChannel)
							{
								idOfEvent = EV_IN_HSM_CHAN;
								// �������� ������ ��� ���������� ������� �������
								if(m_PeriodsCountingData[HSM_SIGNAL].partOfPeriod != 0)
								{
									m_PeriodsCountingData[HSM_SIGNAL].periodsNumber++;									
									m_PeriodsCountingData[HSM_SIGNAL].sumOfPeriods = 
										(m_aRisingEdge[j] + m_PeriodsCountingData[HSM_SIGNAL].partOfPeriod);
									m_PeriodsCountingData[HSM_SIGNAL].lastPulseTime = m_aRisingEdge[j];
									m_PeriodsCountingData[HSM_SIGNAL].partOfPeriod = 0;
								}
								else
								{
									if(m_PeriodsCountingData[HSM_SIGNAL].lastPulseTime == 0)
										m_PeriodsCountingData[HSM_SIGNAL].lastPulseTime = m_aRisingEdge[j];
									else
									{
										m_PeriodsCountingData[HSM_SIGNAL].periodsNumber++;									
										m_PeriodsCountingData[HSM_SIGNAL].sumOfPeriods += 
											(m_aRisingEdge[j] - m_PeriodsCountingData[HSM_SIGNAL].lastPulseTime);
										m_PeriodsCountingData[HSM_SIGNAL].lastPulseTime = m_aRisingEdge[j];
									}
								}
							}
							if(j==m_MonitorPulseChannel)
							{
								idOfEvent = EV_IN_MONITOR_CHAN;
								m_MonitorCounter++;
							}
							if(j==m_DetectorPulseChannel)
							{
								idOfEvent = EV_IN_DETECTOR_CHAN;
								m_DetectorCounter++;
							}
							// ������� ������� ���� ������� � ��������� ���������
							if(m_DataFile.is_open() && m_IsSysInMeasConditions[requestNumber])
							{
								if(!WriteEvent(idOfEvent,m_aRisingEdge+j)) return 32;
							}
							m_aRisingEdge[j]=-1ll;
							m_CurEdgeLevel[j] = m_EdgeLevel[RISING];
							m_aNextDelta[j] = buffer[i+j] - m_CurEdgeLevel[j];
						}
					}
				}
			}
		}
	}
	m_Timer += m_DataStep;
	m_CountRateTimingData[0] += m_DataStep;
	// ����� ���������� ������� �������� ����� � ��������� � ������ ������ 10���
	if(m_CountRateTimingData[0] > m_CountRateTimingData[1])
	{
		// detector
		if(m_DetectorPulseChannel != -1)
			if(m_DetectorCounter != 0) 
			{
				CSpectrDlg::PrintInEdit(m_pSpectrDlg->GetDlgItem(IDC_EDIT7),
										m_DetectorCounter*m_AdcRate*1000./m_CountRateTimingData[0]);
				m_DetectorCounter = 0;
			}
			else m_pSpectrDlg->SetDlgItemText(IDC_EDIT7,_T("No signal!"));
		else m_pSpectrDlg->SetDlgItemText(IDC_EDIT7,_T("Not active!"));
		// monitor
		if(m_MonitorPulseChannel != -1)
			if(m_MonitorCounter != 0) 
			{
				CSpectrDlg::PrintInEdit(m_pSpectrDlg->GetDlgItem(IDC_EDIT12),
										m_MonitorCounter*m_AdcRate*1000./m_CountRateTimingData[0]);
				m_MonitorCounter = 0;
			}
			else m_pSpectrDlg->SetDlgItemText(IDC_EDIT12,_T("No signal!"));
		else m_pSpectrDlg->SetDlgItemText(IDC_EDIT12,_T("Not active!")); 
		m_CountRateTimingData[0] = 0ul;	
	}

	// �������� �������� ����� ���������, ���� ���� ��������� � �����, ������� ������ 
	// � ��� ���������� ������
	if ((!m_IsTimerReseted) && m_IsTurbineOpen[requestNumber] &&
		m_IsSysInMeasConditions[requestNumber] && !m_IsChamberFilling)
	{
		m_GoodCollectionTime += m_DataStep;
		if(m_GoodCollectionTime > m_AdcRate*1000*(CSpectrDlg::m_CurMeasTime+1)) CSpectrDlg::m_CurMeasTime++;
	}
	// ��������� ����� ���������� ������
	if(m_IsChamberFilling)
	{
		if(m_ChamberFillingCounter > m_ChamberFillingTimeInADCCounts) 
		{
			m_ChamberFillingCounter = 0;
			m_IsChamberFilling = false;
		}
		else m_ChamberFillingCounter += m_DataStep;
	}
	// ���� � ����� ��������� ������ ���������� ������
	// ������� ��� � �������� ������ �� ��� ���������
	if(m_Timer > m_LimitOfTimeInFileInADCCount && m_DataFile.is_open()) 
	{ 
		if (m_IsTurbineOpen[requestNumber])
			if(m_IsChamberFilling){if(!WriteEvent(EV_IN_TURBINE_CLOSE_CHAN, &m_Timer)) return 32;}
			else {if(!WriteEvent(EV_IN_TURBINE_OPEN_CHAN, &m_Timer)) return 32;}
		else
			{if(!WriteEvent(EV_IN_TURBINE_CLOSE_CHAN,&m_Timer)) return 32;}
		m_LimitOfTimeInFileInADCCount += long long(m_SecondsNumberInFile*m_AdcRate*1000);
		// ������� ����
		m_DataFile.close();
		// ������� ������ �� �������� ������ �����
		m_CreateNewFile = true;
		//�������� ������ �� ��������� ����� �����
		WORD err(0);
		if((err = ProcessFile(m_DataFileName)) != 0 && err != 46) return err;
		m_pSpectrDlg->SendMessageW(MyM_DRAW_GRAPH);
	}
	return WORD(0);
}


CString CDataAcqSystem::GetFileNameAsDirName(CString dirName)
{
	int N , i(1);
	N = dirName.GetLength();
	while (dirName.GetAt(N-i)!='\\') i++;
	return dirName.Right(i);
}

bool CDataAcqSystem::WriteEvent(BYTE id, long long *pValue)
{
	// ����� � ����
	if(!m_DataFile.write(reinterpret_cast<const char*>(&id),sizeof(BYTE)))
		return false;
	if(!m_DataFile.write(reinterpret_cast<const char*>(pValue),5)) 
		return false;
	return true;
}
// ������� ��������� ������ ������
WORD CDataAcqSystem::ProcessFile(CString inRawDataFileName)
{
	ifstream inClientFile;
	inClientFile.open(inRawDataFileName,ios::in | ios::binary);
	if(!inClientFile) return 43;
	
	BYTE eventID(0);
	long long time[2] = {NULL};
	// ������� ������� � ����� �������
	DWORD turbineOpenTime[2] = {NULL};
	WORD turbineOpenCounter(0);
	DWORD turbineCloseTime[2] = {NULL};
	WORD turbineCloseCounter(0);
	// c ������ ������������ �������� ��������� 300�� 
	// � �������� ������� � ���� 20� ������� ������ �� 7000 �� 
	DWORD detectorPulseTime[7000] = {NULL};
	WORD detectorPulseCounter(0);
	// ������� ��������� � ������, ����� ������� �������
	DWORD detectorPulseInTurbOpenCounter(0);
	// ������� ���� �������� �������� 10 ��� ������
	DWORD monitorPulseTime[70000] = {NULL};
	WORD monitorPulseCounter(0);
	// ����� ������� �������� � ����������� 
	// ���� c ������� ������� 150��
	DWORD modulatorPulseTime[3000] = {NULL};
	WORD modulatorPulseCounter(0);
	// ����� ������� �������� � ������� 
	DWORD hsmMotorPulseTime[4000] = {NULL};
	WORD hsmMotorPulseCounter(0);
	// ��������� �������
	WORD estCarriagePos(0);
	double measCarriagePos(0.);
	// ������ ��� ���������� ������������� ������� ������� ��
	//������� ��������� � ����������� �������
	const WORD oscilChanelNumber(90);
	DWORD oscilDestrib[oscilChanelNumber] = {NULL};
	// ����� ��������� ����� ����� �������� ���������� ������������
	// ������� ������ ������� ������� ����� ������� �������
	// � �������� ��������������� �����
	DWORD measModTimeInFileInADCTicks(0);


	// ����� ��������� � ����� � �����
	DWORD measTimeInFileInADCTicks(0);
	//DWORD adcRate(5000), dataStep(1024*1024);
	WORD error(0);
	// ������ ������ ������ ���� ��������� �������
	if(inClientFile>>eventID && 
		inClientFile.read(reinterpret_cast<char *>(&time[0]),5) &&
		!inClientFile.eof())
	{
		switch(eventID)
		{
			case EV_IN_TURBINE_OPEN_CHAN:
				 turbineOpenTime[turbineOpenCounter++] = 0;
				 break;
			case EV_IN_TURBINE_CLOSE_CHAN:
				turbineCloseTime[turbineCloseCounter++] = 0;
				break;
			default:
				// ������� ����
				inClientFile.close();
				return 44;
		}
	}
	else 
	{
		// ������� ����
		inClientFile.close();
		return 45;
	}
	// ��� ������ ������ ������ ���� ��������� ������� ������������
	
	if(inClientFile>>eventID && 
		inClientFile.read(reinterpret_cast<char *>(&time[1]),5) &&
		!inClientFile.eof())
	{
		if(eventID == EV_IN_CARRIAGE_EST_POS_CHAN) estCarriagePos = time[1];
		else 
		{
			// ������� ����
			inClientFile.close();
			return 44;
		}
	}
	// � ���������� ��������
	if(inClientFile>>eventID && !inClientFile.eof())
	{
		if(eventID == EV_IN_CARRIAGE_MEAS_POS_CHAN) 
		{
			if(!inClientFile.read(reinterpret_cast<char *>(&measCarriagePos),5) &&
				inClientFile.eof())
			{
				// ������� ����
				inClientFile.close();
				return 44;
			}
		}
		else 
		{			
			// ������� ����
			inClientFile.close();
			return 44;
		}
	}
	// ������ ��������� � ��������� ���� ������� ���
	// � ������������ ��������
	while(inClientFile>>eventID &&
		inClientFile.read(reinterpret_cast<char *>(&time[1]),5) &&
		!inClientFile.eof())
	{
		switch(eventID)
		{
			case EV_IN_CHOPPER_CHAN:
				modulatorPulseTime[modulatorPulseCounter++] = DWORD(time[1]-time[0]);
				 break;
			case EV_IN_HSM_CHAN:
				hsmMotorPulseTime[hsmMotorPulseCounter++] = DWORD(time[1]-time[0]);
				 break;
			case EV_IN_MONITOR_CHAN:
				monitorPulseTime[monitorPulseCounter++] = DWORD(time[1]-time[0]);
				 break;
			case EV_IN_DETECTOR_CHAN: 
				detectorPulseTime[detectorPulseCounter++] = DWORD(time[1]-time[0]);
				break;
			case EV_IN_TURBINE_OPEN_CHAN:
				turbineOpenTime[turbineOpenCounter++] = DWORD(time[1]-time[0]);
				 break;
			case EV_IN_TURBINE_CLOSE_CHAN:
				turbineCloseTime[turbineCloseCounter++] = DWORD(time[1]-time[0]);
				 break;
			default:				
				// ������� ����
				inClientFile.close();
				return 44;
		}
	}
	// ������� ����, ���� �� ��� ������ ��� ���������� ����� ������
	if(inClientFile.is_open()) inClientFile.close();
	// �������� ���� �� �������� � ������ ���������, ���� ��� �������
	if(detectorPulseCounter == 0)
	{
		// ������� ����
		inClientFile.close();
		return 46;
	}
	// �������� ���� �� �������� � ������� ����������� � �������
	WORD modFrequency(0),hsmFrequency(0);
	if(hsmMotorPulseCounter !=0)
	{
		DWORD sum(0);
		for(WORD i(1);i<hsmMotorPulseCounter;i++)
			sum += (hsmMotorPulseTime[i]-hsmMotorPulseTime[i-1]);
		hsmFrequency = WORD((m_AdcRate*10000*hsmMotorPulseCounter/sum+5)/10);
	}
	// ����� �������� ��������� ������ ����������� �� �����
	// �������� ���� �� ��������� ����� ������� � ���
	switch (turbineOpenCounter)
	{ 
	// ���� �������� ������, ���������� ����� ������� ���� �� � ���
	// ��� ������ ��� �� �����
	case 0:
		// ������� ����
		inClientFile.close();
		return 0;
	case 1:		
		if(turbineCloseCounter != 2)
		{
			// ������� ����
			inClientFile.close();
			return 44;
		}
		// ��� ������ ������� ���� �����-�� ����� � ������ �����, � ����� ����
		else 
		{
			// ������ ������ � ��������� ������� �� ����������� ������� � ������ �������� �������
			// � ������ �������� ������� ���������
			int firstModPulseInOpenTurb(-1), lastModPulseInOpenTurb(-1);
			if(modulatorPulseCounter !=0)
			{
				
				DWORD sum(0);
				for(WORD i(1);i<modulatorPulseCounter;i++)
				{
					sum += (modulatorPulseTime[i]-modulatorPulseTime[i-1]);
					// ������ ������ �������
					if(turbineOpenTime[0]<modulatorPulseTime[i-1] && firstModPulseInOpenTurb == -1)
						firstModPulseInOpenTurb = i-1; 
					// ������ ��������� �������
					if(turbineCloseTime[0]-m_DataStep > modulatorPulseTime[modulatorPulseCounter-i] && 
					   lastModPulseInOpenTurb == -1)
						lastModPulseInOpenTurb = modulatorPulseCounter-i;
				}
				modFrequency = WORD((m_AdcRate*10000.*modulatorPulseCounter/sum+5.)/10.);
				measModTimeInFileInADCTicks = modulatorPulseTime[lastModPulseInOpenTurb] -
											  modulatorPulseTime[firstModPulseInOpenTurb];
			}

			for(WORD i(0);i < detectorPulseCounter;i++)
			{
				if(turbineOpenTime[0] <= detectorPulseTime[i] && 
					detectorPulseTime[i] <= turbineCloseTime[0]-m_DataStep)
				{
					detectorPulseInTurbOpenCounter++;
					if(firstModPulseInOpenTurb != lastModPulseInOpenTurb)
					{
						if(detectorPulseTime[i] > modulatorPulseTime[firstModPulseInOpenTurb])
						{
							while(detectorPulseTime[i] > modulatorPulseTime[firstModPulseInOpenTurb+1] &&
								  firstModPulseInOpenTurb+1 != lastModPulseInOpenTurb)
								firstModPulseInOpenTurb++;
							if(detectorPulseTime[i] < modulatorPulseTime[lastModPulseInOpenTurb])
								// �������� �������������
								PutEventInDestrib(detectorPulseTime[i],
													modulatorPulseTime[firstModPulseInOpenTurb],
													modulatorPulseTime[firstModPulseInOpenTurb+1],
													oscilChanelNumber,
													oscilDestrib);
						}
					}
				}
			}
			measTimeInFileInADCTicks = turbineCloseTime[0]-m_DataStep;
		}
		break;
	case 2:
		// ������ ������ � ��������� ������� �� ����������� ������� � ������ �������� �������
		// � ������ �������� ������ ���������
		int firstModPulseInOpenTurb(-1), lastModPulseInOpenTurb(-1);
		if(modulatorPulseCounter !=0)
		{
				
			DWORD sum(0);
			for(WORD i(1);i<modulatorPulseCounter;i++)
			{
				sum += (modulatorPulseTime[i]-modulatorPulseTime[i-1]);
				// ������ ������ �������
				if(turbineOpenTime[0]<modulatorPulseTime[i-1] && firstModPulseInOpenTurb == -1)
					firstModPulseInOpenTurb = i-1; 
				// ������ ��������� �������
				if(turbineOpenTime[1] > modulatorPulseTime[modulatorPulseCounter-i] && lastModPulseInOpenTurb == -1)
					lastModPulseInOpenTurb = modulatorPulseCounter-i;
			}
			modFrequency = WORD((m_AdcRate*10000.*modulatorPulseCounter/sum+5.)/10.);
			measModTimeInFileInADCTicks = modulatorPulseTime[lastModPulseInOpenTurb] -
											  modulatorPulseTime[firstModPulseInOpenTurb];
		}
		// �������� ���������� ��������� ����� ������� �������
		for(WORD i(0);i < detectorPulseCounter;i++)
		{
			if(turbineOpenTime[0] <= detectorPulseTime[i] &&
				detectorPulseTime[i] <= turbineOpenTime[1])
			{
				detectorPulseInTurbOpenCounter++;
				if(firstModPulseInOpenTurb != lastModPulseInOpenTurb)
				{
					if(detectorPulseTime[i] > modulatorPulseTime[firstModPulseInOpenTurb])
					{
						while(detectorPulseTime[i] > modulatorPulseTime[firstModPulseInOpenTurb+1] &&
							  firstModPulseInOpenTurb+1 != lastModPulseInOpenTurb)
							firstModPulseInOpenTurb++;
						if(detectorPulseTime[i] < modulatorPulseTime[lastModPulseInOpenTurb])
							// �������� �������������
							PutEventInDestrib(detectorPulseTime[i],
												modulatorPulseTime[firstModPulseInOpenTurb],
												modulatorPulseTime[firstModPulseInOpenTurb+1],
												oscilChanelNumber,
												oscilDestrib);
					}
				}
			}

		}
		measTimeInFileInADCTicks = turbineOpenTime[1]-turbineOpenTime[0];
		break;
	}
	// ������������ ������������ ��� ������ ������������� �����
	WORD pointCounter(0);
	WORD point[200] = {NULL};
	// ���������� ������� � �����
	DWORD eventNumber[200]= {NULL};
	// ����� ������ � ����� � ��������
	WORD collectionTimeInSec[200] = {NULL},inThisFileCollTimeInSec( measTimeInFileInADCTicks/m_AdcRate/1000 );
	// ������� �� ������� ������ � ����� ���
	DWORD balanceOfCollTimeInADCTicks[200] = {NULL},inThisFileBalanceOfCollTimeInADCTicks( measTimeInFileInADCTicks - inThisFileCollTimeInSec*m_AdcRate*1000 );
	// ��������� �������� ������������� �����
	CString processedFileName[2] = {NULL};
	GetProcessedFileName(modFrequency,hsmFrequency,estCarriagePos,processedFileName);
	// ������� ������ �� ����� ���������� ����� ��� ���������
	if(!processedFileName[OSCIL_FILENAME].IsEmpty())
	{	
		WORD chan(0);
		DWORD val(0);
		inClientFile.open(processedFileName[OSCIL_FILENAME],ios::in || ios::out);
		if(inClientFile)
		{
			
			while(inClientFile>>chan>>val && !inClientFile.eof())
			{
				oscilDestrib[chan]+=val;
				if(chan == oscilChanelNumber-1)
				{
					inClientFile>>chan>>val;
					break;
				}
			}
		}
		if(inClientFile.is_open()) inClientFile.close();
		// ������� ������ ������������� � ����
		ofstream outClientFile(processedFileName[OSCIL_FILENAME]);
		for(WORD i(0);i<oscilChanelNumber;i++)
			outClientFile<<i<<" "<<oscilDestrib[i]<<"\n";
		WORD measTimeInSec = measModTimeInFileInADCTicks/m_AdcRate/1000;
		DWORD balanceOfMeasTimeInTicks(measModTimeInFileInADCTicks-measTimeInSec*m_AdcRate*1000);
		measTimeInSec += chan;
		balanceOfMeasTimeInTicks += val;
		if(balanceOfMeasTimeInTicks>m_AdcRate*1000)
		{
			measTimeInSec++;
			balanceOfMeasTimeInTicks -= m_AdcRate*1000;
		}
		outClientFile<<measTimeInSec<<" "<<balanceOfMeasTimeInTicks<<"\n";
		outClientFile.close();
		// ������� ���� ��� ��������� �������
		WriteGraphFile(modFrequency,oscilChanelNumber,oscilDestrib,measTimeInSec);
	} 
	// ������� ������ �� ����� ���������� ����� ��� �����
	inClientFile.open(processedFileName[SCAN_FILENAME],ios::in);
	if(inClientFile)
	{
		while(inClientFile>>point[pointCounter] && !inClientFile.eof())
		{
			inClientFile>>eventNumber[pointCounter]
			>>collectionTimeInSec[pointCounter]
			>>balanceOfCollTimeInADCTicks[pointCounter];
			pointCounter++;
		}
	}
	// ������� ����
	inClientFile.close();
	//������� ����
	ofstream outClientFile(processedFileName[SCAN_FILENAME],ios::out);
	if(!outClientFile) return 47;
	for(WORD n(0);n<pointCounter;n++)
	{
		if(estCarriagePos < point[n])
		{
			outClientFile<<estCarriagePos<<" "<<detectorPulseInTurbOpenCounter
					<<" "<<inThisFileCollTimeInSec<<" "<<inThisFileBalanceOfCollTimeInADCTicks<<"\n";
			// ������� ���������� ������ � ����� � ���������
			for(WORD i(n);i<pointCounter;i++)
			{
				outClientFile<<point[i]<<" "<<eventNumber[i]
							<<" "<<collectionTimeInSec[i]<<" "<<balanceOfCollTimeInADCTicks[i]<<"\n";
				 // ����������� ������� � ������ ������ �����
				 point[i] = estCarriagePos;
				 eventNumber[i] = detectorPulseInTurbOpenCounter;
				 collectionTimeInSec[i] = inThisFileCollTimeInSec;
				 if(i == pointCounter-1) 
				 {
					point[i+1] = estCarriagePos;
					eventNumber[i+1] = detectorPulseInTurbOpenCounter;
					collectionTimeInSec[i+1] = inThisFileCollTimeInSec;
					pointCounter++;
					break;
				 }
				 else
				 {
					estCarriagePos = point[i+1];
					detectorPulseInTurbOpenCounter = eventNumber[i+1];
					inThisFileCollTimeInSec = collectionTimeInSec[i+1];
				 }
			}
			//������� ���� ��� �������
			if(!WriteGraphFile(pointCounter,point,eventNumber,collectionTimeInSec))return 48;
			// ������� ����
			outClientFile.close();
			return WORD(0);
		}
		else
		{
			if(estCarriagePos == point[n])
			{
				eventNumber[n] += detectorPulseInTurbOpenCounter;
				DWORD sum(balanceOfCollTimeInADCTicks[n]+inThisFileBalanceOfCollTimeInADCTicks);
				if(sum > m_AdcRate*1000)
				{
					balanceOfCollTimeInADCTicks[n] = sum - m_AdcRate*1000;
					collectionTimeInSec[n] += (++inThisFileCollTimeInSec);
				}
				else
				{
					collectionTimeInSec[n] +=  inThisFileCollTimeInSec;
					balanceOfCollTimeInADCTicks[n] = sum;
				}
				outClientFile<<point[n]<<" "<<eventNumber[n]
				<<" "<<collectionTimeInSec[n]<<" "<<balanceOfCollTimeInADCTicks[n]<<"\n";
				// ������� ���������� ������ � ����� � ���������
				for(WORD i(++n);i<pointCounter;i++)
				{
					outClientFile<<point[i]<<" "<<eventNumber[i]
								<<" "<<collectionTimeInSec[i]<<" "<<balanceOfCollTimeInADCTicks[i]<<"\n";
				}
				//������� ���� ��� �������
				if(!WriteGraphFile(pointCounter,point,eventNumber,collectionTimeInSec))return 48;
				// ������� ����
				outClientFile.close();
				return WORD(0);
			}
			else
			{
				outClientFile<<point[n]<<" "<<eventNumber[n]
							<<" "<<collectionTimeInSec[n]<<" "<<balanceOfCollTimeInADCTicks[n]<<"\n";
			}
		}
	}
	outClientFile<<estCarriagePos<<" "<<detectorPulseInTurbOpenCounter
				<<" "<<inThisFileCollTimeInSec<<" "<<inThisFileBalanceOfCollTimeInADCTicks;
	// ����������� ������� � ������ ������ �����
	point[pointCounter] = estCarriagePos;
	eventNumber[pointCounter] = detectorPulseInTurbOpenCounter;
	collectionTimeInSec[pointCounter] = inThisFileCollTimeInSec;
	pointCounter++;
	//������� ���� ��� �������
	if(!WriteGraphFile(pointCounter,point,eventNumber,collectionTimeInSec))return 48;
	outClientFile.close();
	return WORD(0);
}


void CDataAcqSystem::GetProcessedFileName(WORD modFr, WORD hsmFr, WORD carriagePos, CString *processedFileName)
{
	// ��������� �������� ����� ��� �����
	if(hsmFr == 0)
		processedFileName[SCAN_FILENAME] = m_ConstPartOfDataFile+_T("_Scan.txt");
	else
		processedFileName[SCAN_FILENAME].Format(m_ConstPartOfDataFile +_T("_Scan_hsm%i.txt"),hsmFr);
	// ��������� �������� ����� ��� ���������
	if(modFr != 0)
	{
		if(hsmFr == 0)processedFileName[OSCIL_FILENAME].Format(m_ConstPartOfDataFile +_T("_Mod%i_pnt%i.txt"),modFr,carriagePos);
		else processedFileName[OSCIL_FILENAME].Format(m_ConstPartOfDataFile +_T("_Mod%i_pnt%i_hsm%i.txt"),modFr,carriagePos,hsmFr);
	}
	return ;
}


// ������� ������ �����, ������� ������������ � �������
bool CDataAcqSystem::WriteGraphFile(WORD pointNumber, WORD * point, DWORD * detPulseNumber, WORD * timeInPointInSec)
{
	ofstream outClientFile(m_ExacutableFileDir+_T("\\ScanData.txt"),ios::out);
	if(!outClientFile) return false;
	for(WORD i(0);i<pointNumber;i++)
	{
		outClientFile<<point[i]<<" "<<detPulseNumber[i]/float(timeInPointInSec[i])
			<<" "<<sqrt(detPulseNumber[i]*1.)/float(timeInPointInSec[i])<<"\n";
;
	}
	outClientFile.close();
	return true;
}

bool CDataAcqSystem::WriteGraphFile(WORD frequency,WORD cannelNumber, DWORD * destribution, WORD measTime)
{
	ofstream outClientFile(m_ExacutableFileDir+_T("\\OscilData.txt"),ios::out);
	if(!outClientFile) return false;
	long long sum(0ll);
	for(WORD i(0);i<cannelNumber;i++) sum += destribution[i];
	for(WORD i(0);i<cannelNumber;i++)
		outClientFile<<i<<" "<<destribution[i]*1./sum<<" "<<sqrt(destribution[i]*1.)/sum<<"\n";
	outClientFile.close();
	return true;
}


void CDataAcqSystem::PutEventInDestrib(DWORD value, DWORD valueIn, DWORD valueFin, const WORD channelNumber, DWORD * destribArray)
{
	WORD step = (valueFin-valueIn)/channelNumber;
	for(WORD i(0);i<channelNumber;i++)
	{
		if((valueIn+i*step) < value && 
			value < (valueIn+(i+1)*step)) 
		{
			destribArray[i]++;
			return;
		}
	}
}


// ������� ��������� �������� �� ������
CString CDataAcqSystem::GetSignalFrequency(WORD channel, double & frequency)
{
	switch(channel)
	{
	case E2010_CHOPPER_CHANNEL:
		channel = CHOPPER_SIGNAL;
		if(m_ChopperPulseChannel==-1) return _T("Not active!");
		break;
	case E2010_HSM_CHANNEL:
		channel = HSM_SIGNAL;
		if(m_HSMPulseChannel==-1) return _T("Not active!");
		break;
	}
	if(m_PeriodsCountingData[channel].periodsNumber != 0)
		frequency = m_AdcRate*1000.*m_PeriodsCountingData[channel].periodsNumber/
			m_PeriodsCountingData[channel].sumOfPeriods;
	else 
	{
		if(m_PeriodsCountingData[channel].lastPulseTime == 0 && 
			m_PeriodsCountingData[channel].partOfPeriod == 0) return _T("No signal!");
		else return _T("...");
	}
	return _T("");
}


inline void CDataAcqSystem::DoWhenEdgeHasBeenFound(void)
{

}
