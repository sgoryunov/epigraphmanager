// YesOrNotDlg.cpp: файл реализации
//

#include "stdafx.h"
#include "Spectr.h"
#include "YesOrNotDlg.h"
#include "afxdialogex.h"


// диалоговое окно YesOrNotDlg

IMPLEMENT_DYNAMIC(CYesOrNotDlg, CDialog)

CYesOrNotDlg::CYesOrNotDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CYesOrNotDlg::IDD, pParent)
{

}

CYesOrNotDlg::~CYesOrNotDlg()
{
}

void CYesOrNotDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC3, m_AttenStatic);
	m_AttenStatic.SetIcon(::LoadIconW(NULL,IDI_WARNING));
}


BEGIN_MESSAGE_MAP(CYesOrNotDlg, CDialog)
END_MESSAGE_MAP()


// обработчики сообщений YesOrNotDlg
