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
	// ф-я установки частоты вращения решетки
	int SetFrequency(short frequency);
	// ф-я принятия решения об перестройки частоты
	static int SendMeasFrequency(double);
private:
	// идентификат файла для чтения/записи
	ifstream m_InClientFile;
	static CString m_FreqDacTableFileName;
	// рабочая частота
	static double m_OperatingFrequency;
	// сумма измеренных частот
	static double m_AvrMeasFrequency;
	// шаг Дака
	static short m_DacStep;
	// текущее значение Дака
	static short m_CurDacData;
	static double m_FrequencyRange;
	static WORD m_Channel;
	// счетчик измеренных частот 
	static WORD m_FrequencyCounter;
	static WORD m_Error;
	static WORD m_FreqMeasNumber;
	static WORD m_ReactionTime;
	static bool m_IsAdjustingFreq;
	void SetDacDataForFreq(short freq);
	// Фунция записи данных в таблицу
	static bool WriteFreqDacTable(double freq, short dacData);
};

