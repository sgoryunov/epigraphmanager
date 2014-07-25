// StepMotorSettings.cpp: файл реализации
//

#include "stdafx.h"
#include "Spectr.h"
#include "SpectrDlg.h"
#include "StepMotorSettings.h"
#include "afxdialogex.h"


// диалоговое окно CStepMotorSettings

IMPLEMENT_DYNAMIC(CStepMotorSettings, CPropertyPage)

CStepMotorSettings::CStepMotorSettings()
	: CPropertyPage(CStepMotorSettings::IDD)
{

}

CStepMotorSettings::~CStepMotorSettings()
{
}

void CStepMotorSettings::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_EditNumberOfDevisions);
	DDX_Control(pDX, IDC_EDIT2, m_EditCalibration);
	DDX_Control(pDX, IDC_EDIT3, m_EditMinDistanceBwFilters);
	DDX_Control(pDX, IDC_EDIT4, m_EditCarriageWorkRange);
	DDX_Radio(pDX, IDC_RADIO1, m_SignalSystem);

	m_SignalSystem = GetPrivateProfileInt(_T("SPS"),_T("Signal system"),NULL,CSpectrDlg::m_IniFileName);
	m_pbOK=(CButton*)((CPropertySheet*) GetParent()->GetDlgItem(IDOK));
	m_pbApply=(CButton*)((CPropertySheet*) GetParent()->GetDlgItem(ID_APPLY_NOW));

	char * out=new char[200];

	GetPrivateProfileString(_T("SPS"),_T("Number of devisions"),NULL,LPWSTR(out),200,CSpectrDlg::m_IniFileName);
	m_EditNumberOfDevisions.SetWindowTextW(LPCTSTR(out));

	GetPrivateProfileString(_T("SPS"),_T("Calibration"),NULL,LPWSTR(out),200,CSpectrDlg::m_IniFileName);
	m_EditCalibration.SetWindowTextW(LPCTSTR(out));

	GetPrivateProfileString(_T("SPS"),_T("Min distance bw filters"),NULL,LPWSTR(out),200,CSpectrDlg::m_IniFileName);
	m_EditMinDistanceBwFilters.SetWindowTextW(LPCTSTR(out));

	GetPrivateProfileString(_T("SPS"),_T("Carriage working range"),NULL,LPWSTR(out),200,CSpectrDlg::m_IniFileName);
	m_EditCarriageWorkRange.SetWindowText(LPCTSTR(out));

	GetPrivateProfileString(_T("SPS"),_T("Time delay"),NULL,LPWSTR(out),200,CSpectrDlg::m_IniFileName);
	GetDlgItem(IDC_EDIT6)->SetWindowTextW(LPCTSTR(out));

	GetPrivateProfileString(_T("SPS"),_T("Current carriage position"),NULL,LPWSTR(out),200,CSpectrDlg::m_IniFileName);
	GetDlgItem(IDC_EDIT5)->SetWindowTextW(LPCTSTR(out));




}


BEGIN_MESSAGE_MAP(CStepMotorSettings, CPropertyPage)
	ON_EN_CHANGE(IDC_EDIT1, &CStepMotorSettings::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, &CStepMotorSettings::OnEnChangeEdit2)
	ON_EN_CHANGE(IDC_EDIT3, &CStepMotorSettings::OnEnChangeEdit3)
	ON_EN_CHANGE(IDC_EDIT4, &CStepMotorSettings::OnEnChangeEdit4)
	ON_BN_CLICKED(IDC_RADIO1, &CStepMotorSettings::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CStepMotorSettings::OnBnClickedRadio2)
	ON_EN_CHANGE(IDC_EDIT6, &CStepMotorSettings::OnEnChangeEdit6)
	ON_EN_CHANGE(IDC_EDIT5, &CStepMotorSettings::OnEnChangeEdit5)
END_MESSAGE_MAP()


// обработчики сообщений CStepMotorSettings


void CStepMotorSettings::OnEnChangeEdit1()
{
	WORD val = GetDlgItemInt(IDC_EDIT1);
	switch(val)
	{
	case 1:
		m_pbOK->EnableWindow(true);
		m_pbApply->EnableWindow(true);
		break;
	case 2:
		m_pbOK->EnableWindow(true);
		m_pbApply->EnableWindow(true);
		break;
	case 4:
		m_pbOK->EnableWindow(true);
		m_pbApply->EnableWindow(true);
		break;
	case 8:
		m_pbOK->EnableWindow(true);
		m_pbApply->EnableWindow(true);
		break;
	case 16:
		m_pbOK->EnableWindow(true);
		m_pbApply->EnableWindow(true);
		break;
	default:
		m_pbApply->EnableWindow(false);
		m_pbOK->EnableWindow(false);
	}	
}


