#ifndef __AFGIOINTF_H__
#define __AFGIOINTF_H__
#include "windows.h"
#include "..\..\common\include\unilptx.h"

// Generator modes
const AgentModesCount = 6;
typedef BYTE AFGMode;
const AFGMode cmAFG	= 0; // Base mode (sample-by-sample)
const AFGMode cmDDS = 1; // Direct Digital Synthesis mode
const AFGMode cmSweep = 2; // Sweep Generator
	const MinSweepSamplesCount = 3; // Min number of sweep frequency steps
	const MaxSweepSamplesCount = 0xFFFFFFFF; // Max number of sweep frequency steps (only for min ShiftCode(=1) and max SweepRange)
	const MaxSweepShiftCode = 0xFFFFFFFF; // Max sweep frequency step code

const AFGMode cmAM = 3; // Amplitude Modulation mode
	const ammAmplitude = 0; // Amplitude Modulation submode (base frequency with side frequencies)
	const ammBalance = 1;	// Balance Modulation submode (only side frequencies without base frequency)

const AFGMode cmFrM = 4; // Frequency Modulation mode
#define FMSubModesCount 4
	const double FMMaxFreq[FMSubModesCount] = {25e6/(1<<1), 25e6/(1<<6), 25e6/(1<<11), 25e6/(1<<16)};

const AFGMode cmPhM = 5; // Phase Modulation mode
#define PMSubModesCount 4
	const double PMMaxPhase[PMSubModesCount] = {180/(1<<1), 180/(1<<6), 180/(1<<11), 180/(1<<16)};

// Device
#define AFGChannelsCount 2
#define AFGFiltersCount 4
const double AFGClockFrequency = 100e6; // in cmAFG, cmDDS
const double AFGDDSFrequency = 50e6; // in cmSweep, cmAM, cmFrM, cmPhM

// DAC
#define AFGDACRangesCount 2
const double AFGVoltsMax[AFGDACRangesCount] = {2.048, 8.192};
const WORD AFGDACMaximalCode = 0xFFFF;

// ADC
#define AFGADCRangesCount 2
const double AFGADCRange[AFGADCRangesCount] = {10.0, 5.0}; // Implemented only +/-10 Volts Input Range
const WORD AFGADCMaximalCode = 0xFFFF;
const double AFGADCLevel = 0.0;
const double AFGADCCoef = 1.0;

// Syncronization
const WORD MaxSyncLength = 0xFFFF;

#ifdef __cplusplus
extern "C" {
#endif

typedef struct tagAFGChannelIOControlInfo
{
	WORD			m_nADCCode;			/* OUT, DEAD */
	// ADC Code, currently not used

	BYTE			m_nDGTicksPerCLK;	/* IN */
	// Always =200 Delay Generator Steps Count per AFGClock Period (10 ns)

	BOOL			m_bEnabled;			/* IN */
	// Defines the state of channel output relay : TRUE - On / FALSE - Off
	BOOL			m_bPowerDown;		/* IN */
	// Channel PowerDown Control

	BYTE			m_nMode;			/* IN */
	BYTE			m_nSubMode;			/* IN */
	// Mode selection values (see Generator Modes section above)
	// Must be equal for all channels - different channel modes are not supported by hardware

	double			m_fDDSFrequency;	/* IN */
	// Signal frequency in all DDS-based modes (excluding cmAFG)

	double			m_fDDSShift;		/* IN */
	// Signal frequency step in cmSweep

	double			m_fDDSTime;			/* IN */
	// Sweep period

	BOOL			m_bVoltageRange;	/* IN */
	// Output voltage range (FALSE - 2V, TRUE - 8V)

	WORD			*m_pData;			/* IN */
	// Channel data pointer
	DWORD			m_dwDataSize;		/* IN */
	// Channel data size (in Samples, i.e. in WORDs)

	BOOL			m_bIsRunning;		/* IN, OUT */
	// Indicates channel state

	BOOL			m_bExternalStart;		/* IM */
	// Selects input triger source (FALSE - computer start, TRUE - external trigger start)

	BOOL			m_bExternalStartFront;	/* IN */
	// External trigger slope: FALSE - передний, TRUE - задний

	BOOL			m_bAutoRepeat;			/* IN */
	// Signal period cycling (FALSE - transient, single shoot; TRUE - repetitive, periodical)

	BOOL			m_bExternalClock;		/* IN, DEAD */
	// Using external clock source, not supported by hardware

	DWORD			m_dwSampleTime;			/* IN */
	// cmAFG only. Defines each sample holding time in AFGClock periods (0-10ns,1-20ns,2-30ns,etc)

	BYTE			m_nFilter;				/* IN */
	// Number of filter used for generation
	// 0 - 50 MHz Tchebyshev's filter (recommended for sine)
	// 1 - off (filter is not used) (recommended for pulse signals)
	// 2 - 5 MHz first-order filter (R-C)
	// 3 - 500 kHz first-order filter (R-C)

	// Output triger settings
	BOOL			m_bOutSyncEnabled;		/* IN */
	// Output trigger enable from given channel

	double			m_fOutSyncStart;		/* IN */
	// Output trigger rising edge position (relative to given channel's first signal sample)

	WORD			m_nOutSyncLength;		/* IN */
	// Output trigger length in AFGClock periods (10ns step)

	BYTE			m_nOutSyncMode;
	// Output trigger falling slope mode:
	// 0 - fixed trigger length 20 ns,
	// 1 - fixed trigger length 100 ns
	// 2 - user defined position of output trigger falling slope, by 10 ns step

	BOOL			m_bExtPulseModulation;	/* IN */
	// External pulse modulation (manipulation) by input trigger

	WORD			m_nCurrentZeroCode;	/* IN */
	// DAC code corresponding to output zero volts level for current output range and current filter
	
} CAFGChannelIOControlInfo;

typedef struct tagAFGDeviceInfo
{
	CAFGChannelIOControlInfo ChannelsInfo[AFGChannelsCount];

	BOOL			m_bPowerDown;			/* IN */
	// Device PowerDown Control (including ADC and output triger blocks)

	BOOL			m_bInputRange;			/* IN, DEAD */
	// ADC input voltage range (FALSE - 10.0V, TRUE - 5.0V)

	BOOL			m_bOutToADC;			/* IN */
	// FALSE - ADC input from external connector (ADC channel 3)
	// TRUE  - ADC input from one of the channel output selected by m_bChannelToADC (see below)

	BOOL			m_bChannelToADC;		/* IN */
	// TRUE  - Channel A
	// FALSE - Channel B

	CDeviceID		m_DeviceID;			/* IN */
	// Device index from UniLPTX.dll

	LPCTSTR			m_pResourceName;
	// PLD resource name 

	WORD			ADCCode;
	// ADC code obtained during IOCTL_GETADCCODE

} CAFGDeviceInfo;

#ifdef __cplusplus
}
#endif


