#include "StdAfx.h"
#include "E2010.h"
#include "SpectrDlg.h"
#include "DataAcqSystem.h"
#include <math.h>

// приватные стат переменные для функции GetPulseFrequency
short CE2010::m_PulseLevel(3000);
// дополнительно задаются в Initialise()
double CE2010::m_KadrRate(0);
int CE2010::m_AdcRate(0);
WORD CE2010::m_ChannelsQuantity(0x0);
DWORD CE2010::m_DataStep(0);
// изменяются в самой функции
// дополнительно задаются в конструкторе
int CE2010::m_FirstPulse[ADC_CHANNELS_QUANTITY_E2010]= {0};
short CE2010::m_BuffersBwPulses[ADC_CHANNELS_QUANTITY_E2010]= {0};
short CE2010::m_NextDelta[ADC_CHANNELS_QUANTITY_E2010] = {0};
// номера каналов
short CE2010::m_ChopperPulseChannel(0);
short CE2010::m_HSMPulseChannel(0);
short CE2010::m_MonitorPulseChannel(0);
short CE2010::m_DetectorPulseChannel(0);
WORD CE2010::m_TurbineOpenValue(0);




CE2010::CE2010(void)
{
	// занулим идентификаторы поток сбора данных
	m_hReadThread = NULL;
	m_pReadThreadData = NULL;

	//обнуляем массив c начальными импульсами
	for(WORD i(0);i<4;i++)
	{
		m_FirstPulse[i]=-1;
		m_BuffersBwPulses[i]=-1;
		m_NextDelta[i]=0;
	}
}


CE2010::~CE2010(void)
{

}


// инициализация Е2010 с заданием параметров АЦП
int CE2010::Initialise(void)
{
	WORD i(0), j(0),k(0);

	// обнулим количество каналов
	m_ChannelsQuantity = 0;
	// читаем параметры из inifile
	m_TurbineStatusTTLinPin = GetPrivateProfileInt(_T("E2010"),_T("Turbine signal (in)pin"),NULL,CSpectrDlg::m_IniFileName);
	m_TurbineOpenValue = WORD(pow(2.0,m_TurbineStatusTTLinPin-1));
	m_ChopperPulseChannel = GetPrivateProfileInt(_T("E2010"),_T("Chopper signal channel"),NULL,CSpectrDlg::m_IniFileName);
	if(m_ChopperPulseChannel==0) m_ChopperPulseChannel--;
	else m_ChannelsQuantity++;
	m_HSMPulseChannel = GetPrivateProfileInt(_T("E2010"),_T("HSM signal channel"),NULL,CSpectrDlg::m_IniFileName);
	if(m_HSMPulseChannel==0) m_HSMPulseChannel--;
	else m_ChannelsQuantity++;
	m_MonitorPulseChannel = GetPrivateProfileInt(_T("E2010"),_T("Monitor signal channel"),NULL,CSpectrDlg::m_IniFileName);
	if(m_MonitorPulseChannel==0) m_MonitorPulseChannel--;
	else m_ChannelsQuantity++;
	m_DetectorPulseChannel = GetPrivateProfileInt(_T("E2010"),_T("Detector signal channel"),NULL,CSpectrDlg::m_IniFileName);
	if(m_DetectorPulseChannel==0) m_DetectorPulseChannel--;
	else m_ChannelsQuantity++;
	m_HSMControlChannel = GetPrivateProfileInt(_T("E2010"),_T("HSM control channel"),NULL,CSpectrDlg::m_IniFileName);
	m_HSMControlChannel--;
	// выделим место для данных потока сбора данных
	m_pReadThreadData = (PREADTHREADDATA) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
                sizeof(READTHREADDATA));
	// проверим выделила ли нам система местечко
	if( m_pReadThreadData == NULL ) return 35;
	// Занулим идентивикатор файла данных
	m_pReadThreadData->hFile = INVALID_HANDLE_VALUE;

	// попробуем прочитать дескриптор устройства
	m_pReadThreadData->ModuleHandle = CSpectrDlg::m_pE2010Module->GetModuleHandle();
	if(m_pReadThreadData->ModuleHandle == INVALID_HANDLE_VALUE) return 16;

	// прочитаем название модуля в обнаруженном виртуальном слоте
	if(!CSpectrDlg::m_pE2010Module->GetModuleName(m_ModuleName)) return 17;
	// проверим, что это 'E20-10'
	if(strcmp(m_ModuleName, "E20-10")) return 18;

	// попробуем получить скорость работы шины USB
	if(!CSpectrDlg::m_pE2010Module->GetUsbSpeed(&m_UsbSpeed)) return 19;

	

	// получим информацию из ППЗУ модуля
	if(!CSpectrDlg::m_pE2010Module->GET_MODULE_DESCRIPTION(&m_ModuleDescription)) return 22;

	// получим текущие параметры работы АЦП
	if(!CSpectrDlg::m_pE2010Module->GET_ADC_PARS(&m_pReadThreadData->ap)) return 23;
	// установим желаемые параметры работы АЦП
	if(m_ModuleDescription.Module.Revision == REVISIONS_E2010[REVISION_A_E2010])
		m_pReadThreadData->ap.IsAdcCorrectionEnabled = FALSE;		// запретим автоматическую корректировку данных на уровне модуля (для Rev.A)
	else
		m_pReadThreadData->ap.IsAdcCorrectionEnabled = TRUE;
	// разрешим автоматическую корректировку данных на уровне модуля (для Rev.B и выше)
	m_pReadThreadData->ap.SynchroPars.StartSource = INT_ADC_START_E2010;						// внутренний старт сбора с АЦП
