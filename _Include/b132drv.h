//////////////////////////////////////////////////////////////////////////
// B-132 UNIPRO ARBITRARY FUNCTION GENERATOR DRIVER, (c) AURIS, 2008

//////////////////////////////////////////////////////////////////////////
/**	\defgroup b132_const B-132 DRIVER CONSTANTS
*	@{ */

	/** \defgroup b132_const_id DEVICE ID
	*	Modifier specify what kind of deviceID is requested. Constants should be used in: b132_device_info(), b132_device_lock().
	*	@{ */

#define ID_IDX	0x00000000	///< ID is available found order, (default, if omitted)
#define ID_SER	0x00010000	///< ID is device serial number
#define ID_SLOT	0x00020000	///< ID is device interface slot number

	/**	@} */ //b132_const_id

	/** \defgroup b132_const_mode DEVICE MODES
	*	Device mode constants should be used in: b132_device_set_mode(), b132_device_get_timing()
	*	@{ */

#define cmAFG	0	///< 100 MSPS standard sample-by-sample generation (independed channels, precise samples timing)
#define cmDDS	1	///< 100 MSPS DDS (direct digital synthesis) generator (independed channels, precise frequency tune)
#define cmSWEEP	2	///< 50 MSPS DDS-based sweep generator (independed channels)
#define cmAM	3	///< 50 MSPS DDS-based AM (amplitude modulation): CH_A - carrier (CH_CAR), CH_B - modulation (CH_MOD)
#define cmFM	4	///< 50 MSPS DDS-based FM (frequency modulation): CH_A - carrier (CH_CAR), CH_B - modulation (CH_MOD)
#define cmPM	5	///< 50 MSPS DDS-based PM (phase modulation): CH_A - carrier (CH_CAR), CH_B - modulation (CH_MOD)
#define cmADC	6	///< 100 kHz 4-channel ADC ±10V (TODO:_NOT_SUPPORTED_IN_THIS_VERSION_)

	/** @} */ //b132_const_mode

	/** \defgroup b132_const_chan CHANNEL
	*	Channel settings constants should be used in: \ref b132_func_chan
	*	@{ */

#define CH_AB		0	///< CHAN: both A and B channels
#define CH_A		1	///< CHAN: channel A
#define CH_B		2	///< CHAN: channel B

		/** \defgroup b132_const_chan_limits LIMITS
		*	Channel filter constants should be used in: b132_chan_set_filter()
		*	@{ */

#define MAX_FREQ	2e+7	///< max supported output signal frequency, Hz
#define MEM_SIZE	65536	///< channel memory size, samples

		/** @} */ //b132_const_chan_limits

		/** \defgroup b132_const_chan_filter FILTER
		*	Channel filter constants should be used in: b132_chan_set_filter()
		*	@{ */

#define	FLT_SIN		0	///< CHAN FILTER: 50 MHz, sine-optimized
#define	FLT_OFF		1	///< CHAN FILTER: off, pulse-optimized
#define	FLT_5M		2	///< CHAN FILTER: 5 MHz 1st-order (R-C)
#define	FLT_500K	3	///< CHAN FILTER: 500 kHz 1st-order (R-C)

		/** @} */ //b132_const_chan_filter

		/** \defgroup b132_const_chan_range RANGE
		*	Channel output range constants should be used in: b132_chan_set_range()
		*	@{ */

#define RANGE_2V	0	///< CHAN RANGE: output range (-2..+2)V
#define RANGE_8V	1	///< CHAN RANGE: output range (-8..+8)V

		/** @} */ //b132_const_chan_range

		/** \defgroup b132_const_chan_start START
		*	Channel start delay constants is param range in b132_chan_set_start_delay(...delay...)
		*	@{ */

#define START_DELAY_MIN			0	///< CHAN START DELAY: min value
#define START_DELAY_MAX			40	///< CHAN START DELAY: max value

			/** \defgroup b132_const_chan_start_mode MODES
			*	Channel start mode constants should be used in b132_chan_set_start_mode(), b132_chan_start()
			*	@{ */

