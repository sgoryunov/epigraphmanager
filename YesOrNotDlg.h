#pragma once
//#include "resource.h"

// ���������� ���� YesOrNotDlg

class CYesOrNotDlg : public CDialog
{
	DECLARE_DYNAMIC(CYesOrNotDlg)

public:
	CYesOrNotDlg(CWnd* pParent = NULL);   // ����������� �����������
	virtual ~CYesOrNotDlg();

// ������ ����������� ����
	enum { IDD = IDD_DIALOG3 };

protected:
	CStatic m_AttenStatic;
	virtual void DoDataExchange(CDataExchange* pDX);    // ��������� DDX/DDV

	DECLARE_MESSAGE_MAP()

};