//////////////////////////////////////////////////////////////////////////////

extern void __stdcall ClearDev(CDeviceID* pDeviceID);

//////////////////////////////////////////////////////////////////////////////

extern void __stdcall SetInitAddress(CDeviceID* pDeviceID, int ChannelIndex);

//////////////////////////////////////////////////////////////////////////////

extern void __stdcall Run(CDeviceID* pDeviceID, int ChannelIndex, BOOL State);
// State: TRUE - Run, FALSE - Stop.

//////////////////////////////////////////////////////////////////////////////

extern BOOL __stdcall IOControl(int ChannelIndex, DWORD IoControlCode, void* pParams);

//////////////////////////////////////////////////////////////////////////////
// pParams
// pointer to CAFGDeviceInfo structure

//////////////////////////////////////////////////////////////////////////////
// ChannelIndex
// 0 - Channel A
// 1 - Channel B
//-1 - Both A and B

//////////////////////////////////////////////////////////////////////////////
// Available IoControlCode values

// Hardware reset
#define IOCTL_CLEAR				0x000000

// Complete hardware update
#define IOCTL_UPDATE			0x000001

// Channel output ON / OFF
#define IOCTL_ENABLED			0x000002

// Applying output voltage range
#define IOCTL_VOLTAGERANGE		0x000004

// Aplying PWDN mode for chanels
#define IOCTL_PWDN				0x000008

// Updating output trigger settings
#define IOCTL_SYNCOUT			0x000010

// Internal/external start trigger source
#define IOCTL_RUNEXT			0x000020

// Appliyng AutoRepeat (single shoot or repetitive)
#define IOCTL_RUNMODE			0x000040

// Internal/external clock source /* DEAD */
#define IOCTL_CLOCKTYPE			0x000080

// Applying filter changing
#define IOCTL_FILTER			0x000200

// Updating sample holding time
#define IOCTL_SAMPLETIME		0x000400

// Updating signal data
#define IOCTL_DATA				0x000800

// Enabling/disabling output trigger
#define IOCTL_SYNCOUTENABLED	0x001000

// Connecting channel selected by m_bChannelToADC (below) to ADC input
#define IOCTL_OUTTOADC			0x002000

// Selecting the channel to be connected to ADC input (above)
#define IOCTL_CHANNELTOADC		0x004000

// PLD programming
#define IOCTL_PLDPROG			0x008000

// Setting hardware independed demo mode (no device)
#define IOCTL_DEMO				0x010000

// Get ADC code
#define IOCTL_GETADCCODE		0x020000

// ADC input range setting /* DEAD */
#define IOCTL_INRANGE			0x040000

// Updates all DDS-based mode parameters (excluding signal data)
#define IOCTL_SWEEP				0x080000

// Submode setting
#define IOCTL_SUBMODE			0x100000

// Setting external pulse modulation
#define IOCTL_EXTMODULATION		0x200000

#endif //__AFGIOINTF_H__