#define START_MODE_CURRENT		0	///< CHAN START MODE: previous mode (no changes)
#define START_MODE_SOFT_PER		1	///< CHAN START MODE: immediate start, periodic
#define START_MODE_SOFT_SGL		2	///< CHAN START MODE: (@ cmAFG) immediate start, single shot
#define START_MODE_EXTRISE_PER	3	///< CHAN START MODE: (@ cmAFG) wait for trigger input rise, periodic
#define START_MODE_EXTFALL_PER	4	///< CHAN START MODE: (@ cmAFG) wait for trigger input fall, periodic
#define START_MODE_EXTRISE_SGL	5	///< CHAN START MODE: (@ cmAFG) wait for trigger input rise, single shot
#define START_MODE_EXTFALL_SGL	6	///< CHAN START MODE: (@ cmAFG) wait for trigger input fall, single shot

			/** @} */ //b132_const_chan_start_mode
		/** @} */ //b132_const_chan_start

		/** \defgroup b132_const_chan_clbr CALIBRATION
		*	Channel calibration modes constants should be used in b132_chan_calibrate()
		*	@{ */

#define CLBR_INT_RANGE_ALL	0	///< internal calibration, all output voltage ranges
#define CLBR_INT_RANGE_CUR	1	///< internal calibration, current output voltage range
#define CLBR_EXT_RANGE_ALL	2	///< external calibration, all output voltage ranges
#define CLBR_EXT_RANGE_CUR	3	///< external calibration, current output voltage range

		/** @} */ //b132_const_chan_clbr


		/** \defgroup b132_const_chan_sweep SWEEP
		*	Sweep settings constants sholud be used in: b132_chan_set_sweep()
		*	@{ */

#define SWEEP_RANGE_MIN		1		///< min supported frequency sweep range, Hz
#define SWEEP_PERIOD_MIN	1e-6	///< min supported frequency sweep time, s
#define SWEEP_STEPS_MIN		3		///< min number of sweep frequency points
#define SWEEP_STEPS_AUTO	0		///< smooth frequency sweep (max possible frequency points)
									///< WARNING: range error is possible (a lot of steps + step truncation error)

		/** @} */ //b132_const_chan_sweep

		/** \defgroup b132_const_chan_modulation MODULATION
		*	Modulation settings constants sholud be used in: b132_chan_set_modulating()
		*	@{ */
			/** \defgroup b132_const_chan_modulation_chan CHANNELS ACRONYMS
			*	Channel acronyms should be used in b132_chan_set_ddsfreq() when modulation is used
			*	@{ */

#define CH_CAR	CH_A	///< cmAM, cmFM, cmPM: Channel for carrier signal
#define CH_MOD	CH_B	///< cmAM, cmFM, cmPM: Channel for modulating signal

			/** @} */ //b132_const_chan_modulation_chan

#define BM		0		///< Balance   modulation = carrier * modulation)
#define AM		1		///< Amplitude modulation = carrier * (1 + modulation)
#define FM0		0		///< FM range 0 = 65535/65536*(25 MHz)			=  ~25,0 MHz
#define FM1		1		///< FM range 1 = 65535/65536*(25 MHz)/32		= ~781,2 kHz
#define FM2		2		///< FM range 2 = 65535/65536*(25 MHz)/32/32	=  ~24,4 kHz
#define FM3		3		///< FM range 3 = 65535/65536*(25 MHz)/32/32/32	= ~762,9 Hz
#define PM0		0		///< PM range 0 = 65535/65536*(360)		= ~360
#define PM1		1		///< PM range 1 = 65535/65536*(360)/2	= ~180
#define PM2		2		///< PM range 2 = 65535/65536*(360)/4	= ~90
#define PM3		3		///< PM range 3 = 65535/65536*(360)/8	= ~45

		/** @} */ //b132_const_chan_modulation

	/** @} */ //b132_const_chan

	/** \defgroup b132_const_trig_len TRIGGER OUTPUT
	*	Trigger length modes constants should be used in b132_chan_set_trig()
	*	@{ */

#define TRIG_USER		0	///< user-defined length, falling edge position step ± (current timebase)
#define TRIG_FIX20		1	///< 20 ns fixed length, falling edge moves with rising edge
#define TRIG_FIX100		2	///< 100 ns fixed length, falling edge moves with rising edge

	/** @} */ //b132_const_trig_len

	/**	\defgroup b132_const_adc ADC
	*	ADC channels acronyms should be used in b132_adc_get_voltage()
	*	@{ */

#define ADC_CH_1		1	///< ADC input 1 / generator CH_A (if channel is enabled)
#define ADC_CH_2		2	///< ADC input 2 / generator CH_B (if channel is enabled)
#define ADC_CH_3		3	///< ADC input 3
#define ADC_CH_4		4	///< ADC input 4 @ cmADC (TODO:_NOT_SUPPORTED_IN_THIS_VERSION_)

	/** @} */ //b132_const_adc

	/**	\defgroup b132_const_error RETURN ERROR CODES
	*	@{ */

