#pragma once


// ���������� ���� CSTEPMOTOR

class CSTEPMOTOR : public CPropertyPage
{
	DECLARE_DYNAMIC(CSTEPMOTOR)

public:
	CSTEPMOTOR();
	virtual ~CSTEPMOTOR();

// ������ ����������� ����
	enum { IDD = IDD_SPAGE1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // ��������� DDX/DDV

	DECLARE_MESSAGE_MAP()
};
