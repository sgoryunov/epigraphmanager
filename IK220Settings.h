#pragma once


// ���������� ���� CIK220Settings

class CIK220Settings : public CPropertyPage
{
	DECLARE_DYNAMIC(CIK220Settings)

public:
	CIK220Settings();
	virtual ~CIK220Settings();

// ������ ����������� ����
	enum { IDD = IDD_IK220SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // ��������� DDX/DDV

	DECLARE_MESSAGE_MAP()
};