#define NO_ERRORS		 0	///< request complete with no errors
#define ERR_UNILPTX		-1	///< UNIPRO interface is not initialized (device is not connected or device power off)
#define ERR_NO_DEVICE	-2	///< device is not found in given slot (or has not compatible version), use slot = b132_device_lock(...)
#define ERR_SETMODE		-3	///< generator is not initialized: set mode with b132_device_set_mode( {cmAFG,cmDDS,cmSWEEP,cmAM,cmFM} )
#define ERR_NO_SUPPORT	-4	///< requested operation is not supported in current mode
#define ERR_PARAM		-5	///< invalid param (int or ptr) value
#define ERR_VALUE		-6	///< invalid param (double) value: out of supported range
#define ERR_FAILED		-7	///< command or request failed
#define ERR_CLBRDATA	-8	///< couldn't read eeprom data in b132_lock_device( )
#define ERR_NO_DATA		-9	///< signal data was not set (nothing to generate)

	/**	@} */ //b132_const_error
/**	@} */ //b132_const


//////////////////////////////////////////////////////////////////////////
/**	\defgroup b132_func B-132 DRIVER FUNCTIONS
*	@{ */

/// dll test, always returns 0x0000B132
extern "C" int b132_test();

	//////////////////////////////////////////////////////////////////////////
	/**	\defgroup b132_func_device DEVICE FUNCTIONS
	*	@{ */

///	Number of available B-132 devices
/**	\return
*	- {-}:\ref ERR_UNILPTX - UNIPRO interface is not initialized (device is not connected or device power off)
*	- {+}: found (not locked) devices count
*/
extern "C" int b132_device_count();


/// Look for B-132 device with given ID, return *pSlot and *pSer if found
/**	\return
*	- \ref ERR_PARAM		- check: ID, pSlot, pSer
*	- \ref ERR_UNILPTX		- UNIPRO interface is not initialized (device is not connected or device power off)
*	- \ref ERR_NO_DEVICE	- ID-matched generator is not found
*	- \ref NO_ERRORS		- all done, ID-matched generator slot and serial returned in *pSlot and *pSer
*/
extern "C" int b132_device_info(
	int ID,				///< [in] device index (0..65535) with one of modifiers: \ref ID_IDX, \ref ID_SER, \ref ID_SLOT
	int *pSlot,			///< [out] device slot number (if found)
	int *pSer			///< [out] device serial number (if found)
	);

/// Lock requested B-132 device or the first available (if ID=0)
/** \return
*	- {0..3}				- locked device slot number, using: slot = b132_device_lock(...)
*	- \ref ERR_PARAM		- check: ID
*	- \ref ERR_UNILPTX		- UNIPRO interface is not initialized (device is not connected or device power off)
*	- \ref ERR_NO_DEVICE	- ID-matched generator is not found or is locked (lock-lock will fail, lock-unlock-lock will succeed)
*	- \ref ERR_CLBRDATA	- generator ADC calibration data read failed or data has invalid values (EEPROM data broken)
*/
extern "C" int b132_device_lock(
	int ID				///< [in] device index (0..65535) with one of modifiers: \ref ID_IDX, \ref ID_SER, \ref ID_SLOT
	);

/// Unlock previously locked B-132 device in requested slot number
/** \return
*	- \ref ERR_PARAM		- check: slot
*	- \ref ERR_NO_DEVICE	- generator is not found in given slot
*	- \ref NO_ERRORS		- unlock done or generator was not locked
*/
extern "C" int b132_device_unlock(
	int slot			///< [in] is returned by b132_device_lock()
	);

/// Set and init generator mode
/** \return
*	- \ref ERR_PARAM		- check: slot, mode
*	- \ref ERR_NO_SUPPORT	- mode is cmADC //(TODO:_NOT_SUPPORTED_IN_THIS_VERSION_)
*	- \ref ERR_FAILED		- mode initialization failed
*	- \ref ERR_UNILPTX, \ref ERR_NO_DEVICE
*	- \ref NO_ERRORS
*/
extern "C" int b132_device_set_mode(
	int slot,			///<[in] is returned by b132_device_lock()
	int mode			///<[in] \ref b132_const_mode "cmAFG,cmDDS,cmSweep,cmAM,cmFM,cmPM,cmADC"
	);