//	ap.SynchroPars.StartSource = INT_ADC_START_WITH_TRANS_E2010;		// внутренний старт сбора с АЦП с трансляцией
//	ap.SynchroPars.StartSource = EXT_ADC_START_ON_RISING_EDGE_E2010;	// внешний старт сбора с АЦП по фронту 
	m_pReadThreadData->ap.SynchroPars.SynhroSource = INT_ADC_CLOCK_E2010;						// внутренние тактовые импульсы АЦП
//	ap.SynchroPars.SynhroSource = INT_ADC_CLOCK_WITH_TRANS_E2010;		// внутренние тактовые импульсы АЦП с трансляцией
	m_pReadThreadData->ap.SynchroPars.StartDelay = 0x0;									// задержка начала сбора данных в кадрах отсчётов (для Rev.B и выше)
	m_pReadThreadData->ap.SynchroPars.StopAfterNKadrs = 0x0;							// останов сбора данных через заданное кол-во кадров отсчётов (для Rev.B и выше)
	m_pReadThreadData->ap.SynchroPars.SynchroAdMode = NO_ANALOG_SYNCHRO_E2010;	// тип аналоговой синхронизации (для Rev.B и выше)
//	ap.SynchroPars.SynchroAdMode = ANALOG_SYNCHRO_ON_HIGH_LEVEL_E2010;
	m_pReadThreadData->ap.SynchroPars.SynchroAdChannel = 0x0;							// канал аналоговой синхронизации (для Rev.B и выше)
	m_pReadThreadData->ap.SynchroPars.SynchroAdPorog = 0;								// порог аналоговой синхронизации в кодах АЦП (для Rev.B и выше)
	m_pReadThreadData->ap.SynchroPars.IsBlockDataMarkerEnabled = 0x0;// маркирование начала блока данных (удобно, например, при аналоговой синхронизации ввода по уровню) (для Rev.B и выше)
	// кол-во активных каналов				
	m_pReadThreadData->ap.ChannelsQuantity=m_ChannelsQuantity;
	if(m_pReadThreadData->ap.ChannelsQuantity==0) return 38; 		
	// формируем управляющую таблицу
	for(WORD i(1);i<=ADC_CHANNELS_QUANTITY_E2010;i++)
	{
		if(m_ChopperPulseChannel==i||
			m_HSMPulseChannel==i||
			m_MonitorPulseChannel==i||
			m_DetectorPulseChannel==i)
		{
			m_pReadThreadData->ap.ControlTable[k] = i-1;
			k++;
		}
		if(k>=m_ChannelsQuantity)
		{
			k=0;
			for(WORD i(1);i<=ADC_CHANNELS_QUANTITY_E2010;i++)
			{
				// пронумеруем очередность каналов в буфере
				if(m_ChopperPulseChannel==i) {m_ChopperPulseChannel=k;k++;}
				if(m_HSMPulseChannel==i) {m_HSMPulseChannel=k;k++;}
				if(m_MonitorPulseChannel==i) {m_MonitorPulseChannel=k;k++;}
				if(m_DetectorPulseChannel==i) {m_DetectorPulseChannel=k;k++;}
				if(k>=m_ChannelsQuantity) break;
			}
			break;
		}
	}
	//for(i = 0x0; i < m_pReadThreadData->ap.ChannelsQuantity; i++) m_pReadThreadData->ap.ControlTable[i] = (WORD)(i);
	// частота работы АЦП в кГц
	m_AdcRate = GetPrivateProfileInt(_T("E2010"),_T("ADC rate"),NULL,CSpectrDlg::m_IniFileName);
	m_pReadThreadData->ap.AdcRate = m_AdcRate;
	if(m_pReadThreadData->ap.AdcRate == 0.0) return 39;
	
	// частоту сбора будем устанавливать в зависимости от скорости USB
	if(m_UsbSpeed == USB11_LUSBAPI) 
	{
		m_pReadThreadData->ap.InterKadrDelay = 0.01;		// межкадровая задержка в мс
		if(m_ChannelsQuantity==3)
			m_pReadThreadData->DataStep = m_DataStep = 256*256*m_ChannelsQuantity;
		else
			m_pReadThreadData->DataStep = m_DataStep = 256*1024;				// размер запроса
	}
	else
	{
		m_pReadThreadData->ap.InterKadrDelay = 0.0; // межкадровая задержка в мс
		if(m_ChannelsQuantity==3)
			m_pReadThreadData->DataStep = m_DataStep = 1024*256*m_ChannelsQuantity;
		else 
			m_pReadThreadData->DataStep = m_DataStep = 1024*1024;			// размер запроса
	}
	// сконфигурим входные каналы
	for(i = 0x0; i < ADC_CHANNELS_QUANTITY_E2010; i++)
	{
		m_pReadThreadData->ap.InputRange[i] = ADC_INPUT_RANGE_3000mV_E2010;  	// входной диапазоне 3В
		m_pReadThreadData->ap.InputSwitch[i] = ADC_INPUT_SIGNAL_E2010;			// источник входа - сигнал
	}
	// передаём в структуру параметров работы АЦП корректировочные коэффициенты АЦП
	for(i = 0x0; i < ADC_INPUT_RANGES_QUANTITY_E2010; i++)
		for(j = 0x0; j  < ADC_CHANNELS_QUANTITY_E2010 ; j++)
		{
			// корректировка смещения
			m_pReadThreadData->ap.AdcOffsetCoefs[i][j] = m_ModuleDescription.Adc.OffsetCalibration[j + i*ADC_CHANNELS_QUANTITY_E2010];
			// корректировка масштаба
			m_pReadThreadData->ap.AdcScaleCoefs[i][j] = m_ModuleDescription.Adc.ScaleCalibration[j + i*ADC_CHANNELS_QUANTITY_E2010];
		}
	// передадим требуемые параметры работы АЦП в модуль
	if(!CSpectrDlg::m_pE2010Module->SET_ADC_PARS(&m_pReadThreadData->ap)) return 24;
	
	// зададим частоту кадров
	m_KadrRate = m_pReadThreadData->ap.KadrRate;
	m_AdcRate =int(m_KadrRate*m_ChannelsQuantity);
	
	// выделим память под буфер
	m_pReadThreadData->AdcBuffer= new SHORT[2*m_pReadThreadData->DataStep];
	if(m_pReadThreadData->AdcBuffer==NULL) return 25;

	// дизактивируем а потом активируем ТТЛ порты
	CSpectrDlg::m_pE2010Module->ENABLE_TTL_OUT(false);
	if(!CSpectrDlg::m_pE2010Module->ENABLE_TTL_OUT(true)) return 41;
	
	return 0;
}



