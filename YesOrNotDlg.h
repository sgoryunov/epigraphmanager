#pragma once
//#include "resource.h"

// диалоговое окно YesOrNotDlg

class CYesOrNotDlg : public CDialog
{
	DECLARE_DYNAMIC(CYesOrNotDlg)

public:
	CYesOrNotDlg(CWnd* pParent = NULL);   // стандартный конструктор
	virtual ~CYesOrNotDlg();

// Данные диалогового окна
	enum { IDD = IDD_DIALOG3 };

protected:
	CStatic m_AttenStatic;
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()

};