/// Get current generator mode
/** \return
*	- {0,+}					- current mode: \ref b132_const_mode "cmAFG,cmDDS,cmSweep,cmAM,cmFM,cmPM,cmADC"
*	- \ref ERR_PARAM		- check: slot
*	- \ref ERR_UNILPTX		- UNIPRO interface is not initialized ( device is not connected or device power off )
*	- \ref ERR_NO_DEVICE	- generator is not found in given slot, use slot = b132_device_lock(...)
*/
extern "C" int b132_device_get_mode(
	int slot			///< [in] is returned by b132_device_lock()
	);

/// Get mode info: limits for signal data calculation
/** \return
*	- \ref ERR_PARAM		- check: mode, pSamples, pTimeBase
*	- \ref ERR_VALUE		- check: interval
*	- \ref ERR_NO_SUPPORT	- mode is cmADC (TODO:_NOT_SUPPORTED_IN_THIS_VERSION_)
*	- \ref NO_ERRORS		- done, requested values are assigned to *pSamples and *pTimeBase
*/
extern "C" int b132_device_get_timing(
	int mode,			///< [in] requested mode \ref b132_const_mode "cmAFG,cmDDS,cmSweep,cmAM,cmFM,cmPM,cmADC"
	double interval,	///< [in] requested time interval ( signal length / period )
	int *pSamples,		///< [in] nearest possible number of samples / @ cmADC: data block size
	double *pTimeBase	///< [in] nearest possible output sample hold time (sample rate reciprocal) / @ cmADC: sample rate
	);

	/**	@} */ //b132_func_device


	//////////////////////////////////////////////////////////////////////////
	/** \defgroup b132_func_chan CHANNEL FUNCTIONS
	*	@{ */

/// Connect/disconnect channel to/from output.
/** WARNING: It stops generation and sets level ~0V
*	\return
*	- \ref ERR_PARAM - check: slot, chan, enable
*	- \ref ERR_UNILPTX, \ref ERR_NO_DEVICE, \ref ERR_SETMODE
*	- \ref NO_ERRORS
*/
extern "C" int b132_chan_enable(
	int slot,			///< [in] is returned by b132_device_lock()
	int chan,			///< [in] channel: \ref CH_A, \ref CH_B, \ref CH_AB
	int enable			///< [in] channel state: <ul><li>FALSE - disconnected <li>TRUE - drives output </ul>
	);

/// cmAFG,cmDDS,cmSWEEP: Perform channel(s) calibration with connected output load (TODO:_NOT_SUPPORTED_IN_THIS_VERSION_)
/** \return
*/
extern "C" int b132_chan_calibrate(
	int slot,			///< [in] is returned by b132_device_lock()
	int chan,			///< [in] channel: \ref CH_A, \ref CH_B, \ref CH_AB
	int clbrmode,		///< [in] calibration mode:
						///<<ul><li>\ref CLBR_INT_RANGE_CUR, \ref CLBR_INT_RANGE_ALL,
						///<	<li>\ref CLBR_EXT_RANGE_CUR, \ref CLBR_EXT_RANGE_ALL </ul>
	double vmin,		///< [in] load restrictions: min voltage allowed at the channel output
	double vmax			///< [in] load restrictions: max voltage allowed at the channel output
	);

/// Select channel(s) analog filter
/** \return
*	- \ref ERR_PARAM - check: slot, chan, filter
*	- \ref ERR_UNILPTX, \ref ERR_NO_DEVICE, \ref ERR_SETMODE
*	- \ref NO_ERRORS
*/
extern "C" int b132_chan_set_filter(
	int slot,			///< [in] is returned by b132_device_lock()
	int chan,			///< [in] channel: \ref CH_A, \ref CH_B, \ref CH_AB
	int filt			///< [in] \ref FLT_SIN, \ref FLT_OFF, \ref FLT_5M, \ref FLT_500K
	);

/// Set channel(s) output range.
/**	WARNING: It stops generation and sets level ~0V
*	\return
*	- \ref ERR_PARAM - check: slot, chan, range
*	- \ref ERR_UNILPTX, \ref ERR_NO_DEVICE, \ref ERR_SETMODE
*	- \ref NO_ERRORS
*/
extern "C" int b132_chan_set_range(
	int slot,			///< [in] is returned by b132_device_lock()
	int chan,			///< [in] channel: \ref CH_A, \ref CH_B, \ref CH_AB
	int range			///< [in] \ref RANGE_2V, \ref RANGE_8V
	);

