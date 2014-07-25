// HSMSettings.cpp: файл реализации
//

#include "stdafx.h"
#include "Spectr.h"
#include "SpectrDlg.h"
#include "HSMSettings.h"
#include "afxdialogex.h"


// диалоговое окно CHSMSettings

IMPLEMENT_DYNAMIC(CHSMSettings, CPropertyPage)

CHSMSettings::CHSMSettings()
	: CPropertyPage(CHSMSettings::IDD)
{
	
}

CHSMSettings::~CHSMSettings()
{
}

void CHSMSettings::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_EditFrequencyRange);

	m_pbOK=(CButton*)((CPropertySheet*) GetParent()->GetDlgItem(IDOK));
	m_pbApply=(CButton*)((CPropertySheet*) GetParent()->GetDlgItem(ID_APPLY_NOW));

	char * out=new char[200];

	GetPrivateProfileString(_T("HSM"),_T("Frequency range"),NULL,LPWSTR(out),200,CSpectrDlg::m_IniFileName);
	(&m_EditFrequencyRange)->SetWindowTextW(LPCTSTR(out));
	GetPrivateProfileString(_T("HSM"),_T("Reaction time"),NULL,LPWSTR(out),200,CSpectrDlg::m_IniFileName);
	SetDlgItemTextW(IDC_EDIT2,LPCTSTR(out));
	
}


BEGIN_MESSAGE_MAP(CHSMSettings, CPropertyPage)
	ON_EN_CHANGE(IDC_EDIT1, &CHSMSettings::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, &CHSMSettings::OnEnChangeEdit2)
END_MESSAGE_MAP()


// обработчики сообщений CHSMSettings


void CHSMSettings::OnEnChangeEdit1()
{
	CString str;
	m_EditFrequencyRange.GetWindowTextW(str);

	if (m_EditFrequencyRange.GetWindowTextLengthW()==0 || str==_T("0"))
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


BOOL CHSMSettings::OnSetActive()
{
	// TODO: добавьте специализированный код или вызов базового класса
	m_pbOK->EnableWindow(true);
	return CPropertyPage::OnSetActive();
}


void CHSMSettings::OnEnChangeEdit2()
{
	CString str;
	GetDlgItemTextW(IDC_EDIT2,str);
	WORD intOfEdit2(StrToInt(str)); 

	if (intOfEdit2 == 0)
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
