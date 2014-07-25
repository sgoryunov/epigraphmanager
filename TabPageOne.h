#pragma once


// CTabPageOne dialog

class CTabPageOne : public CDialog
{
	DECLARE_DYNAMIC(CTabPageOne)

public:
	CTabPageOne(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTabPageOne();

// Dialog Data
	enum { IDD = IDD_DIALOG_PAGE1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
