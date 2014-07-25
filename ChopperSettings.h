#pragma once

// диалоговое окно CChopperSettings

class CChopperSettings : public CPropertyPage
{
	DECLARE_DYNAMIC(CChopperSettings)

public:
	CChopperSettings();
	virtual ~CChopperSettings();
	
	
// Данные диалогового окна
	enum { IDD = IDD_CHOPPERSET };

	CEdit m_EditFrequencyStep;
	CEdit m_EditTimeDelay;
	CEdit m_EditInverseStepResolution;
	CEdit m_EditRotorWindowsNumber;
	CEdit m_EditGearRatio;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV
	CButton* m_pbOK;
	CButton* m_pbApply;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnEnChangeEdit3();
	afx_msg void OnEnChangeEdit4();
	afx_msg void OnEnChangeEdit5();
//	virtual void CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType = adjustBorder);
//	virtual BOOL OnSetActive();
	virtual BOOL OnSetActive();
	virtual BOOL OnInitDialog();
};
