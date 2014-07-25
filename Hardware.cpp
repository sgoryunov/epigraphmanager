#include "StdAfx.h"
#include "Hardware.h"


CHardware::CHardware(void)
{
	// ������� ��������� �� ���������
	pModule = NULL;
	// ���� �� ������ ����� ����� ������
	//hReadThread = NULL;
}

CHardware::~CHardware(void)
{
}
//-------������������� ���������� B332----------------------------	
CString CHardware::B332Init(void)
{
	CString sout=_T("B-332 generator found ..."),tsout;
	int count=b132_device_count();
	if (count<=0)
	{
		tsout=_T("none");
		sout+=tsout;
	}
	else
	{
		tsout.Format(_T("%d"),count);
		sout+=tsout;
	}
	return sout;
}

CString CHardware::UsbSpeed(void)
{	
	CString sout = _T("0");
	WORD i;
	// �������� ������ ���� USB
	BYTE UsbSpeed;
	pModule = static_cast<ILE2010 *>(CreateLInstance("e2010"));
	if(!pModule) return (_T(" Module Interface... Bad"));
	// ��������� ���������� ������ E20-10 � ������ MAX_VIRTUAL_SLOtS_QUANTITY_LUSBAPI ����������� ������
	for(i = 0x0; i < MAX_VIRTUAL_SLOTS_QUANTITY_LUSBAPI; i++) if(pModule->OpenLDevice(i)) break;
	// ���-������ ����������?
	if(i == MAX_VIRTUAL_SLOTS_QUANTITY_LUSBAPI) return _T(" Can't find any module E20-10 in first 127 virtual slots!");
	// ������� �������� USB
	if (!pModule->GetUsbSpeed(&UsbSpeed)) sout = (_T(" GetUsbSpeed()... Bad"));
	else 
	{ 
		(UsbSpeed) ? sout = _T("USB is in: High-Speed Mode (480 Mbit/s)"):
		sout = _T("USB is in: Full-Speed Mode (12 Mbit/s)");
	}
	//��������� ���������� ����
	pModule->CloseLDevice();
	// ��������� ��������� ������
	if(pModule)
	{
		// ��������� ��������� ������
		if(!pModule->ReleaseLInstance()) return (_T("Can't free E2010 Module Interface"));
		// ������� ��������� �� ��������� ������
		pModule = NULL;
	}
	return _T("Modul E2010 is found!   ")+sout;
}

//-------������������� ACP E2010----------------------------
CString CHardware::E2010Init(void)
{
	if((DllVersion = GetDllVersion()) != CURRENT_VERSION_LUSBAPI)
	{
		return (_T("Lusbapi.dll Version Error!!!"));
	}

	return (_T(" Lusbapi.dll Version --> OK"));
}