// функция для коректной остановки блока Е2010
int CE2010::Stop(void)
{
	int err(0);
	short null(0);
	if (m_hReadThread!=NULL)
	{
		// ждём окончания работы потока 
		WaitForSingleObject(m_hReadThread, INFINITE);
		// освободим идентификатор потока Task & Read
		if(m_hReadThread) { CloseHandle(m_hReadThread); m_hReadThread = NULL;}
		
	}
	// дизактивируем ТТЛ порты
	if(!CSpectrDlg::m_pE2010Module->ENABLE_TTL_OUT(false))  err = 41;
	// обнулим выход ЦАП
	if(!CSpectrDlg::m_pE2010Module->DAC_SAMPLE(&CSpectrDlg::m_MinE2010DacValue,m_HSMControlChannel))  err = 42;
    

	// освободим память буфера
	if(m_pReadThreadData->AdcBuffer) { delete[] m_pReadThreadData->AdcBuffer; m_pReadThreadData->AdcBuffer = NULL; }
	
	// освободим место выделенное для данных поток
	if(m_pReadThreadData != NULL)
    {
		HeapFree(GetProcessHeap(), 0, m_pReadThreadData);
		m_pReadThreadData = NULL;    // Ensure address is not reused.
    }

	return err;
}

DWORD WINAPI CE2010::ServiceReadThread(LPVOID lpParameters)
{
	WORD i, err(0), RequestNumber, ttlIn(0);
	double frequency;
	CString anserToGetFreq(_T(""));
	CString dataDirectory;
	bool isChamberFallingBefore(false);
	// массив OVERLAPPED структур из двух элементов
	OVERLAPPED ReadOv[2];
	// массив структур с параметрами запроса на ввод/вывод данных
	IO_REQUEST_LUSBAPI IoReq[2];
	// указатель на структуру данных потока
	PREADTHREADDATA pReadThreadData;
	pReadThreadData = (PREADTHREADDATA)lpParameters;
	// возьмем директорию файла из окошка
	pReadThreadData->pSpectrDlg->GetDlgItemTextW(IDC_EDIT8,dataDirectory);
	// заполним окошки скоростей счета детектора и монитора содержимым
	if(m_DetectorPulseChannel == -1) pReadThreadData->pSpectrDlg->SetDlgItemTextW(IDC_EDIT7,_T("Not active!"));
	else pReadThreadData->pSpectrDlg->SetDlgItemInt(IDC_EDIT7,0);
	if(m_MonitorPulseChannel == -1) pReadThreadData->pSpectrDlg->SetDlgItemTextW(IDC_EDIT12,_T("Not active!"));
	else pReadThreadData->pSpectrDlg->SetDlgItemInt(IDC_EDIT12,0);
	// создадим объект класса управляющей записью данных в файл
	CDataAcqSystem dASys(dataDirectory, pReadThreadData->pSpectrDlg);
	// получич ссылку для загрузки иконок
	CStatic * pTurbStatusPict = (CStatic*)pReadThreadData->pSpectrDlg->GetDlgItem(IDC_TURBPICT); 
	// остановим работу АЦП и одновременно сбросим USB-канал чтения данных
	if(!CSpectrDlg::m_pE2010Module->STOP_ADC()) 
	{
		CSpectrDlg::PrintError(27); 
		CSpectrDlg::m_IsReadThreadComplete = true; 
		return 0x0; 
	}
	// формируем необходимые для сбора данных структуры
	for(i = 0x0; i < 0x2; i++)
	{
		// инициализация структуры типа OVERLAPPED
		ZeroMemory(&ReadOv[i], sizeof(OVERLAPPED));
		// создаём событие для асинхронного запроса
		ReadOv[i].hEvent = CreateEvent(NULL, FALSE , FALSE, NULL);
		// формируем структуру IoReq
		IoReq[i].Buffer = pReadThreadData->AdcBuffer+ i*pReadThreadData->DataStep;
		IoReq[i].NumberOfWordsToPass = pReadThreadData->DataStep;
		IoReq[i].NumberOfWordsPassed = 0x0;
		IoReq[i].Overlapped = &ReadOv[i];
		IoReq[i].TimeOut = (DWORD)(pReadThreadData->DataStep/
								   pReadThreadData->ap.KadrRate + 1000);
	}
	RequestNumber = 0x0;
	// запомним передадим состояние турбины и положение кретки
	CSpectrDlg::m_pE2010Module->TTL_IN(&ttlIn);
	if(ttlIn & m_TurbineOpenValue)
	{
		if(pTurbStatusPict->GetIcon() == CSpectrDlg::m_hIcRed)
			pTurbStatusPict->SetIcon(CSpectrDlg::m_hIcYellow);
		dASys.SendTurbineOpenFlag(true,RequestNumber);
	}
	else
	{
		if(pTurbStatusPict->GetIcon() == CSpectrDlg::m_hIcGreen || 
			pTurbStatusPict->GetIcon() == CSpectrDlg::m_hIcYellow)
			pTurbStatusPict->SetIcon(CSpectrDlg::m_hIcRed);
		dASys.SendTurbineOpenFlag(false,RequestNumber);
	}
	// поставим зеленую иконку, когда закончится заполнение  
	if(isChamberFallingBefore && !dASys.IsChamberFalling())
		pTurbStatusPict->SetIcon(CSpectrDlg::m_hIcGreen);
	//isChamberFallingBefore = dASys.IsChamberFalling();
	// вышлем состояние каретки
	dASys.SendControlSysParam(CSampPosSystem::m_EstCarriagePosition,
							CSampPosSystem::m_EstCarriagePosition, 
							CSpectrDlg::m_IsSysInMeasConditions,
							RequestNumber);
	// делаем предварительный запрос на ввод данных
	if(!CSpectrDlg::m_pE2010Module->ReadData(&IoReq[RequestNumber])) 
	{ 
		CloseHandle(ReadOv[0].hEvent); 
		CloseHandle(ReadOv[1].hEvent); 
		CSpectrDlg::PrintError(28); 
		return 0x0; 
	}
	
	// запустим АЦП
	if(CSpectrDlg::m_pE2010Module->START_ADC())
	{
		// цикл сбора данных
		while (CSpectrDlg::m_PressBn!=STOP)
		{
			
			RequestNumber ^= 0x1;
			// запомним передадим состояние турбины и положение кретки
			CSpectrDlg::m_pE2010Module->TTL_IN(&ttlIn);
			if(ttlIn & m_TurbineOpenValue)
			{
				if(pTurbStatusPict->GetIcon() == CSpectrDlg::m_hIcRed)
					pTurbStatusPict->SetIcon(CSpectrDlg::m_hIcYellow);
				dASys.SendTurbineOpenFlag(true,RequestNumber);
			}
			else
			{
				if(pTurbStatusPict->GetIcon() == CSpectrDlg::m_hIcGreen ||
					pTurbStatusPict->GetIcon() == CSpectrDlg::m_hIcYellow)
					pTurbStatusPict->SetIcon(CSpectrDlg::m_hIcRed);
				dASys.SendTurbineOpenFlag(false,RequestNumber);
			}
			// поставим зеленую иконку, когда закончится заполнение  
			if(isChamberFallingBefore && !dASys.IsChamberFalling())
				pTurbStatusPict->SetIcon(CSpectrDlg::m_hIcGreen);
			isChamberFallingBefore = dASys.IsChamberFalling();
			// вышлем состояние каретки
			dASys.SendControlSysParam(CSampPosSystem::m_EstCarriagePosition,
									CSampPosSystem::m_EstCarriagePosition,
									CSpectrDlg::m_IsSysInMeasConditions,
									RequestNumber);
			// сделаем запрос на очередную порцию данных
			if(!CSpectrDlg::m_pE2010Module->ReadData(&IoReq[RequestNumber])) 
			{ 
				err = 28;
				CSpectrDlg::PrintError(err); 
				break;
			}
			// ждём завершения операции сбора предыдущей порции данных
			if(WaitForSingleObject(ReadOv[RequestNumber^0x1].hEvent, IoReq[RequestNumber^0x1].TimeOut) == WAIT_TIMEOUT)
			{ 
				err = 29; 
				CSpectrDlg::PrintError(err);
				break;
			}
			// попробуем получить текущее состояние процесса сбора данных
			if(!CSpectrDlg::m_pE2010Module->GET_DATA_STATE(&pReadThreadData->DataState))
			{ err = 30; CSpectrDlg::PrintError(err); break; }
			// теперь можно проверить этот признак переполнения внутреннего буфера модуля
			else 
				if(pReadThreadData->DataState.BufferOverrun == (0x1 << BUFFER_OVERRUN_E2010)) 
				{ err = 31; CSpectrDlg::PrintError(err); break; }

			// обработаем и запишем полученную порцию данных в файл
			if((err = dASys.WriteData(IoReq[RequestNumber^0x1].Buffer,RequestNumber^0x1))!=0)
			{
				CSpectrDlg::PrintError(err);
				break; 
			}
			//остановим по нажатию кнопки стоп
			if (CSpectrDlg::m_PressBn == STOP) break;//эта строчка должна быть здесь!!!!
			// вычеслим частоту чоппера
			anserToGetFreq = dASys.GetSignalFrequency(E2010_CHOPPER_CHANNEL,frequency);
			if(anserToGetFreq.IsEmpty())
			{
				CSpectrDlg::PrintInEdit(pReadThreadData->pSpectrDlg->GetDlgItem(IDC_EDIT1),frequency);
				// будем следить за частотой чоппера
			}
			else 
				pReadThreadData->pSpectrDlg->SetDlgItemTextW(IDC_EDIT1,anserToGetFreq);
			// вычеслим частоту НSM
			anserToGetFreq = dASys.GetSignalFrequency(E2010_HSM_CHANNEL,frequency);
			if(anserToGetFreq.IsEmpty())
			{
				CSpectrDlg::PrintInEdit(pReadThreadData->pSpectrDlg->GetDlgItem(IDC_EDIT2),frequency);
				// здесь будем отправлять частоту в HSM для принятия решения о перестройке
				if((err = CHighSpeedMotor::SendMeasFrequency(frequency))!=0)
				{
					CSpectrDlg::PrintError(err);
					break;
				}
			}
			else
			{
				pReadThreadData->pSpectrDlg->SetDlgItemTextW(IDC_EDIT2,anserToGetFreq);
				if (anserToGetFreq == _T("No signal"))
				{
					if((err = CHighSpeedMotor::SendMeasFrequency(0))!=0)
					{
						CSpectrDlg::PrintError(err);
						break;
					}
				}
			}
			//Sleep(20);
			// выведем время набранных данных
			CSpectrDlg::PrintInEdit(pReadThreadData->pSpectrDlg->GetDlgItem(IDC_EDIT10),CSpectrDlg::m_CurMeasTime);
			// для примера вносим задержку - для нарушения целостности получаемых данных
//			if(i == 33) Sleep(1100);
		}

		// последняя порция данных
		if(err==0)
		{
			RequestNumber ^= 0x1;
			// запомним передадим состояние турбины и положение кретки
			CSpectrDlg::m_pE2010Module->TTL_IN(&ttlIn);
			if(ttlIn & m_TurbineOpenValue)
				dASys.SendTurbineOpenFlag(true,RequestNumber);
			else
				dASys.SendTurbineOpenFlag(false,RequestNumber);
			// ждём окончания операции сбора последней порции данных
			if(WaitForSingleObject(ReadOv[RequestNumber^0x1].hEvent, IoReq[RequestNumber^0x1].TimeOut) == WAIT_TIMEOUT)
				CSpectrDlg::PrintError(29);
			// обработаем и запишем полученную порцию данных в файл
			if((err = dASys.WriteData(IoReq[RequestNumber^0x1].Buffer, RequestNumber^0x1))!=0)
			{
				CSpectrDlg::PrintError(err);
			}
		}
		else CSpectrDlg::m_PressBn = STOP; // вскинем флажок остановки
	}
	else  CSpectrDlg::PrintError(33);

	// для примера вносим задержку - для нарушения целостности получаемых данных
//	Sleep(1100);

	// остановим работу АЦП
	// !!!ВАЖНО!!! Если необходима достоверная информация о целостности
	// ВСЕХ собраных данных, то функцию STOP_ADC() следует выполнять не позднее,
	// чем через 800 мс после окончания ввода последней порции данных.
	// Для заданной частоты сбора данных в 5 МГц эта величина определяет время
	// переполнения внутренненого FIFO буфера модуля, который имеет размер 8 Мб. 
	if(!CSpectrDlg::m_pE2010Module->STOP_ADC()) CSpectrDlg::PrintError(27);
	// если нужно - анализируем окончательный признак переполнения внутреннего буфера модуля
	if(pReadThreadData->DataState.BufferOverrun != (0x1 << BUFFER_OVERRUN_E2010))
	{
		// попробуем получить окончательный состояние процесса сбора данных
		if(!CSpectrDlg::m_pE2010Module->GET_DATA_STATE(&pReadThreadData->DataState)) 
			CSpectrDlg::PrintError(30);
		// теперь можно проверить этот признак переполнения внутреннего буфера модуля
		//else 
			if(pReadThreadData->DataState.BufferOverrun == (0x1 << BUFFER_OVERRUN_E2010)) 
				CSpectrDlg::PrintError(31);
	}
	// прервём возможно незавершённый асинхронный запрос на приём данных
	if(!CancelIo(pReadThreadData->ModuleHandle)) CSpectrDlg::PrintError(34);
	// освободим все идентификаторы событий
	for(i = 0x0; i < 0x2; i++) CloseHandle(ReadOv[i].hEvent);
	// небольшая задержка
	Sleep(100);
	// установим флажок завершения работы потока сбора данных
	CSpectrDlg::m_IsReadThreadComplete = true;
	// теперь можно спокойно выходить из потока
	return 0x0;
}



