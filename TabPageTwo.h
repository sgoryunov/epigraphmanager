#pragma once


// CTabPageTwo dialog

class CTabPageTwo : public CDialog
{
	DECLARE_DYNAMIC(CTabPageTwo)

public:
	CTabPageTwo(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTabPageTwo();

// Dialog Data
	enum { IDD = IDD_DIALOG_PAGE2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
