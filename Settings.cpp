// Settings.cpp: файл реализации
//

#include "stdafx.h"
#include "Spectr.h"
#include "SpectrDlg.h"
#include "Settings.h"



// CSettings

IMPLEMENT_DYNAMIC(CSettings, CPropertySheet)

CSettings::CSettings(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	

}

CSettings::CSettings(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, 0)
{
	AddPage(&m_ChopperSetDlg); 
	AddPage(&m_HSMSetDlg); 
	AddPage(&m_StepMotorSetDlg);
	AddPage(&m_E2010SetDlg);
	AddPage(&m_IK220SetDlg);

}

CSettings::~CSettings()
{
}

BOOL CSettings::OnInitDialog() 
{
   CPropertySheet::OnInitDialog();
   for(int i=GetPageCount()-1;i>-1,i--;)SetActivePage(i);// активируем все страницы
	

   return TRUE;   // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_MESSAGE_MAP(CSettings, CPropertySheet)
	ON_BN_CLICKED(IDOK,OnOK)
	ON_BN_CLICKED(ID_APPLY_NOW,OnApply)
END_MESSAGE_MAP()

void CSettings::OnOK()
{
	SaveToIniFile(CSpectrDlg::m_IniFileName,true);
	CPropertySheet::EndDialog(IDOK);

}

void CSettings::OnApply()
{
	SaveToIniFile(CSpectrDlg::m_IniFileName,true);
	(CButton*)GetDlgItem(ID_APPLY_NOW)->EnableWindow(false);//погасим Apply	
}



// функция сохранения


void CSettings::SaveToIniFile(CString fileName,bool fileExist)
{
	int DefaultSettings[30]={NULL};
	CString in(_T(""));

	if (!fileExist)
	{
		//зададим массив параметров по умолчанию
		DefaultSettings[CHOPPER_FREQUENCY_STEP]=1;
		DefaultSettings[CHOPPER_TIME_DELAY]=50;
		DefaultSettings[CHOPPER_INVERSE_STEP_RESOLUTION]=8;
		DefaultSettings[CHOPPER_ROTOR_WINDOWS_NUMBER]=3;
		DefaultSettings[CHOPPER_GEAR_RATIO]=2;
		DefaultSettings[HSM_FREQUENCY_RANGE]=1;
		DefaultSettings[HSM_REACTION_TIME]=2;
		DefaultSettings[SPS_NUMBER_OF_DEVISIONS]=2;
		DefaultSettings[SPS_CALIBRATION]=80;
		DefaultSettings[SPS_MIN_DISTANCE_BW_FILTERS]=150;
		DefaultSettings[SPS_CARRIAGE_WORK_RANGE]=500;
		DefaultSettings[E2010_TURBINE_SIGNAL_inPIN]=1;
		DefaultSettings[E2010_SPS_STEPPER_SIGNAL_outPIN]=1;
		DefaultSettings[E2010_SPS_STEPPER_DIRACTION_outPIN]=2;
		DefaultSettings[E2010_CHOPPER_CHANNEL]=1;
		DefaultSettings[E2010_HSM_CHANNEL]=2;
		DefaultSettings[E2010_HSM_CNTL_CHANNEL]=1;
		DefaultSettings[E2010_MONITOR_CHANNEL]=3;
		DefaultSettings[E2010_DETECTOR_CHANNEL]=4;
		DefaultSettings[E2010_ADC_RATE]=5000;
		DefaultSettings[SPS_SIGNAL_SYSTEM]=DIR_SYSTEM;
		DefaultSettings[SPS_CURRENT_CARRIAGE_POS]=200;
		DefaultSettings[SPS_TIME_DELAY]=10;
		
	}
	
	//запишем в ini файл если не создан
	
	if (fileExist) m_ChopperSetDlg.m_EditFrequencyStep.GetWindowTextW(in);
	else in.Format(_T("%i"),DefaultSettings[CHOPPER_FREQUENCY_STEP]);
	WritePrivateProfileString(_T("Chopper"),_T("Frequency step"),in,fileName);
		
	if (fileExist) m_ChopperSetDlg.m_EditTimeDelay.GetWindowTextW(in);
	else in.Format(_T("%i"),DefaultSettings[CHOPPER_TIME_DELAY]);
	WritePrivateProfileString(_T("Chopper"),_T("Time delay"),in,fileName);
	
	if (fileExist) m_ChopperSetDlg.m_EditInverseStepResolution.GetWindowTextW(in);
	else in.Format(_T("%i"),DefaultSettings[CHOPPER_INVERSE_STEP_RESOLUTION]);
	WritePrivateProfileString(_T("Chopper"),_T("Inverse step resolution"),in,fileName);
	
	if (fileExist) m_ChopperSetDlg.m_EditRotorWindowsNumber.GetWindowTextW(in);
	else in.Format(_T("%i"),DefaultSettings[CHOPPER_ROTOR_WINDOWS_NUMBER]);
	WritePrivateProfileString(_T("Chopper"),_T("Rotor windows number"),in,fileName);

	if (fileExist) m_ChopperSetDlg.m_EditGearRatio.GetWindowTextW(in);
	else in.Format(_T("%i"),DefaultSettings[CHOPPER_GEAR_RATIO]);
	WritePrivateProfileString(_T("Chopper"),_T("Gear Ratio"),in,fileName);
	
	if (fileExist) m_HSMSetDlg.m_EditFrequencyRange.GetWindowTextW(in);
	else in.Format(_T("%i"),DefaultSettings[HSM_FREQUENCY_RANGE]);
	WritePrivateProfileString(_T("HSM"),_T("Frequency range"),in,fileName);

	if (fileExist) m_HSMSetDlg.GetDlgItemTextW(IDC_EDIT2,in);
	else in.Format(_T("%i"),DefaultSettings[HSM_REACTION_TIME]);
	WritePrivateProfileString(_T("HSM"),_T("Reaction time"),in,fileName);
	
	if (fileExist) m_StepMotorSetDlg.m_EditNumberOfDevisions.GetWindowTextW(in);
	else in.Format(_T("%i"),DefaultSettings[SPS_NUMBER_OF_DEVISIONS]);
	WritePrivateProfileString(_T("SPS"),_T("Number of devisions"),in,fileName);

	if (fileExist) m_StepMotorSetDlg.GetDlgItem(IDC_EDIT6)->GetWindowTextW(in);
	else in.Format(_T("%i"),DefaultSettings[SPS_TIME_DELAY]);
	WritePrivateProfileString(_T("SPS"),_T("Time delay"),in,fileName);
	
	if (fileExist) m_StepMotorSetDlg.m_EditCalibration.GetWindowTextW(in);
	else in.Format(_T("%i"),DefaultSettings[SPS_CALIBRATION]);
	WritePrivateProfileString(_T("SPS"),_T("Calibration"),in,fileName);

	if (fileExist) m_StepMotorSetDlg.m_EditMinDistanceBwFilters.GetWindowTextW(in);
	else in.Format(_T("%i"),DefaultSettings[SPS_MIN_DISTANCE_BW_FILTERS]);
	WritePrivateProfileString(_T("SPS"),_T("Min distance bw filters"),in,fileName);

	if (fileExist) m_StepMotorSetDlg.m_EditCarriageWorkRange.GetWindowTextW(in);
	else in.Format(_T("%i"),DefaultSettings[SPS_CARRIAGE_WORK_RANGE]);
	WritePrivateProfileString(_T("SPS"),_T("Carriage working range"),in,fileName);

	if (fileExist) m_StepMotorSetDlg.GetDlgItem(IDC_EDIT5)->GetWindowTextW(in);
	else in.Format(_T("%i"),DefaultSettings[SPS_CURRENT_CARRIAGE_POS]);
	WritePrivateProfileString(_T("SPS"),_T("Current carriage position"),in,fileName);

	if (fileExist) in.Format(_T("%i"),m_StepMotorSetDlg.m_SignalSystem);
	else in.Format(_T("%i"),DefaultSettings[SPS_SIGNAL_SYSTEM]);
	WritePrivateProfileString(_T("SPS"),_T("Signal system"),in,fileName);
	
	if (fileExist) m_E2010SetDlg.m_EditTurbineSignalInPin.GetWindowTextW(in);
	else in.Format(_T("%i"),DefaultSettings[E2010_TURBINE_SIGNAL_inPIN]);
	WritePrivateProfileString(_T("E2010"),_T("Turbine signal (in)pin"),in,fileName);
	
	if (fileExist) m_E2010SetDlg.m_EditSPSStepperSignalOutPin.GetWindowTextW(in);
	else in.Format(_T("%i"),DefaultSettings[E2010_SPS_STEPPER_SIGNAL_outPIN]);
	WritePrivateProfileString(_T("E2010"),_T("SPS stepper signal (out)pin"),in,fileName);

	if (fileExist) m_E2010SetDlg.m_EditSPSStepperDiractionOutPin.GetWindowTextW(in);
	else in.Format(_T("%i"),DefaultSettings[E2010_SPS_STEPPER_DIRACTION_outPIN]);
	WritePrivateProfileString(_T("E2010"),_T("SPS stepper diraction (out)pin"),in,fileName);
	
	if (fileExist) m_E2010SetDlg.GetDlgItem(IDC_EDIT4)->GetWindowTextW(in);
	else in.Format(_T("%i"),DefaultSettings[E2010_CHOPPER_CHANNEL]);
	WritePrivateProfileString(_T("E2010"),_T("Chopper signal channel"),in,fileName);

	if (fileExist) m_E2010SetDlg.GetDlgItem(IDC_EDIT5)->GetWindowTextW(in);
	else in.Format(_T("%i"),DefaultSettings[E2010_HSM_CHANNEL]);
	WritePrivateProfileString(_T("E2010"),_T("HSM signal channel"),in,fileName);

	if (fileExist) m_E2010SetDlg.GetDlgItem(IDC_EDIT6)->GetWindowTextW(in);
	else in.Format(_T("%i"),DefaultSettings[E2010_MONITOR_CHANNEL]);
	WritePrivateProfileString(_T("E2010"),_T("Monitor signal channel"),in,fileName);

	if (fileExist) m_E2010SetDlg.GetDlgItem(IDC_EDIT7)->GetWindowTextW(in);
	else in.Format(_T("%i"),DefaultSettings[E2010_DETECTOR_CHANNEL]);
	WritePrivateProfileString(_T("E2010"),_T("Detector signal channel"),in,fileName);

	if (fileExist) m_E2010SetDlg.GetDlgItem(IDC_EDIT8)->GetWindowTextW(in);
	else in.Format(_T("%i"),DefaultSettings[E2010_ADC_RATE]);
	WritePrivateProfileString(_T("E2010"),_T("ADC rate"),in,fileName);

	if (fileExist) m_E2010SetDlg.GetDlgItem(IDC_EDIT9)->GetWindowTextW(in);
	else in.Format(_T("%i"),DefaultSettings[E2010_HSM_CNTL_CHANNEL]);
	WritePrivateProfileString(_T("E2010"),_T("HSM control channel"),in,fileName);
}
