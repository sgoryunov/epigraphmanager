#pragma once
#include "afxwin.h"


// диалоговое окно CStepMotorSettings

class CStepMotorSettings : public CPropertyPage
{
	DECLARE_DYNAMIC(CStepMotorSettings)

public:
	CStepMotorSettings();
	virtual ~CStepMotorSettings();
	CEdit m_EditNumberOfDevisions;
	CEdit m_EditCalibration;
	CEdit m_EditMinDistanceBwFilters;
	CEdit m_EditCarriageWorkRange;
	int m_SignalSystem;
// Данные диалогового окна
	enum { IDD = IDD_STEPMOTORSET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV
	CButton* m_pbOK;
	CButton* m_pbApply;
	CToolTipCtrl m_tt;
	DECLARE_MESSAGE_MAP()
private:
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnEnChangeEdit3();
	afx_msg void OnEnChangeEdit4();
	virtual BOOL OnSetActive();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
public:
	afx_msg void OnEnChangeEdit6();
	afx_msg void OnEnChangeEdit5();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
