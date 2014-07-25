#include "stdafx.h"
#include "windows.h"
#include "math.h"
#include "Resource.h"
#include "Chopper.h"
#include "SpectrDlg.h"

/// Pulse signal for cmAFG, cmDDS, cmSWEEP
CPulseParams pulse = {
	+5.00 V,		// amplitude
	0.00 V,		// offset
	10.00 us,		// period 10 us
	-0.02 x01_09,	// leading edge, rise time 200 ns (relative to period duration)
	-0.50,			// {-1..0}: duty cycle 50%, relative to period duration (length 5 us)
	-0.01 x01_09	// trailing edge, fall time 100 ns (relative to period duration)
};



CChopper::CChopper(void)// конструктор класса
{
	slot = -1;

	// читаем параметры из inifile
	m_genFrequencyStep = double(GetPrivateProfileInt(_T("Chopper"),_T("Frequency step"),NULL,CSpectrDlg::m_IniFileName));
	m_TimeDelay = GetPrivateProfileInt(_T("Chopper"),_T("Time delay"),NULL,CSpectrDlg::m_IniFileName);
	m_InverseStepResolution = GetPrivateProfileInt(_T("Chopper"),_T("Inverse step resolution"),NULL,CSpectrDlg::m_IniFileName);
	m_WindowsRotorNumber = GetPrivateProfileInt(_T("Chopper"),_T("Rotor windows number"),NULL,CSpectrDlg::m_IniFileName);
	m_GearRatio = GetPrivateProfileInt(_T("Chopper"),_T("Gear Ratio"),NULL,CSpectrDlg::m_IniFileName);
	m_modCarrentFrequency = 0;
}
	// конструктор класса c передачей слота
CChopper::CChopper(int sl = -1)
	:slot(sl)
{
	// читаем параметры из inifile
	m_genFrequencyStep = double(GetPrivateProfileInt(_T("Chopper"),_T("Frequency step"),NULL,CSpectrDlg::m_IniFileName));
	m_TimeDelay = GetPrivateProfileInt(_T("Chopper"),_T("Time delay"),NULL,CSpectrDlg::m_IniFileName);
	m_InverseStepResolution = GetPrivateProfileInt(_T("Chopper"),_T("Inverse step resolution"),NULL,CSpectrDlg::m_IniFileName);
	m_WindowsRotorNumber = GetPrivateProfileInt(_T("Chopper"),_T("Rotor windows number"),NULL,CSpectrDlg::m_IniFileName);
	m_GearRatio = GetPrivateProfileInt(_T("Chopper"),_T("Gear Ratio"),NULL,CSpectrDlg::m_IniFileName);
	m_modCarrentFrequency = 0;
}


CChopper::~CChopper(void)//виртуальный деструктор
{
}


// функция разгона и остановки прерывателя 
int CChopper::SetFrequency(int modFrequency)
{
	double realfreq(0.0);
	double genCarrentFrequency(ModToGenFrequency(m_modCarrentFrequency));
	double genFrequency(ModToGenFrequency(modFrequency));
	
	if(m_modCarrentFrequency<modFrequency)
	{
		genCarrentFrequency=genCarrentFrequency+m_genFrequencyStep;
		while(genCarrentFrequency<genFrequency)
		{
			while(CSpectrDlg::m_PressBn == PAUSE) Sleep(100); // если нажата пауза
			if(CSpectrDlg::m_PressBn == STOP) return -1; // если нажата стоп выходим из функции
			//меняем частоту
			if(b132_chan_set_ddsfreq( slot, CH_A, genCarrentFrequency, &realfreq) != NO_ERRORS ) return 9;
			Sleep(m_TimeDelay);
			genCarrentFrequency+=m_genFrequencyStep;
		}
		m_modCarrentFrequency=modFrequency;
		return NO_ERRORS;
	}
	if(modFrequency==0)
	{
		if(b132_chan_set_ddsfreq( slot, CH_A, 0.0, &realfreq) != NO_ERRORS ) return 9;
		m_modCarrentFrequency=modFrequency;
		return NO_ERRORS;
	}
	else
	{
		genCarrentFrequency=genCarrentFrequency-m_genFrequencyStep;
		while(genCarrentFrequency>genFrequency)
		{
			while(CSpectrDlg::m_PressBn == PAUSE) Sleep(100); // если нажата пауза
			if(CSpectrDlg::m_PressBn == STOP) return -1; // если нажата стоп выходим из функции
			// меняем частоту
			if(b132_chan_set_ddsfreq( slot, CH_A, genCarrentFrequency, &realfreq) != NO_ERRORS ) return 9;
			Sleep(m_TimeDelay);
			genCarrentFrequency-=m_genFrequencyStep;
		}
		m_modCarrentFrequency=modFrequency;	
	}
	return NO_ERRORS;
}


