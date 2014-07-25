// E2010Settings.cpp: файл реализации
//

#include "stdafx.h"
#include "Spectr.h"
#include "SpectrDlg.h"
#include "E2010Settings.h"
#include "afxdialogex.h"



// диалоговое окно CE2010Settings

IMPLEMENT_DYNAMIC(CE2010Settings, CPropertyPage)

CE2010Settings::CE2010Settings()
	: CPropertyPage(CE2010Settings::IDD)
{
	m_EditText= _T("");
	m_AdcGroupFlag = m_DacGroupFlag = m_TTLinGroupFlag = 
		m_TTLoutGroupFlag = m_AdcRateFlag = true ;
}

CE2010Settings::~CE2010Settings()
{
}

void CE2010Settings::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_EDIT1,m_EditTurbineSignalInPin);
	DDX_Control(pDX,IDC_EDIT2,m_EditSPSStepperSignalOutPin);
	DDX_Control(pDX,IDC_EDIT3,m_EditSPSStepperDiractionOutPin);

	m_pbOK=(CButton*)((CPropertySheet*) GetParent()->GetDlgItem(IDOK));
	m_pbApply=(CButton*)((CPropertySheet*) GetParent()->GetDlgItem(ID_APPLY_NOW));

	m_EditText.Format(_T("%i"),GetPrivateProfileInt(_T("E2010"),_T("Turbine signal (in)pin"),NULL,CSpectrDlg::m_IniFileName));
	DDX_Text(pDX, IDC_EDIT1, m_EditText);
	DDV_MaxChars(pDX, m_EditText, 200);

	m_EditText.Format(_T("%i"),GetPrivateProfileInt(_T("E2010"),_T("SPS stepper signal (out)pin"),NULL,CSpectrDlg::m_IniFileName));
	DDX_Text(pDX, IDC_EDIT2, m_EditText);
	DDV_MaxChars(pDX, m_EditText, 200);

	m_EditText.Format(_T("%i"),GetPrivateProfileInt(_T("E2010"),_T("SPS stepper diraction (out)pin"),NULL,CSpectrDlg::m_IniFileName));
	DDX_Text(pDX, IDC_EDIT3, m_EditText);
	DDV_MaxChars(pDX, m_EditText, 200);

	m_EditText.Format(_T("%i"),GetPrivateProfileInt(_T("E2010"),_T("Chopper signal channel"),NULL,CSpectrDlg::m_IniFileName));
	DDX_Text(pDX, IDC_EDIT4, m_EditText);
	DDV_MaxChars(pDX, m_EditText, 200);

	m_EditText.Format(_T("%i"),GetPrivateProfileInt(_T("E2010"),_T("HSM signal channel"),NULL,CSpectrDlg::m_IniFileName));
	DDX_Text(pDX, IDC_EDIT5, m_EditText);
	DDV_MaxChars(pDX, m_EditText, 200);

	m_EditText.Format(_T("%i"),GetPrivateProfileInt(_T("E2010"),_T("Monitor signal channel"),NULL,CSpectrDlg::m_IniFileName));
	DDX_Text(pDX, IDC_EDIT6, m_EditText);
	DDV_MaxChars(pDX, m_EditText, 200);

	m_EditText.Format(_T("%i"),GetPrivateProfileInt(_T("E2010"),_T("Detector signal channel"),NULL,CSpectrDlg::m_IniFileName));
	DDX_Text(pDX, IDC_EDIT7, m_EditText);
	DDV_MaxChars(pDX, m_EditText, 200);

	m_EditText.Format(_T("%i"),GetPrivateProfileInt(_T("E2010"),_T("ADC rate"),NULL,CSpectrDlg::m_IniFileName));
	DDX_Text(pDX, IDC_EDIT8, m_EditText);
	DDV_MaxChars(pDX, m_EditText, 200);

	m_EditText.Format(_T("%i"),GetPrivateProfileInt(_T("E2010"),_T("HSM control channel"),NULL,CSpectrDlg::m_IniFileName));
	DDX_Text(pDX, IDC_EDIT9, m_EditText);
	DDV_MaxChars(pDX, m_EditText, 200);
}


