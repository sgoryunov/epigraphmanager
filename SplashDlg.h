#pragma once


// CSplashDlg dialog

class CSplashDlg : public CDialog
{
	DECLARE_DYNAMIC(CSplashDlg)

public:
	CSplashDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSplashDlg();
	static CSplashDlg* c_pSplashDlg;
// Dialog Data
	enum { IDD = IDD_SPLASH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	static void ShowSplashScreen(CWnd* pParentWnd);
public:
	void HideSplashScreen(void);
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnPaint();
};
