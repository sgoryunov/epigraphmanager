//------------------------------------------------------------------------------
#ifndef __LusbapiTypesH__
#define __LusbapiTypesH__
//------------------------------------------------------------------------------

	#include <windows.h>
	//
	#ifndef NAME_LINE_LENGTH_LUSBAPI
		#define NAME_LINE_LENGTH_LUSBAPI 25
	#endif
	#ifndef COMMENT_LINE_LENGTH_LUSBAPI
		#define COMMENT_LINE_LENGTH_LUSBAPI 256
	#endif
	#ifndef ADC_CALIBR_COEFS_QUANTITY_LUSBAPI
		#define ADC_CALIBR_COEFS_QUANTITY_LUSBAPI 128
	#endif
	#ifndef DAC_CALIBR_COEFS_QUANTITY_LUSBAPI
		#define DAC_CALIBR_COEFS_QUANTITY_LUSBAPI 128
	#endif

	// ������������ ��������
	#pragma pack(1)

	// ��������� � ����������� ������� �� ����/����� ������
	struct IO_REQUEST_LUSBAPI
	{
		SHORT * Buffer;							// ��������� �� ����� ������
		DWORD   NumberOfWordsToPass;			// ���-�� ��������, ������� ��������� ��������
		DWORD   NumberOfWordsPassed;			// �������� ���-�� ���������� ��������
		OVERLAPPED * Overlapped;				// ��� ������������� ������� - ��������� �� ��������� ���� OVERLAPPED
		DWORD   TimeOut;							// ��� ����������� ������� - ������� � ��
	};

	// ��������� � ����������� �� ��������� ������ ���������� ����������
	struct LAST_ERROR_INFO_LUSBAPI
	{
		BYTE	ErrorString[256];					// ������ � ������� ��������� ��������� ������
		DWORD	ErrorNumber;	  					// ����� ��������� ������
	};

	// ���������� � ��, ���������� � ��������������� ����������: MCU, DSP, PLD � �.�.
	struct VERSION_INFO_LUSBAPI
	{
		BYTE 	Version[10];					  	// ������ �� ��� ���������������� ����������
		BYTE 	Date[14];						  	// ���� ������ ��
		BYTE 	Manufacturer[NAME_LINE_LENGTH_LUSBAPI]; 	// ������������� ��
		BYTE 	Author[NAME_LINE_LENGTH_LUSBAPI];		 	// ����� ��
		BYTE	Comment[COMMENT_LINE_LENGTH_LUSBAPI];		// ������ �����������
	};

	// ���������� � �� MCU, ������� �������� � ���� ���������� � ���������
	// ��� �������� ���������, ��� � ����������
	struct MCU_VERSION_INFO_LUSBAPI
	{
		VERSION_INFO_LUSBAPI FwVersion;						// ���������� � ������ �������� �������� ��������� '����������'(Application) ����������������
		VERSION_INFO_LUSBAPI BlVersion;						// ���������� � ������ �������� '����������'(BootLoader) ����������������
	};

	// ����� ���������� � ������ (������� �������)
	struct MODULE_INFO_LUSBAPI
	{
		BYTE	CompanyName[NAME_LINE_LENGTH_LUSBAPI];		// �������� �����-������������ �������
		BYTE	DeviceName[NAME_LINE_LENGTH_LUSBAPI]; 		// �������� �������
		BYTE	SerialNumber[16];									// �������� ����� �������
		BYTE	Revision;											// ������� ������� (��������� ������)
		BYTE	Modification;										// ���������� ������ (�����);
		BYTE	Comment[COMMENT_LINE_LENGTH_LUSBAPI];		// ������ �����������
	};

	// ����� ���������� � ������ (� �������������� ����� Modification)
