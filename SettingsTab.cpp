// SettingsTab.cpp : implementation file
//

#include "stdafx.h"
#include "Spectr.h"
#include "SettingsTab.h"


// SettingsTab dialog

IMPLEMENT_DYNAMIC(SettingsTab, CDialog)

SettingsTab::SettingsTab(CWnd* pParent /*=NULL*/)
	: CDialog(SettingsTab::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    pPageOne = new CTabPageOne;
	pPageTwo = new CTabPageTwo;

}

SettingsTab::~SettingsTab()
{
}

void SettingsTab::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_TabCtrl);
}


BEGIN_MESSAGE_MAP(SettingsTab, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &SettingsTab::OnTcnSelchangeTab1)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB1, &SettingsTab::OnTcnSelchangingTab1)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// SettingsTab message handlers

void SettingsTab::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	int iTab = m_TabCtrl.GetCurSel();
	TC_ITEM tci;
	tci.mask = TCIF_PARAM;
	m_TabCtrl.GetItem(iTab, &tci);
	CWnd* pWnd = (CWnd *)tci.lParam;
	pWnd->ShowWindow(SW_SHOW); 
	*pResult = 0;
}

void SettingsTab::OnTcnSelchangingTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	int iTab = m_TabCtrl.GetCurSel();
	TC_ITEM tci;
	tci.mask = TCIF_PARAM;
	m_TabCtrl.GetItem(iTab, &tci);
	CWnd* pWnd = (CWnd *)tci.lParam;
	pWnd->ShowWindow(SW_HIDE); 
	*pResult = 0;
}

BOOL SettingsTab::OnInitDialog()
{
	CDialog::OnInitDialog();
	TC_ITEM tci; 
	tci.mask = TCIF_TEXT;
	tci.iImage = -1; 
	tci.pszText = _T("One");
	m_TabCtrl.InsertItem(0, &tci); 
	tci.pszText = _T("Two"); 
	m_TabCtrl.InsertItem(1, &tci);
	tci.mask = TCIF_PARAM;
	tci.lParam = (LPARAM)pPageOne;
	m_TabCtrl.SetItem(0, &tci);
	VERIFY(pPageOne->Create(CTabPageOne::IDD, &m_TabCtrl));
	pPageOne->SetWindowPos(NULL, 30, 30, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	pPageOne->ShowWindow(SW_SHOW);
	tci.mask = TCIF_PARAM;
	tci.lParam = (LPARAM)pPageTwo;
	m_TabCtrl.SetItem(1, &tci);
	pPageTwo->Create(CTabPageTwo::IDD, &m_TabCtrl);
	pPageTwo->SetWindowPos(NULL, 30, 30, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	pPageTwo->ShowWindow(SW_HIDE);
	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