BEGIN_MESSAGE_MAP(CE2010Settings, CPropertyPage)
	ON_EN_CHANGE(IDC_EDIT1, &CE2010Settings::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, &CE2010Settings::OnEnChangeEdit2)
	ON_EN_CHANGE(IDC_EDIT3, &CE2010Settings::OnEnChangeEdit3)
	ON_EN_CHANGE(IDC_EDIT4, &CE2010Settings::OnEnChangeEdit4)
	ON_EN_CHANGE(IDC_EDIT5, &CE2010Settings::OnEnChangeEdit5)
	ON_EN_CHANGE(IDC_EDIT6, &CE2010Settings::OnEnChangeEdit6)
	ON_EN_CHANGE(IDC_EDIT7, &CE2010Settings::OnEnChangeEdit7)
	ON_EN_CHANGE(IDC_EDIT8, &CE2010Settings::OnEnChangeEdit8)
	ON_EN_CHANGE(IDC_EDIT9, &CE2010Settings::OnEnChangeEdit9)
END_MESSAGE_MAP()

// обработчики сообщений CE2010Settings


void CE2010Settings::OnEnChangeEdit1()
{
	CString str = _T("");
	m_EditTurbineSignalInPin.GetWindowTextW(str);

	if (str.IsEmpty())
	{
		m_pbOK->EnableWindow(false);
		m_pbApply->EnableWindow(false);
		m_TTLinGroupFlag = false;
	}
	else
	{
		WORD edit1 = StrToInt(str);
		if(edit1 == 0 || edit1 > 16)
		{
			m_pbOK->EnableWindow(false);
			m_pbApply->EnableWindow(false);
			m_TTLinGroupFlag = false;
		}
		else
		{
			if(m_AdcGroupFlag && m_DacGroupFlag && 
				m_TTLoutGroupFlag && m_AdcRateFlag)
			{
				m_pbApply->EnableWindow(true);
				m_pbOK->EnableWindow(true);
			}
			m_TTLinGroupFlag = true;
		}
	}
}


void CE2010Settings::OnEnChangeEdit2()
{
	CString str = _T("");
	WORD edit2, edit3;

	m_EditSPSStepperDiractionOutPin.GetWindowTextW(str);
	edit3 = StrToInt(str);
	m_EditSPSStepperSignalOutPin.GetWindowTextW(str);
	edit2 = StrToInt(str);

	if(edit2!=0)
		if(edit2>16 || edit3==edit2)
		{
			m_pbOK->EnableWindow(false);
			m_pbApply->EnableWindow(false);
			m_TTLoutGroupFlag = false;
		}
		else
		{
			if(m_AdcGroupFlag && m_DacGroupFlag && 
				m_TTLinGroupFlag && m_AdcRateFlag)
			{
				m_pbApply->EnableWindow(true);
				m_pbOK->EnableWindow(true);
			}
			m_TTLoutGroupFlag = true;
		}
	else
	{
		m_pbOK->EnableWindow(false);
		m_pbApply->EnableWindow(false);	
		m_TTLoutGroupFlag = false;
	}
	
}


void CE2010Settings::OnEnChangeEdit3()
{
	CString str = _T("");
	WORD edit2, edit3;

	m_EditSPSStepperSignalOutPin.GetWindowTextW(str);
	edit2 = StrToInt(str);
	m_EditSPSStepperDiractionOutPin.GetWindowTextW(str);
	edit3 = StrToInt(str);

	if(edit3!=0)
		if(edit3>16 || edit3==edit2)
		{
			m_pbOK->EnableWindow(false);
			m_pbApply->EnableWindow(false);
			m_TTLoutGroupFlag = false;
		}
		else
		{
			if(m_AdcGroupFlag && m_DacGroupFlag && 
				m_TTLinGroupFlag && m_AdcRateFlag)
			{
				m_pbApply->EnableWindow(true);
				m_pbOK->EnableWindow(true);	
			}
			m_TTLoutGroupFlag = true;
		}
	else
	{
		m_pbOK->EnableWindow(false);
		m_pbApply->EnableWindow(false);
		m_TTLoutGroupFlag = false;
	}
}


