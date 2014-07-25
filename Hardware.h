#pragma once
#include "_include\b132drv.h" // генератор Б332
#include "_include\Lusbapi.h" // АЦП Е20-10 
#pragma comment (lib,"..\\_Lib\\b132drv.lib")
#pragma comment (lib,"..\\_Lib\\lusbapi.lib")

class CHardware
{
public:
	CHardware(void);// конструктор
	~CHardware(void);// деструктор
	
	CString B332Init(void);
	CString E2010Init(void);
	CString UsbSpeed(void);
	
private:
	// указатель на интерфейс модуля
	ILE2010 *pModule;
	//проверка версии библиотеки Lusbapi
	DWORD DllVersion;

	
};
