#include "StdAfx.h"
#include "SampPosSystem.h"
#include "SpectrDlg.h"
#include <math.h>


short CSampPosSystem::m_EstCarriagePosition = 0;
short CSampPosSystem::m_MeasCarriagePosition = 0;

CSampPosSystem::CSampPosSystem(void)
{
}
CSampPosSystem::CSampPosSystem(CWnd *pEdit1 = NULL, CWnd *pEdit2 = NULL)
	:m_pEditForEstPos(pEdit1), m_pEditForMeasPos(pEdit2)
{
	
	m_IsPulseSignalSys = GetPrivateProfileInt(_T("SPS"),_T("Signal system"),NULL,CSpectrDlg::m_IniFileName);	
	m_PulseOrUpPulseTTLoutPin = GetPrivateProfileInt(_T("E2010"),_T("SPS stepper signal (out)pin"),NULL,CSpectrDlg::m_IniFileName);
	m_DirOrDownPulseTTLoutPin = GetPrivateProfileInt(_T("E2010"),_T("SPS stepper diraction (out)pin"),NULL,CSpectrDlg::m_IniFileName);
	
	
	m_NumberOfDevisions = GetPrivateProfileInt(_T("SPS"),_T("Number of devisions"),NULL,CSpectrDlg::m_IniFileName);
	m_TimeDelay = GetPrivateProfileInt(_T("SPS"),_T("Time delay"),NULL,CSpectrDlg::m_IniFileName);
	m_Calibration = GetPrivateProfileInt(_T("SPS"),_T("Calibration"),NULL,CSpectrDlg::m_IniFileName);
	m_MinDistanceBwFilters = GetPrivateProfileInt(_T("SPS"),_T("Min distance bw filters"),NULL,CSpectrDlg::m_IniFileName);
	m_CarriageWorkRange = GetPrivateProfileInt(_T("SPS"),_T("Carriage working range"),NULL,CSpectrDlg::m_IniFileName);
	m_EstCarriagePosition = GetPrivateProfileInt(_T("SPS"),_T("Current carriage position"),NULL,CSpectrDlg::m_IniFileName);
	m_LuftInMillimetre = 4;
	m_StepsInMillimetre = WORD(200*m_NumberOfDevisions/m_Calibration);
	m_CurCarriagePosInSteps = m_EstCarriagePosition*m_StepsInMillimetre;
	m_MeasCarriagePosition = 0;
	CSpectrDlg::PrintInEdit(m_pEditForEstPos,m_EstCarriagePosition);
}


CSampPosSystem::~CSampPosSystem(void)
{
}


// функция установки каретки в нужной точке
 bool CSampPosSystem::SetPosition(WORD position)
{
	if(position != m_EstCarriagePosition)
	{
		int stepNumber(0);
		WORD ttlOut(0), flipper(0x1), pulse(0);
		stepNumber = position*m_StepsInMillimetre - m_CurCarriagePosInSteps;
		// go up, but distanse bw filters decreasing
		if(stepNumber<0)
		{
			WORD pulseHighLevel(0), valForDiraction(0);
			if(m_IsPulseSignalSys) pulseHighLevel = pow(2.0,m_PulseOrUpPulseTTLoutPin-1);
			else
			{
				pulseHighLevel = pow(2.0,m_PulseOrUpPulseTTLoutPin-1);
				valForDiraction = pow(2.0,m_DirOrDownPulseTTLoutPin-1);
			}
			for(int stp(0);stp<abs(2*stepNumber);stp++)
			{
				
				ttlOut=pulseHighLevel*flipper+valForDiraction;
				// проверим ошибки при выводе числа на ТТЛ, в случае ошибки шаг не считаем
				if(!(CSpectrDlg::m_pE2010Module->TTL_OUT(ttlOut))) stp-- ;
				else 
				{
					Sleep(m_TimeDelay); 
					flipper^=0x1;
					// вычисляем оценочное положение каретк в милиметрах
					if(ttlOut==(pulseHighLevel+valForDiraction)) pulse++;
					if (pulse==m_StepsInMillimetre)
					{
						m_CurCarriagePosInSteps-=m_StepsInMillimetre;
						// выведем расчетное положение каретки
						if(CSpectrDlg::m_PressBn !=STOP)
							CSpectrDlg::PrintInEdit(m_pEditForEstPos,--m_EstCarriagePosition);
						else 
							m_EstCarriagePosition--;
						CString in;
						in.Format(_T("%i"),m_EstCarriagePosition);
						WritePrivateProfileString(_T("SPS"),_T("Current carriage position"),in,CSpectrDlg::m_IniFileName);
						pulse = 0;
						// если нажата стоп выходим из функции
						if(CSpectrDlg::m_PressBn == STOP) 
						{
							CSpectrDlg::m_pE2010Module->TTL_OUT(NULL); 
							return false;
						}
						
					}
					// если нажата кнопка пауза ждем
					while(CSpectrDlg::m_PressBn == PAUSE) Sleep(10);
					// если нажата стоп выходим из функции
					if(CSpectrDlg::m_PressBn == STOP && pulse == 79) 
						int t=0;
				}
			}
		}
		// go down
		else
		{
			WORD pulseHighLevel(0);
			if(m_IsPulseSignalSys) pulseHighLevel = pow(2.0,m_DirOrDownPulseTTLoutPin-1);
			else pulseHighLevel = pow(2.0,m_PulseOrUpPulseTTLoutPin-1);
			for(int stp(0);stp<2*abs(stepNumber);stp++)
			{
				
				ttlOut = pulseHighLevel*flipper;
				// проверим ошибки при выводе числа на ТТЛ, в случае ошибки шаг не считаем
				if(!(CSpectrDlg::m_pE2010Module->TTL_OUT(ttlOut))) stp-- ;
				else 
				{ 
					Sleep(m_TimeDelay); 
					flipper^=0x1;
					// вычисляем оценочное положение каретк в милиметрах
					if(ttlOut==pulseHighLevel) pulse++;
					if (pulse>=m_StepsInMillimetre)
					{
						m_CurCarriagePosInSteps+=m_StepsInMillimetre;
						// выведем расчетное положение каретки
						if(CSpectrDlg::m_PressBn !=STOP)
							CSpectrDlg::PrintInEdit(m_pEditForEstPos,++m_EstCarriagePosition);
						else 
							m_EstCarriagePosition++;
						// write the current position
						CString in;
						in.Format(_T("%i"),m_EstCarriagePosition);
						WritePrivateProfileString(_T("SPS"),_T("Current carriage position"),in,CSpectrDlg::m_IniFileName);
						pulse = 0;
						// если нажата стоп выходим из функции
						if(CSpectrDlg::m_PressBn == STOP) 
						{
							KillThelLuft();
							CSpectrDlg::m_pE2010Module->TTL_OUT(NULL); 
							return false;
						}
					}	
				}
				// если нажата кнопка пауза ждем
				while(CSpectrDlg::m_PressBn == PAUSE) Sleep(10);
			}
			// kill the luft
			KillThelLuft();
		}
		// обнулим ТТЛ выходы, чтобы ничего не висело
		CSpectrDlg::m_pE2010Module->TTL_OUT(NULL);
		//m_CurCarriagePosInSteps += stepNumber;	
	}
	return true;
}