BOOL CE2010Settings::OnSetActive()
{
	// TODO: добавьте специализированный код или вызов базового класса
	m_pbOK->EnableWindow(true);
	return CPropertyPage::OnSetActive();
}


void CE2010Settings::OnEnChangeEdit4()
{
	CString str;
	WORD edit4,edit5,edit6,edit7;

	GetDlgItem(IDC_EDIT4)->GetWindowTextW(str);
	edit4 = StrToInt(str);
	GetDlgItem(IDC_EDIT5)->GetWindowTextW(str);
	edit5 = StrToInt(str);
	GetDlgItem(IDC_EDIT6)->GetWindowTextW(str);
	edit6 = StrToInt(str);
	GetDlgItem(IDC_EDIT7)->GetWindowTextW(str);
	edit7 = StrToInt(str);

	if(edit4!=0)
		if(edit4>4 || edit4==edit5 || edit4==edit6 || edit4==edit7)
		{
			m_pbOK->EnableWindow(false);
			m_pbApply->EnableWindow(false);
			m_AdcGroupFlag = false;
		}
		else
		{
			if(m_TTLinGroupFlag && m_DacGroupFlag &&
				m_TTLoutGroupFlag && m_AdcRateFlag)
			{
				m_pbApply->EnableWindow(true);
				m_pbOK->EnableWindow(true);	
			}
			m_AdcGroupFlag = true;
		}
	else
		if((edit4+edit5+edit6+edit7)==0)
		{
			m_pbOK->EnableWindow(false);
			m_pbApply->EnableWindow(false);
			m_AdcGroupFlag = false;
		}
		else
		{
			if(m_TTLinGroupFlag && m_DacGroupFlag &&
				m_TTLoutGroupFlag && m_AdcRateFlag)
			{
				m_pbApply->EnableWindow(true);
				m_pbOK->EnableWindow(true);	
			}
			m_AdcGroupFlag = true;
		}
}


void CE2010Settings::OnEnChangeEdit5()
{
	CString str;
	WORD edit4,edit5,edit6,edit7;

	GetDlgItem(IDC_EDIT4)->GetWindowTextW(str);
	edit4 = StrToInt(str);
	GetDlgItem(IDC_EDIT5)->GetWindowTextW(str);
	edit5 = StrToInt(str);
	GetDlgItem(IDC_EDIT6)->GetWindowTextW(str);
	edit6 = StrToInt(str);
	GetDlgItem(IDC_EDIT7)->GetWindowTextW(str);
	edit7 = StrToInt(str);

	if(edit5!=0)
		if(edit5>4 || edit5==edit4 || edit5==edit6 || edit5==edit7)
		{
			m_pbOK->EnableWindow(false);
			m_pbApply->EnableWindow(false);
			m_AdcGroupFlag = false;
		}
		else
		{
			if(m_TTLinGroupFlag && m_DacGroupFlag &&
				m_TTLoutGroupFlag && m_AdcRateFlag)
			{
				m_pbApply->EnableWindow(true);
				m_pbOK->EnableWindow(true);	
			}
			m_AdcGroupFlag = true;
		}
	else
		if((edit4+edit5+edit6+edit7)==0)
		{
			m_pbOK->EnableWindow(false);
			m_pbApply->EnableWindow(false);
			m_AdcGroupFlag = false;
		}
		else
		{
			if(m_TTLinGroupFlag && m_DacGroupFlag &&
				m_TTLoutGroupFlag && m_AdcRateFlag)
			{
				m_pbApply->EnableWindow(true);
				m_pbOK->EnableWindow(true);	
			}
			m_AdcGroupFlag = true;
		}
}