void CStepMotorSettings::OnEnChangeEdit2()
{
	CString str;
	m_EditCalibration.GetWindowTextW(str);

	if (m_EditCalibration.GetWindowTextLengthW()==0 || str==_T("0"))
	{
		m_pbOK->EnableWindow(false);
		m_pbApply->EnableWindow(false);
	}
	else
	{
		m_pbApply->EnableWindow();
		m_pbOK->EnableWindow(true);
	}
}


void CStepMotorSettings::OnEnChangeEdit3()
{
	if (m_EditMinDistanceBwFilters.GetWindowTextLengthW()==0)
	{
		m_pbOK->EnableWindow(false);
		m_pbApply->EnableWindow(false);
	}
	else
	{
		m_pbApply->EnableWindow();
		m_pbOK->EnableWindow(true);
	}
}


void CStepMotorSettings::OnEnChangeEdit4()
{
	CString str;
	m_EditCarriageWorkRange.GetWindowTextW(str);

	if (m_EditCarriageWorkRange.GetWindowTextLengthW()==0 || str==_T("0") ||
		StrToInt(str)>500)
	{
		m_pbOK->EnableWindow(false);
		m_pbApply->EnableWindow(false);
	}
	else
	{
		m_pbApply->EnableWindow();
		m_pbOK->EnableWindow(true);
	}
}


BOOL CStepMotorSettings::OnSetActive()
{
	// TODO: добавьте специализированный код или вызов базового класса
	m_pbOK->EnableWindow(true);
	return CPropertyPage::OnSetActive();
}




void CStepMotorSettings::OnBnClickedRadio1()
{
	if(m_SignalSystem!=0)
	{
		m_pbApply->EnableWindow(true);
		m_SignalSystem = 0;
	}
	
}


void CStepMotorSettings::OnBnClickedRadio2()
{
	if(m_SignalSystem!=1)
	{
		m_pbApply->EnableWindow(true);
		m_SignalSystem = 1;
	}
}

// filter for time delay bw pulse
void CStepMotorSettings::OnEnChangeEdit6()
{
	CString str;
	GetDlgItem(IDC_EDIT6)->GetWindowTextW(str);

	if (GetDlgItem(IDC_EDIT6)->GetWindowTextLengthW()==0 || str==_T("0"))
	{
		m_pbOK->EnableWindow(false);
		m_pbApply->EnableWindow(false);
	}
	else
	{
		m_pbApply->EnableWindow();
		m_pbOK->EnableWindow(true);
	}
}

// filter for current carriage position
void CStepMotorSettings::OnEnChangeEdit5()
{
	CString str;
	GetDlgItem(IDC_EDIT5)->GetWindowTextW(str);
	WORD val = StrToInt(str);
	WORD min = GetDlgItemInt(IDC_EDIT3);
	WORD max = min + GetDlgItemInt(IDC_EDIT4);
	if (min > val || max < val)
	{
		m_pbOK->EnableWindow(false);
		m_pbApply->EnableWindow(false);
	}
	else
	{
		m_pbApply->EnableWindow();
		m_pbOK->EnableWindow(true);
	}
}


BOOL CStepMotorSettings::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	 m_tt.Create(this);
	 m_tt.AddTool(GetDlgItem(IDC_EDIT1),_T("Must be 1,2,4,8,16"));
	 WORD min = GetDlgItemInt(IDC_EDIT3);
	 WORD max = min + GetDlgItemInt(IDC_EDIT4);
	 CString str;
	 str.Format(_T("Must between %i and %i"),min,max);
	 m_tt.AddTool(GetDlgItem(IDC_EDIT5),str);

	return TRUE;  // return TRUE unless you set the focus to a control
	// Исключение: страница свойств OCX должна возвращать значение FALSE
}


BOOL CStepMotorSettings::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message >= WM_MOUSEFIRST && pMsg->message <= WM_MOUSELAST)
		m_tt.RelayEvent(pMsg);

	return CPropertyPage::PreTranslateMessage(pMsg);
}
