// ChopperSettings.cpp: файл реализации
//

#include "stdafx.h"
#include "Spectr.h"
#include "SpectrDlg.h"
#include "ChopperSettings.h"
#include "afxdialogex.h"


// диалоговое окно CChopperSettings

IMPLEMENT_DYNAMIC(CChopperSettings, CPropertyPage)

CChopperSettings::CChopperSettings()
	: CPropertyPage(CChopperSettings::IDD)
{

}

CChopperSettings::~CChopperSettings()
{
}

void CChopperSettings::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_EDIT1,m_EditFrequencyStep);
	DDX_Control(pDX,IDC_EDIT2,m_EditTimeDelay);
	DDX_Control(pDX,IDC_EDIT3,m_EditInverseStepResolution);
	DDX_Control(pDX,IDC_EDIT4,m_EditRotorWindowsNumber);
	DDX_Control(pDX,IDC_EDIT5,m_EditGearRatio);


	m_pbOK=(CButton*)((CPropertySheet*) GetParent()->GetDlgItem(IDOK));
	m_pbApply=(CButton*)((CPropertySheet*) GetParent()->GetDlgItem(ID_APPLY_NOW));

	

}


BEGIN_MESSAGE_MAP(CChopperSettings, CPropertyPage)
	ON_EN_CHANGE(IDC_EDIT1, &CChopperSettings::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, &CChopperSettings::OnEnChangeEdit2)
	ON_EN_CHANGE(IDC_EDIT3, &CChopperSettings::OnEnChangeEdit3)
	ON_EN_CHANGE(IDC_EDIT4, &CChopperSettings::OnEnChangeEdit4)
	ON_EN_CHANGE(IDC_EDIT5, &CChopperSettings::OnEnChangeEdit5)
END_MESSAGE_MAP()


// обработчики сообщений CChopperSettings


void CChopperSettings::OnEnChangeEdit1()
{
	CString str;
	m_EditFrequencyStep.GetWindowTextW(str);

	if (m_EditFrequencyStep.GetWindowTextLengthW()==0 || str==_T("0"))
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


void CChopperSettings::OnEnChangeEdit2()
{
	CString str;
	m_EditTimeDelay.GetWindowTextW(str);

	if (m_EditTimeDelay.GetWindowTextLengthW()==0 || str==_T("0"))
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


void CChopperSettings::OnEnChangeEdit3()
{
	CString str;
	m_EditInverseStepResolution.GetWindowTextW(str);

	if (m_EditInverseStepResolution.GetWindowTextLengthW()==0 || str==_T("0"))
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


void CChopperSettings::OnEnChangeEdit4()
{
	CString str;
	m_EditRotorWindowsNumber.GetWindowTextW(str);

	if (m_EditRotorWindowsNumber.GetWindowTextLengthW()==0 || str==_T("0"))
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


void CChopperSettings::OnEnChangeEdit5()
{
	CString str;
	m_EditGearRatio.GetWindowTextW(str);

	if (m_EditGearRatio.GetWindowTextLengthW()==0 || str==_T("0"))
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






BOOL CChopperSettings::OnSetActive()
{
	m_pbOK->EnableWindow(true);
	return CPropertyPage::OnSetActive();
}


BOOL CChopperSettings::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  Добавить дополнительную инициализацию
	char* out=new char[200];

	GetPrivateProfileString(_T("Chopper"),_T("Frequency step"),NULL,LPWSTR(out),200,CSpectrDlg::m_IniFileName);
	m_EditFrequencyStep.SetWindowTextW(LPCTSTR(out));
	
	GetPrivateProfileString(_T("Chopper"),_T("Time delay"),NULL,LPWSTR(out),200,CSpectrDlg::m_IniFileName);
	m_EditTimeDelay.SetWindowTextW(LPCTSTR(out));

	GetPrivateProfileString(_T("Chopper"),_T("Inverse step resolution"),NULL,LPWSTR(out),200,CSpectrDlg::m_IniFileName);
	m_EditInverseStepResolution.SetWindowTextW(LPCTSTR(out));

	GetPrivateProfileString(_T("Chopper"),_T("Rotor windows number"),NULL,LPWSTR(out),200,CSpectrDlg::m_IniFileName);
	m_EditRotorWindowsNumber.SetWindowTextW(LPCTSTR(out));
	
	GetPrivateProfileString(_T("Chopper"),_T("Gear ratio"),NULL,LPWSTR(out),200,CSpectrDlg::m_IniFileName);
	m_EditGearRatio.SetWindowTextW(LPCTSTR(out));
	return TRUE;  // return TRUE unless you set the focus to a control
	// Исключение: страница свойств OCX должна возвращать значение FALSE
}