/*	struct MODULE_INFO_M_LUSBAPI
	{
		BYTE	CompanyName[NAME_LINE_LENGTH_LUSBAPI];		// �������� �����-������������ �������
		BYTE	DeviceName[NAME_LINE_LENGTH_LUSBAPI]; 		// �������� �������
		BYTE	SerialNumber[16];									// �������� ����� �������
		BYTE	Revision;											// ������� ������� (��������� ������)
		BYTE	Modification;										// ���������� ������ (�����);
		BYTE	Comment[COMMENT_LINE_LENGTH_LUSBAPI];		// ������ �����������
	};*/

	// ���������� � DSP
	struct DSP_INFO_LUSBAPI
	{
		BOOL	Active;												// ���� ������������� ��������� ����� ���������
		BYTE	Name[NAME_LINE_LENGTH_LUSBAPI];				// �������� DSP
		double	ClockRate;										// �������� ������� ������ DSP � ���
		VERSION_INFO_LUSBAPI Version;							// ���������� � �������� DSP
		BYTE	Comment[COMMENT_LINE_LENGTH_LUSBAPI];		// ������ �����������
	};

	// ���������� � ����������������
	template <class VersionType>
	struct MCU_INFO_LUSBAPI
	{
		BOOL	Active;												// ���� ������������� ��������� ����� ���������
		BYTE	Name[NAME_LINE_LENGTH_LUSBAPI];				// �������� ����������������
		double	ClockRate;										// �������� ������� ������ ���������������� � ���
//		VERSION_INFO_LUSBAPI Version;							// ���������� � ������ �������� ����������������
		VersionType Version;										// ���������� � ������ ��� ����� �������� ����������������, ���, ��������, � �������� '����������'
		BYTE	Comment[COMMENT_LINE_LENGTH_LUSBAPI];		// ������ �����������
	};

	// ���������� � ���� (PLD)
	struct PLD_INFO_LUSBAPI										// PLD - Programmable Logic Device
	{
		BOOL	Active;												// ���� ������������� ��������� ����� ���������
		BYTE	Name[NAME_LINE_LENGTH_LUSBAPI];		  		// �������� ����
		double ClockRate;											// �������� ������ ������ ���� � ���
		VERSION_INFO_LUSBAPI Version;							// ���������� � ������ �������� ����
		BYTE	Comment[COMMENT_LINE_LENGTH_LUSBAPI];		// ������ �����������
	};

	// ���������� � ���
	struct ADC_INFO_LUSBAPI
	{
		BOOL	Active;												// ���� ������������� ��������� ����� ���������
		BYTE	Name[NAME_LINE_LENGTH_LUSBAPI];				// �������� ���������� ���
		double	OffsetCalibration[ADC_CALIBR_COEFS_QUANTITY_LUSBAPI];	// ���������������� ������������ �������� ����
		double	ScaleCalibration[ADC_CALIBR_COEFS_QUANTITY_LUSBAPI];		// ���������������� ������������ ��������
		BYTE	Comment[COMMENT_LINE_LENGTH_LUSBAPI];		// ������ �����������
	};

	// ���������� � ���
	struct DAC_INFO_LUSBAPI
	{
		BOOL	Active;												// ���� ������������� ��������� ����� ���������
		BYTE	Name[NAME_LINE_LENGTH_LUSBAPI];				// �������� ���������� ���
		double	OffsetCalibration[DAC_CALIBR_COEFS_QUANTITY_LUSBAPI];	// ���������������� ������������
		double	ScaleCalibration[DAC_CALIBR_COEFS_QUANTITY_LUSBAPI];		// ���������������� ������������
		BYTE	Comment[COMMENT_LINE_LENGTH_LUSBAPI];		// ������ �����������
	};

	// ���������� � ��������� �����-������
	struct DIGITAL_IO_INFO_LUSBAPI
	{
		BOOL	Active;												// ���� ������������� ��������� ����� ���������
		BYTE	Name[NAME_LINE_LENGTH_LUSBAPI];				// �������� �������� ����������
		WORD	InLinesQuantity;	 								// ���-�� ������� �����
		WORD	OutLinesQuantity; 								// ���-�� �������� �����
		BYTE	Comment[COMMENT_LINE_LENGTH_LUSBAPI];		// ������ �����������
	};

	// ���������� � ������������� ���������� ��� ������� � ������
	struct INTERFACE_INFO_LUSBAPI
	{
		BOOL	Active;												// ���� ������������� ��������� ����� ���������
		BYTE	Name[NAME_LINE_LENGTH_LUSBAPI];			 	// ��������
		BYTE	Comment[COMMENT_LINE_LENGTH_LUSBAPI];		// ������ �����������
	};
	// ��������� ������� ModulatFreqCalc()

	//const double pi = 3.1415926535897932384626433832795;
    struct MOD_FREQ_CALC_PAR
	{
		// 1 �����
		long TickIn1;
		long AddTime1;
		long Step1;
		short lastAmp1;
		// 2 �����
		long TickIn2;
		long AddTime2;
		long Step2;
		short lastAmp2;
		// 3 �����
		long TickIn3;
		long AddTime3;
		long Step3;
		short lastAmp3;
	};
	//
	#pragma pack()

#endif
