#include "StdAfx.h"
#include "E2010.h"
#include "SpectrDlg.h"
#include "DataAcqSystem.h"
#include <math.h>

// ��������� ���� ���������� ��� ������� GetPulseFrequency
short CE2010::m_PulseLevel(3000);
// ������������� �������� � Initialise()
double CE2010::m_KadrRate(0);
int CE2010::m_AdcRate(0);
WORD CE2010::m_ChannelsQuantity(0x0);
DWORD CE2010::m_DataStep(0);
// ���������� � ����� �������
// ������������� �������� � ������������
int CE2010::m_FirstPulse[ADC_CHANNELS_QUANTITY_E2010]= {0};
short CE2010::m_BuffersBwPulses[ADC_CHANNELS_QUANTITY_E2010]= {0};
short CE2010::m_NextDelta[ADC_CHANNELS_QUANTITY_E2010] = {0};
// ������ �������
short CE2010::m_ChopperPulseChannel(0);
short CE2010::m_HSMPulseChannel(0);
short CE2010::m_MonitorPulseChannel(0);
short CE2010::m_DetectorPulseChannel(0);
WORD CE2010::m_TurbineOpenValue(0);




CE2010::CE2010(void)
{
	// ������� �������������� ����� ����� ������
	m_hReadThread = NULL;
	m_pReadThreadData = NULL;

	//�������� ������ c ���������� ����������
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


// ������������� �2010 � �������� ���������� ���
int CE2010::Initialise(void)
{
	WORD i(0), j(0),k(0);

	// ������� ���������� �������
	m_ChannelsQuantity = 0;
	// ������ ��������� �� inifile
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
	// ������� ����� ��� ������ ������ ����� ������
	m_pReadThreadData = (PREADTHREADDATA) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
                sizeof(READTHREADDATA));
	// �������� �������� �� ��� ������� ��������
	if( m_pReadThreadData == NULL ) return 35;
	// ������� ������������� ����� ������
	m_pReadThreadData->hFile = INVALID_HANDLE_VALUE;

	// ��������� ��������� ���������� ����������
	m_pReadThreadData->ModuleHandle = CSpectrDlg::m_pE2010Module->GetModuleHandle();
	if(m_pReadThreadData->ModuleHandle == INVALID_HANDLE_VALUE) return 16;

	// ��������� �������� ������ � ������������ ����������� �����
	if(!CSpectrDlg::m_pE2010Module->GetModuleName(m_ModuleName)) return 17;
	// ��������, ��� ��� 'E20-10'
	if(strcmp(m_ModuleName, "E20-10")) return 18;

	// ��������� �������� �������� ������ ���� USB
	if(!CSpectrDlg::m_pE2010Module->GetUsbSpeed(&m_UsbSpeed)) return 19;

	

	// ������� ���������� �� ���� ������
	if(!CSpectrDlg::m_pE2010Module->GET_MODULE_DESCRIPTION(&m_ModuleDescription)) return 22;

	// ������� ������� ��������� ������ ���
	if(!CSpectrDlg::m_pE2010Module->GET_ADC_PARS(&m_pReadThreadData->ap)) return 23;
	// ��������� �������� ��������� ������ ���
	if(m_ModuleDescription.Module.Revision == REVISIONS_E2010[REVISION_A_E2010])
		m_pReadThreadData->ap.IsAdcCorrectionEnabled = FALSE;		// �������� �������������� ������������� ������ �� ������ ������ (��� Rev.A)
	else
		m_pReadThreadData->ap.IsAdcCorrectionEnabled = TRUE;
	// �������� �������������� ������������� ������ �� ������ ������ (��� Rev.B � ����)
	m_pReadThreadData->ap.SynchroPars.StartSource = INT_ADC_START_E2010;						// ���������� ����� ����� � ���
//	ap.SynchroPars.StartSource = INT_ADC_START_WITH_TRANS_E2010;		// ���������� ����� ����� � ��� � �����������
//	ap.SynchroPars.StartSource = EXT_ADC_START_ON_RISING_EDGE_E2010;	// ������� ����� ����� � ��� �� ������ 
	m_pReadThreadData->ap.SynchroPars.SynhroSource = INT_ADC_CLOCK_E2010;						// ���������� �������� �������� ���
//	ap.SynchroPars.SynhroSource = INT_ADC_CLOCK_WITH_TRANS_E2010;		// ���������� �������� �������� ��� � �����������
	m_pReadThreadData->ap.SynchroPars.StartDelay = 0x0;									// �������� ������ ����� ������ � ������ �������� (��� Rev.B � ����)
	m_pReadThreadData->ap.SynchroPars.StopAfterNKadrs = 0x0;							// ������� ����� ������ ����� �������� ���-�� ������ �������� (��� Rev.B � ����)
	m_pReadThreadData->ap.SynchroPars.SynchroAdMode = NO_ANALOG_SYNCHRO_E2010;	// ��� ���������� ������������� (��� Rev.B � ����)
//	ap.SynchroPars.SynchroAdMode = ANALOG_SYNCHRO_ON_HIGH_LEVEL_E2010;
	m_pReadThreadData->ap.SynchroPars.SynchroAdChannel = 0x0;							// ����� ���������� ������������� (��� Rev.B � ����)
	m_pReadThreadData->ap.SynchroPars.SynchroAdPorog = 0;								// ����� ���������� ������������� � ����� ��� (��� Rev.B � ����)
	m_pReadThreadData->ap.SynchroPars.IsBlockDataMarkerEnabled = 0x0;// ������������ ������ ����� ������ (������, ��������, ��� ���������� ������������� ����� �� ������) (��� Rev.B � ����)
	// ���-�� �������� �������				
	m_pReadThreadData->ap.ChannelsQuantity=m_ChannelsQuantity;
	if(m_pReadThreadData->ap.ChannelsQuantity==0) return 38; 		
	// ��������� ����������� �������
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
				// ����������� ����������� ������� � ������
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
	// ������� ������ ��� � ���
	m_AdcRate = GetPrivateProfileInt(_T("E2010"),_T("ADC rate"),NULL,CSpectrDlg::m_IniFileName);
	m_pReadThreadData->ap.AdcRate = m_AdcRate;
	if(m_pReadThreadData->ap.AdcRate == 0.0) return 39;
	
	// ������� ����� ����� ������������� � ����������� �� �������� USB
	if(m_UsbSpeed == USB11_LUSBAPI) 
	{
		m_pReadThreadData->ap.InterKadrDelay = 0.01;		// ����������� �������� � ��
		if(m_ChannelsQuantity==3)
			m_pReadThreadData->DataStep = m_DataStep = 256*256*m_ChannelsQuantity;
		else
			m_pReadThreadData->DataStep = m_DataStep = 256*1024;				// ������ �������
	}
	else
	{
		m_pReadThreadData->ap.InterKadrDelay = 0.0; // ����������� �������� � ��
		if(m_ChannelsQuantity==3)
			m_pReadThreadData->DataStep = m_DataStep = 1024*256*m_ChannelsQuantity;
		else 
			m_pReadThreadData->DataStep = m_DataStep = 1024*1024;			// ������ �������
	}
	// ����������� ������� ������
	for(i = 0x0; i < ADC_CHANNELS_QUANTITY_E2010; i++)
	{
		m_pReadThreadData->ap.InputRange[i] = ADC_INPUT_RANGE_3000mV_E2010;  	// ������� ��������� 3�
		m_pReadThreadData->ap.InputSwitch[i] = ADC_INPUT_SIGNAL_E2010;			// �������� ����� - ������
	}
	// ������� � ��������� ���������� ������ ��� ���������������� ������������ ���
	for(i = 0x0; i < ADC_INPUT_RANGES_QUANTITY_E2010; i++)
		for(j = 0x0; j  < ADC_CHANNELS_QUANTITY_E2010 ; j++)
		{
			// ������������� ��������
			m_pReadThreadData->ap.AdcOffsetCoefs[i][j] = m_ModuleDescription.Adc.OffsetCalibration[j + i*ADC_CHANNELS_QUANTITY_E2010];
			// ������������� ��������
			m_pReadThreadData->ap.AdcScaleCoefs[i][j] = m_ModuleDescription.Adc.ScaleCalibration[j + i*ADC_CHANNELS_QUANTITY_E2010];
		}
	// ��������� ��������� ��������� ������ ��� � ������
	if(!CSpectrDlg::m_pE2010Module->SET_ADC_PARS(&m_pReadThreadData->ap)) return 24;
	
	// ������� ������� ������
	m_KadrRate = m_pReadThreadData->ap.KadrRate;
	m_AdcRate =int(m_KadrRate*m_ChannelsQuantity);
	
	// ������� ������ ��� �����
	m_pReadThreadData->AdcBuffer= new SHORT[2*m_pReadThreadData->DataStep];
	if(m_pReadThreadData->AdcBuffer==NULL) return 25;

	// ������������� � ����� ���������� ��� �����
	CSpectrDlg::m_pE2010Module->ENABLE_TTL_OUT(false);
	if(!CSpectrDlg::m_pE2010Module->ENABLE_TTL_OUT(true)) return 41;
	
	return 0;
}