/// cmAFG: initialize output, set channel(s) initial voltage level.
/**	WARNING: It overwrites level_when_masked assigned in b132_chan_set_pulse_mod().
*	\return
*	- \ref ERR_PARAM - check: slot, chan
*	- \ref ERR_NO_SUPPORT - current mode is not cmAFG
*	- \ref ERR_UNILPTX, \ref ERR_NO_DEVICE, \ref ERR_SETMODE
*	- \ref NO_ERRORS
*/
extern "C" int b132_chan_set_level(
	int slot,			///< [in] is returned by b132_device_lock()
	int chan,			///< [in] channel: \ref CH_A, \ref CH_B, \ref CH_AB
	double level		///< [in] initial output voltage level
	);

/// Enable/disable output amplitude pulse modulation with external trigger input (TTL)
/**	- WARNING: not supported in {cmAM, cmADC}
*	- WARNING: overwrites initial voltage level defined in b132_chan_set_level()
*
*	\return
*	- \ref ERR_PARAM - check: slot, chan, enable
*	- \ref ERR_NO_SUPPORT - current mode is cmAM or cmADC
*	- \ref ERR_UNILPTX, \ref ERR_NO_DEVICE, \ref ERR_SETMODE
*	- \ref NO_ERRORS
*/
extern "C" int b132_chan_set_pulse_mod(
	int slot,			///< [in] is returned by b132_device_lock()
	int chan,			///< [in] channel: \ref CH_A, \ref CH_B, \ref CH_AB
	int enable,			///< [in] Pulse modulation status: <ul><li>{FALSE} - disable modulation <li>{TRUE} - enable modulation <ol><li>TRIG_IN=HI: signal enabled <li>TRIG_IN=LO: signal masked</ol></ul>
	double mask_level	///< [in] output voltage level when signal is masked
	);

/// cmAFG,cmDDS: Set channel's signal data
/** Samples count and hold time should be obtained from b132_device_get_timing()
*	\return
*	- \ref ERR_PARAM - check: slot, chan, ppData, *ppData, samples, timebase(@ cmAFG)
*	- \ref ERR_NO_SUPPORT - current mode is {cmAM,cmFM,cmPM,cmADC}
*	- \ref ERR_UNILPTX, \ref ERR_NO_DEVICE, \ref ERR_SETMODE
*	- \ref NO_ERRORS
*/
extern "C" int b132_chan_set_data(
	int slot,			///< [in] is returned by b132_device_lock()
	int chan,			///< [in] channel: \ref CH_A, \ref CH_B, \ref CH_AB
	float **ppData,		///< [in] signal data (see available \ref b132_func_signal "signal templates")
	int samples,		///< [in] signal data samples count
	double timebase		///< [in] cmAFG: signal sample hold time
	);

/// Set trigger output pulse params (binding, position, and length)
/**	\return
*	- \ref ERR_PARAM - check: slot, chan, len_mode
*	- \ref ERR_VALUE - check: pos (-20ns min), len (>=0)
*	- \ref ERR_NO_SUPPORT - current mode is {cmADC}
*	- \ref ERR_UNILPTX, \ref ERR_NO_DEVICE, \ref ERR_SETMODE
*	- \ref NO_ERRORS
*/
extern "C" int b132_chan_set_trig(
	int slot,			///< [in] is returned by b132_device_lock()
	int chan,			///< [in] trigger binding:
						///<<ul><li>{0} - trigger output disabled
						///<	<li>{\ref CH_A} - trigger on channel A
						///<	<li>{\ref CH_B} - trigger on channel B </ul>
	double pos,			///< [in] position of trigger pulse, s, from signal period beginning,
						///< (50ps step if timebase=10ns, 10ns step otherwize)
	double len,			///< [in] trigger pulse length, s
	int len_mode		///< [in] trigger pulse length mode: \ref TRIG_USER, \ref TRIG_FIX20, \ref TRIG_FIX100
	);

