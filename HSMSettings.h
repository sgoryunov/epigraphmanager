#pragma once


// диалоговое окно CHSMSettings

class CHSMSettings : public CPropertyPage
{
	DECLARE_DYNAMIC(CHSMSettings)

public:
	CHSMSettings();
	virtual ~CHSMSettings();
	CEdit m_EditFrequencyRange;

// Данные диалогового окна
	enum { IDD = IDD_HSMSET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV
	CButton* m_pbOK;
	CButton* m_pbApply;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdit1();
	virtual BOOL OnSetActive();
	afx_msg void OnEnChangeEdit2();
};
