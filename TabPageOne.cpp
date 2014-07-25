// TabPageOne.cpp : implementation file
//

#include "stdafx.h"
#include "Spectr.h"
#include "TabPageOne.h"


// CTabPageOne dialog

IMPLEMENT_DYNAMIC(CTabPageOne, CDialog)

CTabPageOne::CTabPageOne(CWnd* pParent /*=NULL*/)
	: CDialog(CTabPageOne::IDD, pParent)
{

}

CTabPageOne::~CTabPageOne()
{
}

void CTabPageOne::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTabPageOne, CDialog)
END_MESSAGE_MAP()


// CTabPageOne message handlers
