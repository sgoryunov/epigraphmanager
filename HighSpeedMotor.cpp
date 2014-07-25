#include "StdAfx.h"
#include "HighSpeedMotor.h"
#include "SpectrDlg.h"
#include <math.h>

WORD CHighSpeedMotor::m_FreqMeasNumber(0);
WORD CHighSpeedMotor::m_ReactionTime(0);
double CHighSpeedMotor::m_OperatingFrequency(0.);
WORD CHighSpeedMotor::m_Error(0);
WORD CHighSpeedMotor::m_FrequencyCounter(0);
double CHighSpeedMotor::m_AvrMeasFrequency(0.);
// ������� ������ ���������� �������
bool CHighSpeedMotor::m_IsAdjustingFreq(false);
short CHighSpeedMotor::m_DacStep(50);
short CHighSpeedMotor::m_CurDacData(0);
WORD CHighSpeedMotor::m_Channel(0);
double CHighSpeedMotor::m_FrequencyRange(0.1);
CString CHighSpeedMotor::m_FreqDacTableFileName(_T(""));

CHighSpeedMotor::CHighSpeedMotor(void)
{
	m_FrequencyRange = double(GetPrivateProfileInt(_T("HSM"),_T("Frequency range"),NULL,CSpectrDlg::m_IniFileName));
	m_FrequencyRange *= 0.1;
	m_Channel = GetPrivateProfileInt(_T("E2010"),_T("HSM control channel"),NULL,CSpectrDlg::m_IniFileName);
	m_Channel -- ;
	// ������� ����� �������� �������
	m_ReactionTime = GetPrivateProfileInt(_T("HSM"),_T("Reaction time"),NULL,CSpectrDlg::m_IniFileName);
	// ��������� ��� �����
	m_FreqDacTableFileName.Format(CSpectrDlg::GetExecutFileDir()+_T("//HSM_Table.tbl"));
	// ������� ���������� �� ��� ������ ���� ���� ������ ������ ����,
	//� ����� ����������� ������������� �������
	m_FreqMeasNumber = 0;
	m_OperatingFrequency = 0.;
	m_Error = 0;
	m_FrequencyCounter = 0;
	m_AvrMeasFrequency = 0.;
	// ������� ������ ���������� �������
	m_IsAdjustingFreq = false;
	m_DacStep = 1;
	m_CurDacData = 0;
}


CHighSpeedMotor::~CHighSpeedMotor(void)
{
	if(m_InClientFile.is_open())
		m_InClientFile.close();
}


// �-� ��������� ������� �������� �������
int CHighSpeedMotor::SetFrequency(short frequency)
{
	if(m_OperatingFrequency != double(frequency) && m_Error == 0);
	{
		// ��������� ������� �������
		m_OperatingFrequency = double(frequency);
		m_IsAdjustingFreq = true;
		// ������ � ������� �������� DAC ��� ���� �������
		if(frequency!=0)SetDacDataForFreq(frequency);
		else
		{
			m_CurDacData = CSpectrDlg::m_MinE2010DacValue;
			if(!CSpectrDlg::m_pE2010Module->DAC_SAMPLE(&m_CurDacData,m_Channel)) return 42;
		}
		while(m_IsAdjustingFreq && m_Error == 0)
		{
			if(CSpectrDlg::m_PressBn == STOP) return -1;
			Sleep(40);
		}
	}
	return m_Error;
}

