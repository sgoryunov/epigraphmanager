// IK220Settings.cpp: файл реализации
//

#include "stdafx.h"
#include "Spectr.h"
#include "IK220Settings.h"
#include "afxdialogex.h"


// диалоговое окно CIK220Settings

IMPLEMENT_DYNAMIC(CIK220Settings, CPropertyPage)

CIK220Settings::CIK220Settings()
	: CPropertyPage(CIK220Settings::IDD)
{

}

CIK220Settings::~CIK220Settings()
{
}

void CIK220Settings::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CIK220Settings, CPropertyPage)
END_MESSAGE_MAP()


// обработчики сообщений CIK220Settings
