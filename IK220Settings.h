#pragma once


// диалоговое окно CIK220Settings

class CIK220Settings : public CPropertyPage
{
	DECLARE_DYNAMIC(CIK220Settings)

public:
	CIK220Settings();
	virtual ~CIK220Settings();

// Данные диалогового окна
	enum { IDD = IDD_IK220SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
};
