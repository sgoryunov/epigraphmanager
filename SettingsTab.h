#pragma once
#include "afxcmn.h"
#include "TabPageOne.h"
#include "TabPageTwo.h"

// SettingsTab dialog

class SettingsTab : public CDialog
{
	DECLARE_DYNAMIC(SettingsTab)

public:
	SettingsTab(CWnd* pParent = NULL);   // standard constructor
	virtual ~SettingsTab();

// Dialog Data
	enum { IDD = IDD_Settings };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnTcnSelchangingTab1(NMHDR *pNMHDR, LRESULT *pResult);
public:
	CTabCtrl m_TabCtrl;
	// Реализация
protected:
	HICON m_hIcon;
	CTabPageOne * pPageOne;
	CTabPageTwo * pPageTwo;

public:
	virtual BOOL OnInitDialog();

};
