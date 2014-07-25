// ScanTaskDlg.cpp: файл реализации
//

#include "stdafx.h"
#include "Spectr.h"
#include "ScanTaskDlg.h"
#include "afxdialogex.h"
#include "MyDefines.h"


// диалоговое окно CScanTaskDlg

IMPLEMENT_DYNAMIC(CScanTaskDlg, CDialog)

CScanTaskDlg::CScanTaskDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CScanTaskDlg::IDD, pParent)
{

}

CScanTaskDlg::~CScanTaskDlg()
{
}

void CScanTaskDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_bOK);
}


BEGIN_MESSAGE_MAP(CScanTaskDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CScanTaskDlg::OnBnClickedOk)
END_MESSAGE_MAP()




BOOL CScanTaskDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	if(m_dlgID == FOURIER_TASK)
	{
		this->SetWindowText(_T("Fourier tasklist"));
		GetDlgItem(IDC_STATIC1)->SetWindowText(_T("Modulation frequency"));
		GetDlgItem(IDC_STATIC2)->SetWindowText(_T("Hz"));
		GetDlgItem(IDC_STATIC3)->SetWindowText(_T("Distance bw filters"));
		GetDlgItem(IDC_STATIC4)->SetWindowText(_T("mm"));
	}
	
	// TODO:  Äîáàâèòü äîïîëíèòåëüíóþ èíèöèàëèçàöèþ

	return TRUE;  // return TRUE unless you set the focus to a control
	// Èñêëþ÷åíèå: ñòðàíèöà ñâîéñòâ OCX äîëæíà âîçâðàùàòü çíà÷åíèå FALSE
}


void CScanTaskDlg::OnBnClickedOk()
{
	CString text[6] = {NULL};
	CString ftext(_T(""));
	WORD initPos(0),finitPos(0),stepPos(0);
	CDialog * parent = (CDialog *)GetParent(); 
	CButton * bSave = (CButton *)parent->GetDlgItem(IDC_SAVE);
	CButton * bOK = (CButton *)parent->GetDlgItem(IDOK);
	CListBox * taskList = (CListBox*)parent->GetDlgItem(IDC_LIST1);
	bool isAllWndsEmpty(false);

	GetDlgItem(IDC_EDIT1)->GetWindowText(text[0]);
	initPos = StrToInt(text[0]);
	GetDlgItem(IDC_EDIT2)->GetWindowText(text[1]);
	finitPos = StrToInt(text[1]);
	GetDlgItem(IDC_EDIT3)->GetWindowText(text[2]);
	stepPos = StrToInt(text[2]);
	GetDlgItem(IDC_EDIT4)->GetWindowText(text[3]);// meas. time
	GetDlgItem(IDC_EDIT5)->GetWindowText(text[4]);
	GetDlgItem(IDC_EDIT6)->GetWindowText(text[5]);// grating freq
	if (Filter(text))
	{

		for (WORD i(initPos);i<=finitPos;i+=stepPos)
		{
			text[0].Format(_T("%i"),i);
			ftext=(_T(" Chopper = "))+text[0]+(_T(" HSMotor = "))+text[5]+(_T(" Carriage Position = "))+text[4]+(_T(" Measurement Time = "))+text[3];
			taskList->AddString(ftext);
		}
		bSave->EnableWindow(true); // активируем кнопку save
		bOK->EnableWindow(false); // дизактивируем кнопку ОК
		CDialog::OnOK();
	}
	else AfxMessageBox(_T("Don't define measurement parameters or some parameters incorect!"));
}


bool CScanTaskDlg::Filter(CString text[])
{

	for (WORD i(0);i<6;i++)
		if (StrToInt(text[i])<0) return false;
	WORD val(StrToInt(text[0]));
	for (WORD i(0);i<3;i++) val *= StrToInt(text[i]);
	if(val==0) return false;
	if((StrToInt(text[1])-StrToInt(text[0]))<StrToInt(text[2])) 
		return false;
	if(StrToInt(text[3])<20) return false;
	if (m_dlgID==FOURIER_TASK)
	{
		if(StrToInt(text[4])==0 || text[5].IsEmpty()) return false;
	}
	else
		if(text[4].IsEmpty() || text[5].IsEmpty()) return false;			
	
	
	return true;
}