void CSampPosSystem::KillThelLuft(void)
{
	// выберим люфт
	WORD ttlOut(0), flipper(0x1), pulse(0), luftInSteps(0),pulseHighLevel(0);
	luftInSteps = m_LuftInMillimetre*m_StepsInMillimetre;

	if(m_IsPulseSignalSys) pulseHighLevel = pow(2.0,m_DirOrDownPulseTTLoutPin-1);
	else pulseHighLevel = pow(2.0,m_PulseOrUpPulseTTLoutPin-1);
	for(int stp(0);stp<2*luftInSteps;stp++)
	{
		ttlOut = pulseHighLevel*flipper;
		// проверим ошибки при выводе числа на ТТЛ, в случае ошибки шаг не считаем
		if(!(CSpectrDlg::m_pE2010Module->TTL_OUT(ttlOut))) stp-- ;
		else 
		{ 
			Sleep(m_TimeDelay); 
			flipper^=0x1;
			// вычисляем оценочное положение каретк в милиметрах
			if(ttlOut==pulseHighLevel) pulse++;
			if (pulse>=m_StepsInMillimetre)
			{
				// выведем расчетное положение каретки
				if(CSpectrDlg::m_PressBn !=STOP)
					CSpectrDlg::PrintInEdit(m_pEditForEstPos,++m_EstCarriagePosition);
				else 
					m_EstCarriagePosition++;
				pulse = 0;
			}
		}
		// если нажата кнопка пауза ждем
		while(CSpectrDlg::m_PressBn == PAUSE) Sleep(10);
	}
	WORD valForDiraction(0);
	if(m_IsPulseSignalSys) pulseHighLevel = pow(2.0,m_PulseOrUpPulseTTLoutPin-1);
	else
	{
		pulseHighLevel = pow(2.0,m_PulseOrUpPulseTTLoutPin-1);
		valForDiraction = pow(2.0,m_DirOrDownPulseTTLoutPin-1);
	}
	for(int stp(0);stp<2*luftInSteps;stp++)
	{
		ttlOut = pulseHighLevel*flipper + valForDiraction;
		// проверим ошибки при выводе числа на ТТЛ, в случае ошибки шаг не считаем
		if(!(CSpectrDlg::m_pE2010Module->TTL_OUT(ttlOut))) stp-- ;
		else 
		{
			Sleep(m_TimeDelay); 
			flipper^=0x1;
			// вычисляем оценочное положение каретк в милиметрах
			if(ttlOut==(pulseHighLevel+valForDiraction)) pulse++;
			if (pulse>=m_StepsInMillimetre)
			{
				// выведем расчетное положение каретки
				if(CSpectrDlg::m_PressBn !=STOP)
					CSpectrDlg::PrintInEdit(m_pEditForEstPos,--m_EstCarriagePosition);
				else 
					m_EstCarriagePosition--;
				pulse = 0;
			}
		}
		// если нажата кнопка пауза ждем
		while(CSpectrDlg::m_PressBn == PAUSE) Sleep(10);
	}
}
