#pragma once
#include "e2010.h"
#include <fstream>

using std::fstream;
using std::ifstream;
using std::ios;

class CHighSpeedMotor :
	private CE2010
{
public:
	CHighSpeedMotor(void);
	virtual ~CHighSpeedMotor(void);
	// �-� ��������� ������� �������� �������
	int SetFrequency(short frequency);
	// �-� �������� ������� �� ����������� �������
	static int SendMeasFrequency(double);
private:
	// ����������� ����� ��� ������/������
	ifstream m_InClientFile;
	static CString m_FreqDacTableFileName;
	// ������� �������
	static double m_OperatingFrequency;
	// ����� ���������� ������
	static double m_AvrMeasFrequency;
	// ��� ����
	static short m_DacStep;
	// ������� �������� ����
	static short m_CurDacData;
	static double m_FrequencyRange;
	static WORD m_Channel;
	// ������� ���������� ������ 
	static WORD m_FrequencyCounter;
	static WORD m_Error;
	static WORD m_FreqMeasNumber;
	static WORD m_ReactionTime;
	static bool m_IsAdjustingFreq;
	void SetDacDataForFreq(short freq);
	// ������ ������ ������ � �������
	static bool WriteFreqDacTable(double freq, short dacData);
};

