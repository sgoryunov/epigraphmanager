// STEPMOTOR.cpp: ���� ����������
//

#include "stdafx.h"
#include "Spectr.h"
#include "STEPMOTOR.h"
#include "afxdialogex.h"


// ���������� ���� CSTEPMOTOR

IMPLEMENT_DYNAMIC(CSTEPMOTOR, CPropertyPage)

CSTEPMOTOR::CSTEPMOTOR()
	: CPropertyPage(CSTEPMOTOR::IDD)
{

}

CSTEPMOTOR::~CSTEPMOTOR()
{
}

void CSTEPMOTOR::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSTEPMOTOR, CPropertyPage)
END_MESSAGE_MAP()


// ����������� ��������� CSTEPMOTOR
