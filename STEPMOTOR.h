#pragma once


// диалоговое окно CSTEPMOTOR

class CSTEPMOTOR : public CPropertyPage
{
	DECLARE_DYNAMIC(CSTEPMOTOR)

public:
	CSTEPMOTOR();
	virtual ~CSTEPMOTOR();

// Данные диалогового окна
	enum { IDD = IDD_SPAGE1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
};
