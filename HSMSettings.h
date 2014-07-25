#pragma once


// ���������� ���� CHSMSettings

class CHSMSettings : public CPropertyPage
{
	DECLARE_DYNAMIC(CHSMSettings)

public:
	CHSMSettings();
	virtual ~CHSMSettings();
	CEdit m_EditFrequencyRange;

// ������ ����������� ����
	enum { IDD = IDD_HSMSET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // ��������� DDX/DDV
	CButton* m_pbOK;
	CButton* m_pbApply;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdit1();
	virtual BOOL OnSetActive();
	afx_msg void OnEnChangeEdit2();
};
