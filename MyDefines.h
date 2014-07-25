// My Definetions
// CE2010 
//#define CHANNELS_QUANTITY			(0x4)



// CSpectrDlg значения индикатора кнопок
#define PAUSE    1
#define STOP     2


// TaskArray 
#define CHOPPER_FREQUENCY				  0
#define HSM_FREQUENCY					  1
#define CARRIAGE_POSITION				  2
#define MEASUREMENT_TIME				  3




// Settings Parametrs
#define CHOPPER_FREQUENCY_STEP             0
#define CHOPPER_TIME_DELAY                 1
#define CHOPPER_INVERSE_STEP_RESOLUTION    2
#define CHOPPER_ROTOR_WINDOWS_NUMBER       3
#define CHOPPER_GEAR_RATIO                 4
#define HSM_FREQUENCY_RANGE                5
#define SPS_NUMBER_OF_DEVISIONS			   6
#define SPS_CALIBRATION                    7
#define SPS_MIN_DISTANCE_BW_FILTERS        8
#define SPS_CARRIAGE_WORK_RANGE            9
#define E2010_TURBINE_SIGNAL_inPIN          10
#define E2010_SPS_STEPPER_SIGNAL_outPIN     11
#define E2010_SPS_STEPPER_DIRACTION_outPIN	12
#define E2010_CHOPPER_CHANNEL				13
#define E2010_HSM_CHANNEL					14
#define E2010_MONITOR_CHANNEL				15
#define E2010_DETECTOR_CHANNEL				16
#define E2010_ADC_RATE						17
#define SPS_SIGNAL_SYSTEM					18
#define E2010_HSM_CNTL_CHANNEL				19
#define HSM_REACTION_TIME					20
#define SPS_TIME_DELAY						21
#define SPS_CURRENT_CARRIAGE_POS			22


//CStepMotorSettings
#define DIR_SYSTEM							0
#define UP_DOWN_SYSTEM						1


#define NO_ERRORS	0

// B132 interface (Generator)
#define V		*1		///< units visualization: voltage (dummy)
#define mV		* 1e-3	///< units visualization: voltage multiplier constant
#define s				///< units visualization: time (dummy)
#define ms		* 1e-3  ///< units visualization: time multiplier constant
#define msec	* 1e-3
#define us		* 1e-6	///< units visualization: time multiplier constant
#define ns		* 1e-9	///< units visualization: time multiplier constant
#define Hz				///< units visualization: frequency (dummy)
#define kHz		* 1e+3	///< units visualization: frequency multiplier constant
#define MHz		* 1e+6	///< units visualization: frequency multiplier constant
#define x03_07	* 2.50	///< pulse signal rise/fall time correction for measuring levels 30%-70%
#define x01_09	* 1.25	///< pulse signal rise/fall time correction for measuring levels 10%-90%

// для класса CGraphDlg
#define BAR				0
#define DATA_POINT		1
#define SCAN_GRAPH		2
#define OSCIL_GRAPH		3

// мои сообщения
#define MyM_DRAW_GRAPH					WM_APP+1

// CHighSpeedMotor
#define MIN_DAC_VALUE   -2048
#define MAX_DAC_VALUE   2047

// для класса CScanTaskDlg
#define SCAN_TASK			0
#define FOURIER_TASK		1