/// cmAFG: Set additional channel(s) delay after start signal
/**	\return
*	- \ref ERR_PARAM - check: slot, chan, startdelay
*	- \ref ERR_NO_SUPPORT - current device mode is not cmAFG
*	- \ref ERR_UNILPTX, \ref ERR_NO_DEVICE, \ref ERR_SETMODE
*/
extern "C" int b132_chan_set_start_delay(
	int slot,			///< [in] is returned by b132_device_lock()
	int chan,			///< [in] channel: \ref CH_A, \ref CH_B, \ref CH_AB
	double startdelay	///< [in] additional channel start delay [\ref START_DELAY_MIN..\ref START_DELAY_MAX] @ 10 ns step
	);

/// Set channel(s) start mode
/** \return
*	- \ref ERR_PARAM - check: slot, chan, mode
*	- \ref ERR_NO_SUPPORT
*	  - current device mode is cmADC
*	  - current device mode is not cmAFG and start mode is not START_MODE_SOFT_PER
*	- \ref ERR_UNILPTX, \ref ERR_NO_DEVICE, \ref ERR_SETMODE
*	- \ref NO_ERRORS
*/
extern "C" int b132_chan_set_start_mode(
	int slot,			///< [in] is returned by b132_device_lock()
	int chan,			///< [in] channel: \ref CH_A, \ref CH_B, \ref CH_AB
	int startmode		///< [in] start mode:
						///<<ul><li>\ref START_MODE_SOFT_PER, \ref START_MODE_EXTRISE_PER, \ref START_MODE_EXTFALL_PER
						///<	<li>\ref START_MODE_SOFT_SGL, \ref START_MODE_EXTRISE_SGL, \ref START_MODE_EXTFALL_SGL </ul>
	);

/// Start signal generation
/**	\return
*	- \ref ERR_PARAM - check: slot, chan, startmode
*	- \ref ERR_NO_SUPPORT
*	  - current device mode is cmADC
*	  - current device mode is not cmAFG and start mode is not START_MODE_SOFT_PER
*	- \ref ERR_UNILPTX, \ref ERR_NO_DEVICE, \ref ERR_SETMODE
*	- \ref NO_ERRORS
*/
extern "C" int b132_chan_start(
	int slot,			///< [in] is returned by b132_device_lock()
	int chan,			///< [in] channel: \ref CH_A, \ref CH_B, \ref CH_AB
	int startmode		///< [in] start mode:
						///<<ul><li>\ref START_MODE_CURRENT (mode should be already set with b132_chan_set_start_mode()),
						///<	<li>\ref START_MODE_SOFT_PER, \ref START_MODE_EXTRISE_PER, \ref START_MODE_EXTFALL_PER
						///<	<li>\ref START_MODE_SOFT_SGL, \ref START_MODE_EXTRISE_SGL, \ref START_MODE_EXTFALL_SGL </ul>
	);

/// Channels' status
/** \return
*	- \ref ERR_PARAM - check: slot, chan
*	- \ref ERR_NO_SUPPORT - current device mode is cmADC
*	- \ref ERR_UNILPTX, \ref ERR_NO_DEVICE, \ref ERR_SETMODE
*	- {0} - both channels A and B are stopped
*	- {1} - channel A busy (generates signal)
*	- {2} - channel B busy (generates signal)
*	- {3} - both channels A and B are busy
*/
extern "C" int b132_chan_get_started(
	int slot,			///< [in] is returned by b132_device_lock()
	int chan			///< [in] channel: \ref CH_A, \ref CH_B, \ref CH_AB
	);

/// Stop signal generation
/** \return
*	- \ref ERR_PARAM - check: slot, chan, wait
*	- \ref ERR_NO_SUPPORT
*	  - current device mode is cmADC
*	  - current device mode is not cmAFG and wait=TRUE
*	- \ref ERR_UNILPTX, \ref ERR_NO_DEVICE, \ref ERR_SETMODE
*	- \ref NO_ERRORS
*/
extern "C" int b132_chan_stop(
	int slot,			///< [in] is returned by b132_device_lock()
	int chan,			///< [in] channel: \ref CH_A, \ref CH_B, \ref CH_AB
	int wait			///< [in] stop mode:
						///<<ul><li>FALSE - immediate stop (abort)
						///<	<li>TRUE - cmAFG: wait for end of current period </ul>
	);