int CChopper::Stop(void)
{
	int err(0);
	if( ( b132_chan_stop( slot, CH_AB, 0 )  != NO_ERRORS) ||
		( b132_chan_get_started( slot, CH_AB )  != 0 )) err = 11;

	if( b132_chan_enable( slot, CH_AB, 0 ) != NO_ERRORS )  
		if(err==0) err = 6;
	if( (err = b132_device_unlock( slot ))!= NO_ERRORS) 
		if(err==0) err = 13;
	return err;
}

int CChopper::SignalDataInit(int mode, double signal_length, float **ppData, int *pSamples, double *pTimeBase )
// int mode					{ cmAFG,cmDDS,cmSWEEP,cmAM,cmFM }
// double signal_length		period/duration, s
// float **pData			pointer to first signal data sample pointer
// int *pSamples			required signal data samples count for given signal duration
// int *pTimeBase			sample hold time (sample rate reciprocal)
{
	if( ppData == NULL ) return ERR_PARAM;
	int err;
	if( err = b132_device_get_timing( mode, signal_length, pSamples, pTimeBase ) != NO_ERRORS ) return err;
	if( *ppData != NULL )
		delete [] *ppData;
	*ppData = new float[*pSamples];
	return NO_ERRORS;
}
int CChopper::SignalDataDeinit( float **ppData )
{
	if( ppData == NULL ) return ERR_PARAM;
	if( *ppData != NULL ) {
		delete [] *ppData;
		*ppData = NULL;
	}
	return NO_ERRORS;
}


int CChopper::Initialise()
{
	int count(-1),ser(0), mode(-1), k(0);
	double realfreq(0.0);
	// проверяем параметры settings.ini (не должно быть нулей)
	if(m_genFrequencyStep*m_TimeDelay*m_InverseStepResolution*m_WindowsRotorNumber*m_GearRatio==0.0) return 10;
	count = b132_device_count( );
	if( count <= 0 ) return 1;
	while (1)
	{
		if( b132_device_info( ++k, &slot, &ser ) < 0 ) break;
	}
	slot = b132_device_lock( 0 ); // see {ID_IDX,ID_SER,ID_SLOT} description in b132drv.h
	if( slot < 0 ) return  2;
	b132_device_set_mode( slot, cmDDS );
	if ( b132_device_get_mode( slot ) != cmDDS ) return 3;
	//Initialize generator channels 
	if( (b132_chan_set_range( slot, CH_AB, RANGE_8V ) ) != NO_ERRORS ) return 4;
	if( (b132_chan_set_filter( slot, CH_A, FLT_OFF ) ) != NO_ERRORS ||
		(b132_chan_set_filter( slot, CH_B, FLT_OFF ) ) != NO_ERRORS ) return 5;
	if( (b132_chan_enable( slot, CH_A, 1 ) ) != NO_ERRORS ) return 6;
	if( (b132_chan_set_pulse_mod( slot, CH_A, 0, pulse.offset != NO_ERRORS ) ) ||
		(b132_chan_set_pulse_mod( slot, CH_B, 0, pulse.offset != NO_ERRORS ) ) ) return 7;

	
	int samples;
	double timebase;
	float *pData; pData = NULL;
	SignalDataInit( cmDDS, pulse.frequency, &pData, &samples, &timebase );
	b132_fill_pulse_data( pulse, &pData, samples );
	b132_chan_set_data( slot, CH_A, &pData, samples, timebase );
	SignalDataDeinit( &pData );
	//установим 0-ю частоту
	if(b132_chan_set_ddsfreq( slot, CH_A, 0.0, &realfreq) != NO_ERRORS ) return 9;
	//if(b132_chan_set_ddsfreq( slot, CH_B, 1.0, &realfreq) != NO_ERRORS ) return 9;
	//if( ( err = b132_chan_set_trig( slot, CH_A, 0 us, 1 us, TRIG_USER ) ) != NO_ERRORS ) { AfxMessageBox(_T("Can't set trigger output")); return-1;}
	if( (b132_chan_start( slot, CH_A, START_MODE_SOFT_PER ) ) != NO_ERRORS ) return 8;

	return NO_ERRORS;
}