void CE2010Settings::OnEnChangeEdit6()
{
	CString str;
	WORD edit4,edit5,edit6,edit7;

	GetDlgItem(IDC_EDIT4)->GetWindowTextW(str);
	edit4 = StrToInt(str);
	GetDlgItem(IDC_EDIT5)->GetWindowTextW(str);
	edit5 = StrToInt(str);
	GetDlgItem(IDC_EDIT6)->GetWindowTextW(str);
	edit6 = StrToInt(str);
	GetDlgItem(IDC_EDIT7)->GetWindowTextW(str);
	edit7 = StrToInt(str);

	if(edit6!=0)
		if(edit6>4 || edit6==edit4 || edit6==edit5 || edit6==edit7)
		{
			m_pbOK->EnableWindow(false);
			m_pbApply->EnableWindow(false);
			m_AdcGroupFlag = false;
		}
		else
		{
			if(m_TTLinGroupFlag && m_DacGroupFlag &&
				m_TTLoutGroupFlag && m_AdcRateFlag)
			{
				m_pbApply->EnableWindow(true);
				m_pbOK->EnableWindow(true);	
			}
			m_AdcGroupFlag = true;
		}
	else
		if((edit4+edit5+edit6+edit7)==0)
		{
			m_pbOK->EnableWindow(false);
			m_pbApply->EnableWindow(false);
			m_AdcGroupFlag = false;
		}
		else
		{
			if(m_TTLinGroupFlag && m_DacGroupFlag &&
				m_TTLoutGroupFlag && m_AdcRateFlag)
			{
				m_pbApply->EnableWindow(true);
				m_pbOK->EnableWindow(true);	
			}
			m_AdcGroupFlag = true;
		}
}


void CE2010Settings::OnEnChangeEdit7()
{
	CString str;
	WORD edit4,edit5,edit6,edit7;

	GetDlgItem(IDC_EDIT4)->GetWindowTextW(str);
	edit4 = StrToInt(str);
	GetDlgItem(IDC_EDIT5)->GetWindowTextW(str);
	edit5 = StrToInt(str);
	GetDlgItem(IDC_EDIT6)->GetWindowTextW(str);
	edit6 = StrToInt(str);
	GetDlgItem(IDC_EDIT7)->GetWindowTextW(str);
	edit7 = StrToInt(str);

	if(edit7!=0)
		if(edit7>4 || edit7==edit4 || edit7==edit6 || edit7==edit5)
		{
			m_pbOK->EnableWindow(false);
			m_pbApply->EnableWindow(false);
			m_AdcGroupFlag = false;
		}
		else
		{
			if(m_TTLinGroupFlag && m_DacGroupFlag &&
				m_TTLoutGroupFlag && m_AdcRateFlag)
			{
				m_pbApply->EnableWindow(true);
				m_pbOK->EnableWindow(true);	
			}
			m_AdcGroupFlag = true;
		}
	else
		if((edit4+edit5+edit6+edit7)==0)
		{
			m_pbOK->EnableWindow(false);
			m_pbApply->EnableWindow(false);
			m_AdcGroupFlag = false;
		}
		else
		{
			if(m_TTLinGroupFlag && m_DacGroupFlag &&
				m_TTLoutGroupFlag && m_AdcRateFlag)
			{
				m_pbApply->EnableWindow(true);
				m_pbOK->EnableWindow(true);	
			}
			m_AdcGroupFlag = true;
		}
}


void CE2010Settings::OnEnChangeEdit8()
{
	CString str;
	WORD edit;
	GetDlgItem(IDC_EDIT8)->GetWindowTextW(str);
	edit = StrToInt(str);
	

	if (edit==0 || edit>10000)
	{
		m_pbOK->EnableWindow(false);
		m_pbApply->EnableWindow(false);
		m_AdcRateFlag = false;
	}
	else
	{
		if(m_TTLinGroupFlag && m_DacGroupFlag &&
				m_TTLoutGroupFlag && m_AdcGroupFlag)
		{
			m_pbApply->EnableWindow();
			m_pbOK->EnableWindow(true);
		}
		m_AdcRateFlag = true;
	}
}


void CE2010Settings::OnEnChangeEdit9()
{
	CString str;
	GetDlgItem(IDC_EDIT9)->GetWindowTextW(str);
	WORD edit = StrToInt(str);

	if(edit==1 || edit==2) 
	{
		m_DacGroupFlag = true;
		if(m_TTLinGroupFlag && m_AdcRateFlag &&
			m_TTLoutGroupFlag && m_AdcGroupFlag)
		{
			m_pbApply->EnableWindow(true);
			m_pbOK->EnableWindow(true);
		}
	}
	else 
	{
		m_DacGroupFlag = false;
		m_pbOK->EnableWindow(false);
		m_pbApply->EnableWindow(false);
	}
}
