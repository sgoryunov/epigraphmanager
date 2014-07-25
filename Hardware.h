#pragma once
#include "_include\b132drv.h" // ��������� �332
#include "_include\Lusbapi.h" // ��� �20-10 
#pragma comment (lib,"..\\_Lib\\b132drv.lib")
#pragma comment (lib,"..\\_Lib\\lusbapi.lib")

class CHardware
{
public:
	CHardware(void);// �����������
	~CHardware(void);// ����������
	
	CString B332Init(void);
	CString E2010Init(void);
	CString UsbSpeed(void);
	
private:
	// ��������� �� ��������� ������
	ILE2010 *pModule;
	//�������� ������ ���������� Lusbapi
	DWORD DllVersion;

	
};
