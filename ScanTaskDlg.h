#pragma once


// ���������� ���� CScanTaskDlg

class CScanTaskDlg : public CDialog
{
	DECLARE_DYNAMIC(CScanTaskDlg)

public:
	CScanTaskDlg(CWnd* pParent = NULL);   // ����������� �����������
	virtual ~CScanTaskDlg();

// ������ ����������� ����
	enum { IDD = IDD_SCANTASKDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // ��������� DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	CButton m_bOK;
	virtual BOOL OnInitDialog();
	void SetDlgType(BYTE id)
	{ m_dlgID = id;};
	afx_msg void OnBnClickedOk();

private:
	BYTE m_dlgID;
	bool Filter(CString []);

};
