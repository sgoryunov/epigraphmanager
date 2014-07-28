#pragma once
#include "afxwin.h"
#include "resource.h"		// основные символы

// диалоговое окно СTaskFileDlg

class СTaskFileDlg : public CDialog
{
	DECLARE_DYNAMIC(СTaskFileDlg)

public:
	СTaskFileDlg(CWnd* pParent = NULL);   // стандартный конструктор
	virtual ~СTaskFileDlg();

// Данные диалогового окна
	enum { IDD = IDD_TASKFILE };

protected:
	

	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV
	
	
public:
	CString m_TaskFileName;
	CEdit m_EditOpen;
	CListBox m_listedit;
	CButton m_bOpen;
	CButton m_bOK;
	CButton m_bSave;
	CEdit m_EditChopper;
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedOpentaskfile();
	afx_msg void OnBnClickedInsert();
	afx_msg void OnBnClickedRemove();
	afx_msg void OnBnClickedReplace();
	afx_msg void OnBnClickedSave();
	afx_msg void OnOK();
//	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnGeneratetasklistScan();
	afx_msg void OnGeneratetasklistFourier();
//	afx_msg void OnLbnSetfocusList1();
	afx_msg void OnLbnSelchangeList1();
};
