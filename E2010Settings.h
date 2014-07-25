#pragma once


// диалоговое окно CE2010Settings

class CE2010Settings : public CPropertyPage
{
	DECLARE_DYNAMIC(CE2010Settings)

public:
	CE2010Settings();
	virtual ~CE2010Settings();
	CEdit m_EditTurbineSignalInPin;
	CEdit m_EditSPSStepperSignalOutPin;
	CEdit m_EditSPSStepperDiractionOutPin;

// Данные диалогового окна
	enum { IDD = IDD_E2010SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV
	CButton* m_pbOK;
	CButton* m_pbApply;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnEnChangeEdit3();
	virtual BOOL OnSetActive();
	afx_msg void OnEnChangeEdit4();
	afx_msg void OnEnChangeEdit5();
	afx_msg void OnEnChangeEdit6();
	afx_msg void OnEnChangeEdit7();

	afx_msg void OnEnChangeEdit8();
private:
	CString m_EditText;
	bool m_AdcGroupFlag;
	bool m_DacGroupFlag;
	bool m_TTLinGroupFlag;
	bool m_TTLoutGroupFlag;
	bool m_AdcRateFlag;
public:
	afx_msg void OnEnChangeEdit9();
};