/// cmDDS,cmAM,cmFM,cmPM: Set signal frequency
/** Frequency could be updated on-the-fly w/o phase discontinuity in DDS-based modes
*	\return
*	- \ref ERR_PARAM - check: slot, chan
*	- \ref ERR_VALUE - check: freq
*	- \ref ERR_NO_SUPPORT - current device mode is {cmAFG, cmSWEEP, cmADC};
*	- \ref ERR_UNILPTX, \ref ERR_NO_DEVICE, \ref ERR_SETMODE
*	- \ref NO_ERRORS
*/
extern "C" int b132_chan_set_ddsfreq(
	int slot,			///< [in] is returned by b132_device_lock()
	int chan,			///< [in] channel: \ref CH_A, \ref CH_B, \ref CH_AB
	double freq,		///< [in] new signal frequency
	double *pRealFreq	///< [out] (optional) real output frequency
	);

		/** \defgroup b132_func_chan_sweep SWEEP
		*	@{ */

/// cmSWEEP: Set signal frequency sweep parameters
/** \return
*	- \ref ERR_PARAM - check: slot, chan, steps
*	- \ref ERR_VALUE - check: freq, range, freq+range
*	- \ref ERR_NO_SUPPORT - current device mode is not cmSWEEP;
*	- \ref ERR_UNILPTX, \ref ERR_NO_DEVICE, \ref ERR_SETMODE
*	- \ref NO_ERRORS
*/
extern "C" int b132_chan_set_sweep(
	int slot,			///< [in] is returned by b132_device_lock()
	int chan,			///< [in] channel: \ref CH_A, \ref CH_B, \ref CH_AB
	double freq,		///< [in] base signal frequency value, Hz: [0..\ref MAX_FREQ]
	double range,		///< [in] signal frequency sweep range, Hz, min: {\ref SWEEP_RANGE_MIN}
	double period,		///< [in] signal frequency sweep period, s, min: {\ref SWEEP_PERIOD_MIN}
	int steps,			///< [in] signal frequency sweep steps count:
						///<<ul><li>\ref SWEEP_STEPS_AUTO - smooth sweep
						///<	<li>[\ref SWEEP_STEPS_MIN..INT_MAX] - fixed number of frequency points
						///<	<li>WARNING: the more steps the bigger range error is possible (step truncation error) </ul>
	double *pRange,		///< [out] (optional) implemented frequency range
	double *pPeriod,	///< [out] (optional) implemented sweep period
	int *pSteps			///< [out] (optional) implemented sweep steps count
	);

		/**	@} */ //b132_func_chan_sweep

		/** \defgroup b132_func_chan_modulation MODULATION
		*	@{ */

/// cmAM,cmFM,cmPM: Set carrier signal for amplitude or frequency modulation
/**	\return
*	- \ref ERR_PARAM - check: slot, ppData, *ppData, invalid samples count (use b132_device_get_timing() to get supported samples count)
*	- \ref ERR_NO_SUPPORT - current device mode is not {cmAM,cmFM,cmPM}
*	- \ref ERR_UNILPTX, \ref ERR_NO_DEVICE, \ref ERR_SETMODE
*	- \ref NO_ERRORS
*/
extern "C" int b132_chan_set_carrier(
	int slot,			///< [in] is returned by b132_device_lock()
	float **ppData,		///< [in] carrier signal data
	int samples			///< [in] carrier signal data samples count
	);

/// cmAM,cmFM,cmPM: Set modulating signal for amplitude or frequency modulation
/** \return
*	- \ref ERR_PARAM
*	  - check: slot, submode, ppData, *ppData, invalid samples count (use b132_device_get_timing() to get supported samples count)
*	  - cmAM: submode is not {BM,AM}
*	  - cmFM: submode is not {FM0,FM1,FM2,FM3}
*	  - cmPM: submode is not {PM0,PM1,PM2,PM3}
*	  - invalid samples count, use b132_device_get_timing(...) to get supported samples count
*	- \ref ERR_NO_SUPPORT	current device mode is not {cmAM,cmFM,cmPM};
*	- \ref ERR_UNILPTX, \ref ERR_NO_DEVICE, \ref ERR_SETMODE
*	- \ref NO_ERRORS
*/
extern "C" int b132_chan_set_modulating(
	int slot,			///< [in] is returned by b132_device_lock()
	int submode,		///< [in] submode index:
						///<<ul><li>cmAM: \ref AM, \ref BM,
						///<	<li>cmFM: \ref FM0, \ref FM1, \ref FM2, \ref FM3
						///<	<li>cmPM: \ref PM0, \ref PM1, \ref PM2, \ref PM3 </ul>
	float **ppData,		///< [in] modulating signal data; value ranges for different submodes:
						///<<ul><li>BM: [-1,0...1,0]
						///<	<li>AM: [ 0,0...1,0]
						///<	<li>FM[0..3]: see \ref b132_const_chan_modulation
						///<	<li>PM[0..3]: see \ref b132_const_chan_modulation </ul>
	int samples			///< [in] modulating signal data samples count
	);

		/**	@} */ //b132_func_chan_modulation

	/** @} */ //b132_func_chan


	//////////////////////////////////////////////////////////////////////////
	/** \defgroup b132_func_adc ADC FUNCTIONS
	*	@{ */

