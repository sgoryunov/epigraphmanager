// SplashDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Spectr.h"
#include "SplashDlg.h"
#include"Hardware.h"


// CSplashDlg dialog

IMPLEMENT_DYNAMIC(CSplashDlg, CDialog)

CSplashDlg::CSplashDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSplashDlg::IDD, pParent)
{

}

CSplashDlg::~CSplashDlg()
{
}

void CSplashDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSplashDlg, CDialog)
	ON_WM_TIMER()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CSplashDlg message handlers

void CSplashDlg::ShowSplashScreen(CWnd* pParentWnd)
{
	
	// Allocate a new splash screen, and create the window.
		c_pSplashDlg = new CSplashDlg;
	if (!c_pSplashDlg->Create(CSplashDlg::IDD, pParentWnd))
			delete c_pSplashDlg;
	else
	c_pSplashDlg->ShowWindow(SW_SHOW);
	c_pSplashDlg->UpdateWindow();

 c_pSplashDlg->SetTimer(1,5000, NULL);
}

void CSplashDlg::HideSplashScreen(void)
{
	// Destroy the window, and update the mainframe.
	c_pSplashDlg->KillTimer(1);
	DestroyWindow();
	AfxGetMainWnd()->UpdateWindow();
	delete c_pSplashDlg;
	c_pSplashDlg = NULL;
}

void CSplashDlg::OnTimer(UINT_PTR nIDEvent)
{
	

// Destroy the splash screen window.
	HideSplashScreen();

}

BOOL CSplashDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CenterWindow();
	
	 SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0,
      SWP_NOMOVE|SWP_NOSIZE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSplashDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CHardware hardware;

	SetDlgItemText(IDC_STATIC1,hardware.B332Init());
	SetDlgItemText(IDC_STATIC3,hardware.E2010Init());
	SetDlgItemText(IDC_STATIC2,hardware.UsbSpeed());

//	CString usbsout;
//	BYTE isUsbSpeed=hardware.UsbSpeed();
//	if(isUsbSpeed==3){(_T(" GetUsbSpeed()... Bad"));} else
//	{(isUsbSpeed) ? (usbsout= ("USB is in: High-Speed Mode (480 Mbit/s)")):(usbsout=("USB is in: Full-Speed Mode (12 Mbit/s)"));}
//	SetDlgItemText(IDC_STATIC3,usbsout);
	
}