// ������� ��� ��������� ��������� ����� �2010
int CE2010::Stop(void)
{
	int err(0);
	short null(0);
	if (m_hReadThread!=NULL)
	{
		// ��� ��������� ������ ������ 
		WaitForSingleObject(m_hReadThread, INFINITE);
		// ��������� ������������� ������ Task & Read
		if(m_hReadThread) { CloseHandle(m_hReadThread); m_hReadThread = NULL;}
		
	}
	// ������������� ��� �����
	if(!CSpectrDlg::m_pE2010Module->ENABLE_TTL_OUT(false))  err = 41;
	// ������� ����� ���
	if(!CSpectrDlg::m_pE2010Module->DAC_SAMPLE(&CSpectrDlg::m_MinE2010DacValue,m_HSMControlChannel))  err = 42;
    

	// ��������� ������ ������
	if(m_pReadThreadData->AdcBuffer) { delete[] m_pReadThreadData->AdcBuffer; m_pReadThreadData->AdcBuffer = NULL; }
	
	// ��������� ����� ���������� ��� ������ �����
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
	// ������ OVERLAPPED �������� �� ���� ���������
	OVERLAPPED ReadOv[2];
	// ������ �������� � ����������� ������� �� ����/����� ������
	IO_REQUEST_LUSBAPI IoReq[2];
	// ��������� �� ��������� ������ ������
	PREADTHREADDATA pReadThreadData;
	pReadThreadData = (PREADTHREADDATA)lpParameters;
	// ������� ���������� ����� �� ������
	pReadThreadData->pSpectrDlg->GetDlgItemTextW(IDC_EDIT8,dataDirectory);
	// �������� ������ ��������� ����� ��������� � �������� ����������
	if(m_DetectorPulseChannel == -1) pReadThreadData->pSpectrDlg->SetDlgItemTextW(IDC_EDIT7,_T("Not active!"));
	else pReadThreadData->pSpectrDlg->SetDlgItemInt(IDC_EDIT7,0);
	if(m_MonitorPulseChannel == -1) pReadThreadData->pSpectrDlg->SetDlgItemTextW(IDC_EDIT12,_T("Not active!"));
	else pReadThreadData->pSpectrDlg->SetDlgItemInt(IDC_EDIT12,0);
	// �������� ������ ������ ����������� ������� ������ � ����
	CDataAcqSystem dASys(dataDirectory, pReadThreadData->pSpectrDlg);
	// ������� ������ ��� �������� ������
	CStatic * pTurbStatusPict = (CStatic*)pReadThreadData->pSpectrDlg->GetDlgItem(IDC_TURBPICT); 
	// ��������� ������ ��� � ������������ ������� USB-����� ������ ������
	if(!CSpectrDlg::m_pE2010Module->STOP_ADC()) 
	{
		CSpectrDlg::PrintError(27); 
		CSpectrDlg::m_IsReadThreadComplete = true; 
		return 0x0; 
	}
	// ��������� ����������� ��� ����� ������ ���������
	for(i = 0x0; i < 0x2; i++)
	{
		// ������������� ��������� ���� OVERLAPPED
		ZeroMemory(&ReadOv[i], sizeof(OVERLAPPED));
		// ������ ������� ��� ������������ �������
		ReadOv[i].hEvent = CreateEvent(NULL, FALSE , FALSE, NULL);
		// ��������� ��������� IoReq
		IoReq[i].Buffer = pReadThreadData->AdcBuffer+ i*pReadThreadData->DataStep;
		IoReq[i].NumberOfWordsToPass = pReadThreadData->DataStep;
		IoReq[i].NumberOfWordsPassed = 0x0;
		IoReq[i].Overlapped = &ReadOv[i];
		IoReq[i].TimeOut = (DWORD)(pReadThreadData->DataStep/
								   pReadThreadData->ap.KadrRate + 1000);
	}
	RequestNumber = 0x0;
	// �������� ��������� ��������� ������� � ��������� ������
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
	// �������� ������� ������, ����� ���������� ����������  
	if(isChamberFallingBefore && !dASys.IsChamberFalling())
		pTurbStatusPict->SetIcon(CSpectrDlg::m_hIcGreen);
	//isChamberFallingBefore = dASys.IsChamberFalling();
	// ������ ��������� �������
	dASys.SendControlSysParam(CSampPosSystem::m_EstCarriagePosition,
							CSampPosSystem::m_EstCarriagePosition, 
							CSpectrDlg::m_IsSysInMeasConditions,
							RequestNumber);
	// ������ ��������������� ������ �� ���� ������
	if(!CSpectrDlg::m_pE2010Module->ReadData(&IoReq[RequestNumber])) 
	{ 
		CloseHandle(ReadOv[0].hEvent); 
		CloseHandle(ReadOv[1].hEvent); 
		CSpectrDlg::PrintError(28); 
		return 0x0; 
	}
	
	// �������� ���
	if(CSpectrDlg::m_pE2010Module->START_ADC())
	{
		// ���� ����� ������
		while (CSpectrDlg::m_PressBn!=STOP)
		{
			
			RequestNumber ^= 0x1;
			// �������� ��������� ��������� ������� � ��������� ������
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
			// �������� ������� ������, ����� ���������� ����������  
			if(isChamberFallingBefore && !dASys.IsChamberFalling())
				pTurbStatusPict->SetIcon(CSpectrDlg::m_hIcGreen);
			isChamberFallingBefore = dASys.IsChamberFalling();
			// ������ ��������� �������
			dASys.SendControlSysParam(CSampPosSystem::m_EstCarriagePosition,
									CSampPosSystem::m_EstCarriagePosition,
									CSpectrDlg::m_IsSysInMeasConditions,
									RequestNumber);
			// ������� ������ �� ��������� ������ ������
			if(!CSpectrDlg::m_pE2010Module->ReadData(&IoReq[RequestNumber])) 
			{ 
				err = 28;
				CSpectrDlg::PrintError(err); 
				break;
			}
			// ��� ���������� �������� ����� ���������� ������ ������
			if(WaitForSingleObject(ReadOv[RequestNumber^0x1].hEvent, IoReq[RequestNumber^0x1].TimeOut) == WAIT_TIMEOUT)
			{ 
				err = 29; 
				CSpectrDlg::PrintError(err);
				break;
			}
			// ��������� �������� ������� ��������� �������� ����� ������
			if(!CSpectrDlg::m_pE2010Module->GET_DATA_STATE(&pReadThreadData->DataState))
			{ err = 30; CSpectrDlg::PrintError(err); break; }
			// ������ ����� ��������� ���� ������� ������������ ����������� ������ ������
			else 
				if(pReadThreadData->DataState.BufferOverrun == (0x1 << BUFFER_OVERRUN_E2010)) 
				{ err = 31; CSpectrDlg::PrintError(err); break; }

			// ���������� � ������� ���������� ������ ������ � ����
			if((err = dASys.WriteData(IoReq[RequestNumber^0x1].Buffer,RequestNumber^0x1))!=0)
			{
				CSpectrDlg::PrintError(err);
				break; 
			}
			//��������� �� ������� ������ ����
			if (CSpectrDlg::m_PressBn == STOP) break;//��� ������� ������ ���� �����!!!!
			// �������� ������� �������
			anserToGetFreq = dASys.GetSignalFrequency(E2010_CHOPPER_CHANNEL,frequency);
			if(anserToGetFreq.IsEmpty())
			{
				CSpectrDlg::PrintInEdit(pReadThreadData->pSpectrDlg->GetDlgItem(IDC_EDIT1),frequency);
				// ����� ������� �� �������� �������
			}
			else 
				pReadThreadData->pSpectrDlg->SetDlgItemTextW(IDC_EDIT1,anserToGetFreq);
			// �������� ������� �SM
			anserToGetFreq = dASys.GetSignalFrequency(E2010_HSM_CHANNEL,frequency);
			if(anserToGetFreq.IsEmpty())
			{
				CSpectrDlg::PrintInEdit(pReadThreadData->pSpectrDlg->GetDlgItem(IDC_EDIT2),frequency);
				// ����� ����� ���������� ������� � HSM ��� �������� ������� � �����������
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
			// ������� ����� ��������� ������
			CSpectrDlg::PrintInEdit(pReadThreadData->pSpectrDlg->GetDlgItem(IDC_EDIT10),CSpectrDlg::m_CurMeasTime);
			// ��� ������� ������ �������� - ��� ��������� ����������� ���������� ������
//			if(i == 33) Sleep(1100);
		}

		// ��������� ������ ������
		if(err==0)
		{
			RequestNumber ^= 0x1;
			// �������� ��������� ��������� ������� � ��������� ������
			CSpectrDlg::m_pE2010Module->TTL_IN(&ttlIn);
			if(ttlIn & m_TurbineOpenValue)
				dASys.SendTurbineOpenFlag(true,RequestNumber);
			else
				dASys.SendTurbineOpenFlag(false,RequestNumber);
			// ��� ��������� �������� ����� ��������� ������ ������
			if(WaitForSingleObject(ReadOv[RequestNumber^0x1].hEvent, IoReq[RequestNumber^0x1].TimeOut) == WAIT_TIMEOUT)
				CSpectrDlg::PrintError(29);
			// ���������� � ������� ���������� ������ ������ � ����
			if((err = dASys.WriteData(IoReq[RequestNumber^0x1].Buffer, RequestNumber^0x1))!=0)
			{
				CSpectrDlg::PrintError(err);
			}
		}
		else CSpectrDlg::m_PressBn = STOP; // ������� ������ ���������
	}
	else  CSpectrDlg::PrintError(33);

	// ��� ������� ������ �������� - ��� ��������� ����������� ���������� ������
//	Sleep(1100);

	// ��������� ������ ���
	// !!!�����!!! ���� ���������� ����������� ���������� � �����������
	// ���� �������� ������, �� ������� STOP_ADC() ������� ��������� �� �������,
	// ��� ����� 800 �� ����� ��������� ����� ��������� ������ ������.
	// ��� �������� ������� ����� ������ � 5 ��� ��� �������� ���������� �����
	// ������������ ������������� FIFO ������ ������, ������� ����� ������ 8 ��. 
	if(!CSpectrDlg::m_pE2010Module->STOP_ADC()) CSpectrDlg::PrintError(27);
	// ���� ����� - ����������� ������������� ������� ������������ ����������� ������ ������
	if(pReadThreadData->DataState.BufferOverrun != (0x1 << BUFFER_OVERRUN_E2010))
	{
		// ��������� �������� ������������� ��������� �������� ����� ������
		if(!CSpectrDlg::m_pE2010Module->GET_DATA_STATE(&pReadThreadData->DataState)) 
			CSpectrDlg::PrintError(30);
		// ������ ����� ��������� ���� ������� ������������ ����������� ������ ������
		//else 
			if(pReadThreadData->DataState.BufferOverrun == (0x1 << BUFFER_OVERRUN_E2010)) 
				CSpectrDlg::PrintError(31);
	}
	// ������ �������� ������������� ����������� ������ �� ���� ������
	if(!CancelIo(pReadThreadData->ModuleHandle)) CSpectrDlg::PrintError(34);
	// ��������� ��� �������������� �������
	for(i = 0x0; i < 0x2; i++) CloseHandle(ReadOv[i].hEvent);
	// ��������� ��������
	Sleep(100);
	// ��������� ������ ���������� ������ ������ ����� ������
	CSpectrDlg::m_IsReadThreadComplete = true;
	// ������ ����� �������� �������� �� ������
	return 0x0;
}



// ������� ���������� ������� ��������� ������
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
//		return 0; //��� ������. ������� ����, ������� �� �������
//	}
//	if(m_FirstPulse[channel]>=0)
//	{
//		m_BuffersBwPulses[channel]++;
//		if(m_BuffersBwPulses[channel]>10)
//		{
//			m_BuffersBwPulses[channel] = m_FirstPulse[channel] = -1;
//			return 2; // ������ ������ ����������� �������
//		}
//		return 1; //������ ����, �� ��� ����������� �������
//	}
//	return 3; // ��� �������
//}