/// Get voltage at requested ADC input
/** \return
*	- \ref ERR_PARAM - check: slot, chan, averaging, pVoltage
*	- \ref ERR_NO_SUPPORT - ADC_CH_4 @ current mode is not cmADC
*	- \ref ERR_FAILED - ADC error or input voltage is out of range ±10V
*	- \ref ERR_UNILPTX, \ref ERR_NO_DEVICE, \ref ERR_SETMODE
*	- \ref NO_ERRORS - all done, requested input voltage is assigned to *pVoltage
*/
extern "C" int b132_adc_get_voltage(
	int slot,			///< [in] is returned by b132_device_lock()
	int chan,			///< [in] ADC input (\ref b132_const_adc "ADC_CH_[1, 2, 3, 4]")
	int averaging,		///< [in] ADC samples averaging factor [1..1000]
						///<	<ul><li>cmAFG supports hardware-implemented {1,2,4,8,16,32,64,128} times averaging
						///<		<li>averaging in other modes and factors is software-implemented feature
						///<	</ul>
	double *pVoltage	///< [out] voltage value
	);

	/** @} */ //b132_func_adc


	//////////////////////////////////////////////////////////////////////////
	/** \defgroup b132_func_signal SIGNAL TEMPLATES
	*	@{ */

const double pi = 3.1415926535897932384626433832795;

/// Sine signal parameters
typedef struct {
	double amplitude;	///< amplitude, V <ul><li>{+} - normal <li>{-} - inverted (the same as pi radian phase offset) </ul>
	double offset;		///< voltage offset level, V
	double period;		///< period, s
	double phase;		///< phase shift, radians
} CSineParams;

/// Calculate sine signal data with given params and fill float(single) signal samples array
/**	\return
*	- \ref ERR_PARAM - check: ppData, *ppData, samples
*	- \ref NO_ERRORS
*/
int b132_fill_sine_data(
	CSineParams sine,	///< [in] signal params
	float **ppData,		///< [out] signal data
	int samples			///< [in] signal data samples count
	);

/// Pulse signal parameters
typedef struct {
	double amplitude;	///< amplitude, V <ul><li>{+} - positive <li>{-} - inverted </ul>
	double offset;		///< voltage offset level, Vdouble period;       ///< period, s
	double frequency;    //// frequency, Hz
	double lead;		///< leading edge duration <ul><li>{+} - leading edge time, s; <li>{0..-1} - relative to period duration </ul>
	double length;		///< pulse duration <ul><li>{+} - pulse length, s; <li>{0..-1} - relative to period duration (duty factor) </ul>
	double trail;		///< trailing edge duration <ul><li>{+} - trailing edge time, s; <li>{0..-1} - relative to period duration </ul>
} CPulseParams;

/// Calculate pulse signal data with given params and fill float(single) signal samples array
/** \return
*	- \ref ERR_PARAM - check: ppData, *ppData, samples
*	- \ref NO_ERRORS
*/
int b132_fill_pulse_data(
	CPulseParams pulse,	///< [in] signal params
	float **ppData,		///< [out] signal data
	int samples			///< [in] signal data samples count
	);

/// Triangle signal parameters
typedef struct {
	double amplitude;	///< amplitude, V <ul><li>{+} - positive <li>{-} - inverted </ul>
	double offset;		///< voltage offset level, V
	double period;		///< period, s
	double peakpos;		///< peak position <ul><li>{+} - peak position, s; <li>{0..-1} - relative to period duration </ul>
} CTriangleParams;

/// Calculate triangle signal data with given params and fill float(single) signal samples array
/** \return
*	- \ref ERR_PARAM - check: ppData, *ppData, samples
*	- \ref NO_ERRORS
*/
int b132_fill_triangle_data(
	CTriangleParams triangle,	///< [in] signal params
	float **ppData,				///< [out] signal data
	int samples					///< [in] signal data samples count
	);

	/** @} */ //b132_func_signal
/**	@} */ //b132_func