// функция вычисления частоты импульсов канале
//WORD CE2010::GetPulseFrequency(WORD channel, short aBuffer[], double &frequency)
//{
//	const WORD avrN(10);
//	float aFrequency[avrN]={0.0};
//	short lastDelta(0);
//	WORD j(0);
//	frequency = 0.0;
//
//	switch (channel)
//	{
//		case E2010_CHOPPER_CHANNEL: 
//			if(m_ChopperPulseChannel==-1) return 4;
//			channel = m_ChopperPulseChannel;
//			break;
//		case E2010_HSM_CHANNEL: 
//			if(m_HSMPulseChannel==-1) return 4;
//			channel = m_HSMPulseChannel;
//			break;
//		case E2010_DETECTOR_CHANNEL: 
//			if(m_DetectorPulseChannel==-1) return 4;
//			channel = m_DetectorPulseChannel;
//			break;
//		case E2010_MONITOR_CHANNEL: 
//			if(m_MonitorPulseChannel==-1) return 4;
//			channel = m_MonitorPulseChannel;
//			break;
//	}
//	for(DWORD i(channel); i<m_DataStep;
//		i+=m_ChannelsQuantity)
//	{
//		lastDelta = m_NextDelta[channel];
//		m_NextDelta[channel] = aBuffer[i]-m_PulseLevel;
//		if((lastDelta*m_NextDelta[channel])<0)
//		{
//			if(m_NextDelta[channel]>0)
//			{
//				if(m_FirstPulse[channel] == -1) m_FirstPulse[channel] = i;
//				else
//				{
//					if((i-m_FirstPulse[channel])<1000 && m_BuffersBwPulses[channel]==-1);
//					else
//					{
//						aFrequency[j] = m_KadrRate*1000.*
//										m_ChannelsQuantity/
//										(i-m_FirstPulse[channel]+
//										(m_BuffersBwPulses[channel]+1)*(
//										m_DataStep));
//						if(m_BuffersBwPulses[channel]!=-1) m_BuffersBwPulses[channel] = -1;
//						m_FirstPulse[channel] = i;
//						j++;
//					}
//					if(j==avrN) break;
//				}
//			}
//		}
//	}
//	for(WORD i(0);i<j;i++) frequency+=aFrequency[i];
//	if(frequency != 0.0)
//	{
//	  	frequency /= j;
//		if(j<avrN) m_BuffersBwPulses[channel]++;
//		else
//		{
//			m_FirstPulse[channel] = m_BuffersBwPulses[channel] = -1;	
//			m_NextDelta[channel] = 0;
//		}	 
//		return 0; //все хорошо. частота есть, выходим из функции
//	}
//	if(m_FirstPulse[channel]>=0)
//	{
//		m_BuffersBwPulses[channel]++;
//		if(m_BuffersBwPulses[channel]>10)
//		{
//			m_BuffersBwPulses[channel] = m_FirstPulse[channel] = -1;
//			return 2; // сигнал меньше минимальной частоты
//		}
//		return 1; //сигнал есть, но еще вычисляется частота
//	}
//	return 3; // нет сигнала
//}