int CHighSpeedMotor::SendMeasFrequency(double frequency)
{
	// ������� ���������� ��������� �� �������� �������
	if(m_FreqMeasNumber == 0)
		m_FreqMeasNumber = WORD(m_ReactionTime*m_AdcRate*1000/m_DataStep);
	// �������� ������ ������
	if(m_FrequencyCounter<m_FreqMeasNumber)
	{
		m_AvrMeasFrequency += frequency;
		m_FrequencyCounter++;
	}
	else 
	{
		// �������� ������� �������
		//for(WORD i(0);i<m_FreqMeasNumber;i++) avrFreq += m_pMeasFrequency[i];
		m_AvrMeasFrequency /= m_FreqMeasNumber;
		// ������ ������� �� ���������� ������� 
		if(abs(m_OperatingFrequency - m_AvrMeasFrequency) > m_FrequencyRange && m_OperatingFrequency != 0)
		{
			// �������� ������ ����������� �������
			m_IsAdjustingFreq = true;
			if(m_AvrMeasFrequency < m_OperatingFrequency)
			{
				if(m_CurDacData<(CSpectrDlg::m_MaxE2010DacValue - m_DacStep) && CSpectrDlg::m_PressBn != PAUSE)
				{
					m_CurDacData += m_DacStep;
					if(!CSpectrDlg::m_pE2010Module->DAC_SAMPLE(&m_CurDacData,m_Channel)) return 42;
				}
			}
			else
			{
				if(m_CurDacData>(CSpectrDlg::m_MinE2010DacValue + m_DacStep) && CSpectrDlg::m_PressBn != PAUSE)
				{
					m_CurDacData -= m_DacStep;
					if(!CSpectrDlg::m_pE2010Module->DAC_SAMPLE(&m_CurDacData,m_Channel)) return 42;
				}
			}
		}
		else 
		{
			if(m_IsAdjustingFreq) 
			{
				if(m_OperatingFrequency != 0)
				{
					// ������� �������� DAC � �������
					if(!WriteFreqDacTable(m_OperatingFrequency,m_CurDacData)) return 50;
					m_IsAdjustingFreq = false;
				}
				else
					// start measurin when the grating is stoped
					if(m_AvrMeasFrequency < m_FrequencyRange) m_IsAdjustingFreq = false;

			}
		}
		// ������� ������� ������ �� 1 � ������� ������
		m_FrequencyCounter = 1;
		m_AvrMeasFrequency = frequency;
		//ZeroMemory(m_pMeasFrequency,m_FreqMeasNumber*sizeof(double));
		
	}
	return NO_ERRORS;
}





void CHighSpeedMotor::SetDacDataForFreq(short freq)
{
	m_InClientFile.open(m_FreqDacTableFileName);
	if(!m_InClientFile) return;
	short x(0),y(0);
	while(m_InClientFile>>x>>y)
		if(x == freq)
		{
			m_CurDacData = y;
			break;
		}
	m_InClientFile.close();
	if(!CSpectrDlg::m_pE2010Module->DAC_SAMPLE(&m_CurDacData,m_Channel)) m_Error = 42;

	return;
}



// ������ ������ ������ � �������
bool CHighSpeedMotor::WriteFreqDacTable(double in_freq, short dacData)
{
	WORD freq = WORD(in_freq),n(0);
	WORD x[120]={NULL};
	short y[120]={NULL};
	fstream inOutClientFile(m_FreqDacTableFileName,ios::in); 
	if (inOutClientFile)
	{
		// ������ ����� ������� ��� ������
		while(inOutClientFile>>x[n]>>y[n])
		{
			if(freq <= x[n])
			{
				if(x[n] == freq)
				{
					y[n++] = dacData;
					while(inOutClientFile>>x[n]>>y[n])n++;
					break;
				}
				else 
				{
					x[n+1] = x[n];
					y[n+1] = y[n];
					x[n] = freq;
					y[n] = dacData;
					n+=2;
					while(inOutClientFile>>x[n]>>y[n])n++;
					break;
				}
			}
			n++;
		}
		if(freq > x[n-1] && n!=0)
		{
			x[n] = freq;
			y[n++] = dacData;
		}
		inOutClientFile.close();
	}
	//������� ����
	inOutClientFile.open(m_FreqDacTableFileName,ios::out); 
	if (!inOutClientFile)return false;
	if(n == 0) inOutClientFile<<freq<<" "<<dacData<<"\n";
	else
	{
		for(WORD i(0);i<n;i++) 
			inOutClientFile<<x[i]<<" "<<y[i]<<"\n";
	}
	inOutClientFile.close();
	return true;
}
