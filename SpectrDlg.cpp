// SpectrDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Spectr.h"
#include "SpectrDlg.h"
#include "SplashDlg.h"
#include "Settings.h"
#include "�TaskFileDlg.h"
#include "DataAcqSystem.h"
#include "YesOrNotDlg.h"
#include <fstream>



using std::ifstream;



#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// ������� ��������� ���� ��������
void StopAll(CTRL_USE_BY_MEASUREING Control); 
//����������� CSpectrDlg
CString CSpectrDlg::m_IniFileName = _T(""); //�������� �������� ini file
CString CSpectrDlg::m_ExacutableFileDir(_T(""));
WORD CSpectrDlg::m_PressBn(0); // ��������� ������� ������
short CSpectrDlg::m_TaskArray[4][100] = {NULL}; //�������� ������ �����
short CSpectrDlg::m_CurMeasTime = 0; // ������� ����� ���������
// ������ ������ ��������� ������ ������ ����� ������
bool CSpectrDlg::m_IsReadThreadComplete = false;
// ������� ������ ����� ��� ������� ������ � ���������
bool CSpectrDlg::m_IsSysInMeasConditions = false;
HICON CSpectrDlg::m_hIcGreen = NULL;
HICON CSpectrDlg::m_hIcRed = NULL;
HICON CSpectrDlg::m_hIcYellow = NULL;
ILE2010 *CSpectrDlg::m_pE2010Module(NULL);
short CSpectrDlg::m_MinE2010DacValue(-2048),CSpectrDlg::m_MaxE2010DacValue(2047);
CSplashDlg* CSplashDlg::c_pSplashDlg;
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();// ����������� ����������� ������ 

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSettingsAcp();
public:
	afx_msg void OnAboutbox();

protected:

};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_COMMAND(ID_SETTINGS_ACP, &CAboutDlg::OnSettingsAcp)
	ON_COMMAND(IDD_ABOUTBOX, &CAboutDlg::OnAboutbox)
END_MESSAGE_MAP()


// CSpectrDlg dialog




CSpectrDlg::CSpectrDlg(CWnd* pParent /*=NULL*/)  // ����������� ������ SpectrDlg
	: CDialog(CSpectrDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	TASK_DEFINE = false; // ������ ��������� ������� ������ �����
	m_DataFilesDirectory = _T("");// ������� �������� ����� � �������
	m_pScanDlg = m_pOscilDlg = NULL;// ������� ��������� �� ������ ��������
	
}

void CSpectrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_EditText);
	DDX_Control(pDX, IDC_EDIT8, m_EditDataFile);
	DDX_Control(pDX,IDC_TURBPICT,m_TurbPict);
	DDX_Control(pDX, IDC_STATIC5, m_statusText);
	m_statusText.GetDC()->SetTextColor(RGB(255,0,0));
}

BEGIN_MESSAGE_MAP(CSpectrDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	ON_COMMAND(ID_MAIN_SETTINGS, &CSpectrDlg::OnMainSettings)
	ON_COMMAND(ID_GRAPH_SCAN, &CSpectrDlg::OnGraphScan)
	ON_BN_CLICKED(IDC_START, &CSpectrDlg::OnBnClickedStart)
	ON_BN_CLICKED(IDC_PAUSE, &CSpectrDlg::OnBnClickedPause)
	ON_BN_CLICKED(IDC_STOP, &CSpectrDlg::OnBnClickedStop)
	ON_BN_CLICKED(IDC_BUTTON4, &CSpectrDlg::OnBnClickedButton4)
	ON_WM_CTLCOLOR()
	
	ON_COMMAND(ID_GRAPH_OSCILLATION, &CSpectrDlg::OnGraphOscillation)
	ON_MESSAGE(MyM_DRAW_GRAPH, &CSpectrDlg::OnMymDrawGraph)
	//ON_COMMAND(ID_TASKFILE_ADVANSEDMODE, &CSpectrDlg::OnTaskfileAdvansedmode)
	//ON_COMMAND(ID_TASKFILE_SCANMODE, &CSpectrDlg::OnTaskfileScanmode)
	ON_COMMAND(ID_MENU_TASKFILE, &CSpectrDlg::OnMenuTaskfile)
END_MESSAGE_MAP()


// CSpectrDlg message handlers

BOOL CSpectrDlg::OnInitDialog()
{
	
	CDialog::OnInitDialog();
	CSplashDlg::ShowSplashScreen(NULL);

	// ������� ���� � ������� ����������
	//m_statusText.SendMessage(WM_CTLCOLOR);
	m_statusText.SetWindowText(_T("Experiment is stoped. To start define task file and datafile name."));
	CString filePath(_T(""));
	GetModuleFileName(NULL,filePath.GetBufferSetLength(_MAX_PATH),_MAX_PATH);
	m_ExacutableFileDir = GetDirPathOfFile(filePath);
	//��������� ������
	UINT n = ExtractIconEx(m_ExacutableFileDir + _T("\\icon1.ico"),0,&m_hIcGreen,NULL,1);
	n += ExtractIconEx(m_ExacutableFileDir +_T("\\icon2.ico"),0,&m_hIcRed,NULL,1);
	n += ExtractIconEx(m_ExacutableFileDir +_T("\\icon3.ico"),0,&m_hIcYellow,NULL,1);
	if(n!=3) AfxMessageBox(_T("Don't all icons found!!!"));
	else m_TurbPict.SetIcon(m_hIcRed);// ��������� ������ �������� �������
	// �������� ������ ���� ��� settings.ini
	m_IniFileName.Empty();// ������� ��� ����� settings.ini	
	m_IniFileName.Format(m_ExacutableFileDir + _T("\\settings.ini"));
	// �������� settings.ini, ���� ��� ���
	ifstream testFileOpen(m_IniFileName);
	if (!testFileOpen) 
	{
		CSettings dlg(_T(""));
		dlg.SaveToIniFile(m_IniFileName,false);
	}
	else testFileOpen.close();

	//===========part of initialisation of E2010 modul=============
	// ��������� �������� ��������� �� ���������
	m_pE2010Module = static_cast<ILE2010 *>(CreateLInstance("e2010"));
	WORD i(0);
	// ��������� ���������� ������ E20-10 � ������ MAX_VIRTUAL_SLOtS_QUANTITY_LUSBAPI ����������� ������
	for(i = 0x0; i < MAX_VIRTUAL_SLOTS_QUANTITY_LUSBAPI; i++) if(CSpectrDlg::m_pE2010Module->OpenLDevice(i)) break;
	// ���-������ ����������?
	if(i == MAX_VIRTUAL_SLOTS_QUANTITY_LUSBAPI) PrintError(15);
	// ����� ��� ���� ������ �� ���������������� ������� ������� DLL ����������
	if(!CSpectrDlg::m_pE2010Module->LOAD_MODULE()) PrintError(20);
	// �������� �������� ������
	if(!CSpectrDlg::m_pE2010Module->TEST_MODULE()) PrintError(21);
	if(!m_pE2010Module) PrintError(14);
	else 
	{	
		// write min value -2048 in DAC to prevent HSM rotation
		WORD chan = GetPrivateProfileInt(_T("E2010"),_T("HSM control channel"),NULL,CSpectrDlg::m_IniFileName);
		if (!CSpectrDlg::m_pE2010Module->DAC_SAMPLE(&m_MinE2010DacValue,--chan)) PrintError(51);
	}
	//===============================================

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	ShowWindow(SW_SHOW);

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSpectrDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSpectrDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSpectrDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CAboutDlg::OnSettingsAcp()
{
	// TODO: Add your command handler code here
}

void CAboutDlg::OnAboutbox()
{
	// TODO: Add your command handler code here
}

void CSpectrDlg::OnMainSettings()
{
	// TODO: �������� ���� ��� ����������� ������
	CSettings PSettings(_T("Settings"));
	PSettings.DoModal();
	//if(PSettings.DoModal()==IDOK) AfxMessageBox(_T("Inifile define!"));
}


void CSpectrDlg::OnMenuTaskfile()
{
	// TODO: �������� ���� ��� ����������� ������
	�TaskFileDlg TaskDlg;
	if (TaskDlg.DoModal()==IDOK) 
	{
		TASK_DEFINE=true ;
		CEdit *pEdit=(CEdit *)GetDlgItem(IDC_EDIT9);
		pEdit->Clear(); // ������� ������
		pEdit->SetWindowText(TaskDlg.m_TaskFileName);//������� �������� ����� � ������ IDC_EDIT9
		pEdit->LineScroll(0,TaskDlg.m_TaskFileName.GetLength());// ������� � ����� �������, ����� ���� ����� �������� �����
		// ��������� ��������� ����� ����� ��� � CGraphDlg
		if( m_pScanDlg != NULL && m_pScanDlg->m_hWnd != NULL) m_pScanDlg->SetMinMaxScanPoints();
	}
		
}


void CSpectrDlg::OnBnClickedStart()
{
	// �������� ������ �� ���������� ��� ������ � ������ �����
	if (m_EditDataFile.GetWindowTextLengthW()==0) {AfxMessageBox(_T("Don't specify data directory!"));return;}
	if (!TASK_DEFINE) {AfxMessageBox(_T("TaskArray don't define!"));return;}
	//������� ������ ����� � ����
	GetDlgItem(IDC_PAUSE)->EnableWindow(1);
	GetDlgItem(IDC_STOP)->EnableWindow(1);
	GetDlgItem(IDC_START)->EnableWindow(0);
	// ������� ������ ���� ��� ��������� ��������
	CMenu* mmenu = GetMenu();
	mmenu->EnableMenuItem(ID_GRAPH_OSCILLATION,MF_ENABLED);
	mmenu->EnableMenuItem(ID_GRAPH_SCAN,MF_ENABLED);
	//������� � ��������� �����
	m_hTaskThread = CreateThread(NULL,0,TaskThreadFunc,this,0,&m_dwTaskThreadId);
	if(!m_hTaskThread) {PrintError(12);return;} 
	m_statusText.SetWindowText(_T("Experiment start!"));
	return;
}


void CSpectrDlg::OnBnClickedPause()
{
	switch (m_PressBn)
	{
	case NULL:
		GetDlgItem(IDC_PAUSE)->SetWindowTextW(_T("Continue"));
		m_PressBn = PAUSE;
		break;
	case PAUSE:
		GetDlgItem(IDC_PAUSE)->SetWindowTextW(_T("Pause"));
		m_PressBn = NULL;
		break;
	}
	// TODO: �������� ���� ��� ����������� �����������
	
}


void CSpectrDlg::OnBnClickedStop()
{
	m_PressBn = STOP;
	
	// ��� ��������� ������ �������
	WaitForSingleObject(m_hTaskThread, INFINITE);
	// ��������� ������������� ������ Task & Read
	if(m_hTaskThread) { CloseHandle(m_hTaskThread); m_hTaskThread = NULL; }
	//�������� ������ ����� � ����
	GetDlgItem(IDC_PAUSE)->EnableWindow(0);
	GetDlgItem(IDC_STOP)->EnableWindow(0);
	GetDlgItem(IDC_START)->EnableWindow(1);
	//��������� ������
	GetDlgItem(IDC_EDIT5)->SetWindowTextW(0);
	GetDlgItem(IDC_EDIT10)->SetWindowTextW(0);
	GetDlgItem(IDC_EDIT11)->SetWindowTextW(0);
	GetDlgItem(IDC_EDIT2)->SetWindowTextW(0);
	GetDlgItem(IDC_EDIT1)->SetWindowTextW(0);
	GetDlgItem(IDC_EDIT8)->SetWindowTextW(0);
	GetDlgItem(IDC_EDIT7)->SetWindowTextW(0);
	GetDlgItem(IDC_EDIT12)->SetWindowTextW(0);
	// ������ ������ Pause �� ��������
	GetDlgItem(IDC_PAUSE)->SetWindowTextW(_T("Pause"));
	//��������� ������ �������� �������
	m_TurbPict.SetIcon(m_hIcRed);
	m_statusText.SetWindowText(_T("Experiment is stoped. To start define task file and datafile name."));
	//������� ������������� ������� ������� 
	m_PressBn = NULL;

}


void CSpectrDlg::OnCancel()
{
	// ��������� ����������� ������
	DestroyIcon(m_hIcRed);
	DestroyIcon(m_hIcGreen);
	DestroyIcon(m_hIcYellow);
	// ������ ����� ��������
	HANDLE handleTxt(INVALID_HANDLE_VALUE);
	WIN32_FIND_DATA ft;
	handleTxt = FindFirstFile(m_ExacutableFileDir +_T("\\*.txt"), &ft);
	// ������ ��� txt �����, ���� �����
	if(handleTxt!=INVALID_HANDLE_VALUE)
	{
		CString str(_T(""));
		do
		{
			str = m_ExacutableFileDir+_T("\\")+ ft.cFileName;
			DeleteFile(str);
		}
		while (FindNextFile(handleTxt, &ft));
		FindClose(handleTxt);
	}

	// ������� ����������� ���� �2010
	if(m_pE2010Module->CloseLDevice()==false) PrintError(40);
	// ��������� ��������� ������
	if(m_pE2010Module)
	{
		// ��������� ��������� ������
		if(!m_pE2010Module->ReleaseLInstance()) PrintError(26);
		// ������� ��������� �� ��������� ������
		m_pE2010Module = NULL;
	}
	CDialog::OnCancel();
}


// ������� ������ ������
void CSpectrDlg::PrintError(int err)
{
	switch(err)
	{
		// ������ ���������� �132
		case 1:  AfxMessageBox(_T("No one b132 device find!"));
				break;
		case 2: AfxMessageBox(_T("No one b132 slot find!"));
				break;
		case 3: AfxMessageBox(_T("Can't set cmDDS mode on b132!"));
				break;
		case 4: AfxMessageBox(_T("Can't set range on b132!"));
				break;
		case 5: AfxMessageBox(_T("Can't set filter on b132 channels"));
				break;
		case 6:  AfxMessageBox(_T( "Can't b132 channel enable!"));
				break;
		case 7: AfxMessageBox(_T( "Can't set pulse mode on b132 channels!"));
				break;
		case 8: AfxMessageBox(_T("Can't start b132 generator!"));
				break;
		case 9: AfxMessageBox(_T("Can't set b132 generator frequency!"));
				break;
		case 10: AfxMessageBox(_T("Chopper setting parametrs can't be zerro!"));
				break;
		case 11: AfxMessageBox(_T("Can't stop b132 generator!"));
				break;
		case 12: AfxMessageBox(_T("Can't create task thread!"));
				break;
		case 13: AfxMessageBox(_T("Can't unlock b132 generator!"));
				break;
		// ������ ������ �2010
		case 14: AfxMessageBox(_T("Can't get pointer to E2010 modul interface!"));
				break;
		case 15: AfxMessageBox(_T(" Can't find any module E20-10 in first 127 virtual slots!\n"));
				break;
		case 16: AfxMessageBox(_T(" GetModuleHandle() --> Bad"));
				break;
		case 17: AfxMessageBox(_T(" GetModuleName() --> Bad"));
				break;
		case 18: AfxMessageBox(_T(" The module is not 'E20-10'"));
				break;
		case 19: AfxMessageBox(_T(" GetUsbSpeed() --> Bad"));
				break;
		case 20: AfxMessageBox(_T(" LOAD_MODULE() --> Bad"));
				break;
		case 21: AfxMessageBox(_T(" TEST_MODULE() --> Bad"));
				break;
		case 22: AfxMessageBox(_T(" GET_MODULE_DESCRIPTION() --> Bad"));
				break;
		case 23: AfxMessageBox(_T(" GET_ADC_PARS() --> Bad"));
				break;
		case 24: AfxMessageBox(_T(" SET_ADC_PARS() --> Bad"));
				break;
		case 25: AfxMessageBox(_T(" Can not allocate m_AdcBuffer memory!"));
				break;
		case 26: AfxMessageBox(_T(" ReleaseLInstance() --> Bad"));
				break;
		case 27: AfxMessageBox(_T(" ADC Thread: STOP_ADC() --> Bad"));
				break;
		case 28: AfxMessageBox(_T(" ADC Thread: ReadData() --> Bad"));
				break;
		case 29: AfxMessageBox(_T(" ADC Thread: Timeout is occured!"));
				break;
		case 30: AfxMessageBox(_T(" ADC Thread: CHECK_DATA_INTERGRITY() --> Bad"));
				break;
		case 31: AfxMessageBox(_T(" ADC Thread: Buffer Overrun --> Bad"));
				break;
		case 32: AfxMessageBox(_T(" DAS: Writing data file error!"));
				break;
		case 33: AfxMessageBox(_T(" ADC Thread: START_ADC() --> Bad"));
				break;
		case 34: AfxMessageBox(_T(" ADC Thread: Can't cancel pending input and output (I/O) operations!"));
				break;
		case 35: AfxMessageBox(_T(" Can't allocate memory for ReadThreadData!"));
				break;
		case 36: AfxMessageBox(_T("DAS:Can't create file for ADC read data!"));
				break;
		case 37: AfxMessageBox(_T(" ServiceReadThread() --> Bad"));
				break;
		case 38: AfxMessageBox(_T("No one Adc channel define!"));
				break;
		case 39: AfxMessageBox(_T("Adc rate can't be zerro!"));
				break;
		case 40: AfxMessageBox(_T("E2010: Can't close virtual device!"));
				break;
		case 41: AfxMessageBox(_T("SampPosSystem: Can't open out TTL port!"));
				break;
		case 42: AfxMessageBox(_T("HighSpeedMotor: Can't set DAC data! DAC_SAMPLE() error!"));
				break;
		case 43: AfxMessageBox(_T("DAS:Can't open raw data file!"));
				break;
		case 44: AfxMessageBox(_T("DAS:Incorrect written raw data file!"));
				break;
		case 45: AfxMessageBox(_T("DAS:Raw data file is empty!"));
				break;
		case 46: AfxMessageBox(_T("DAS:No one events in detector chennal!"));
				break;
		case 47: AfxMessageBox(_T("DAS:Can't write processed file!"));
				break;
		case 48: AfxMessageBox(_T("DAS:Can't write graph file!"));
				break;
		case 49: AfxMessageBox(_T("HighSpeedMotor: Can't allocate memory for m_pMeasFrequency!"));
				break;
		case 50: AfxMessageBox(_T("HighSpeedMotor: Can't open file HSM_Table.tbl!"));
				break;
		case 51: AfxMessageBox(_T("E2010 initalisation: Can't set DAC value!"));
			break;
		case 52: AfxMessageBox(_T("Critical drop in HSM frequency!!!"));
			break;
		case 53: AfxMessageBox(_T("Critical drop in chopper frequency!!!"));
			break;

	}
	return;
}

DWORD WINAPI CSpectrDlg::TaskThreadFunc(LPVOID lpParametrs)
{
	int err(NO_ERRORS),Cicle(1),Point(0);
	CString dataDirectory = _T("");
	//������� ��������� �� SpectrDlg
	CDialog *pSpectrDlg = (CDialog*)lpParametrs;
	// �������� ������� ����������
	// ������ �����������
	CChopper chopper;
	// ������ ����� ������ � ���������
	CE2010 e2010;
	// ������ ������ ���������
	CSampPosSystem sampPosSys(pSpectrDlg->GetDlgItem(IDC_EDIT5),
		pSpectrDlg->GetDlgItem(IDC_EDIT6));
	// ������ ���������� ���������
	CHighSpeedMotor highSpeedMotor;

	CTRL_USE_BY_MEASUREING Control;
	//������������� ��������� CTRL_USE_BY_MEASUREING
	Control.pChopper = &chopper;
	Control.pE2010 = &e2010;
	Control.pSPS = &sampPosSys;
	Control.pHSM = &highSpeedMotor;
	// ������������� �������
	if ((err=chopper.Initialise())!=NO_ERRORS) {PrintError(err);return 0;} 
	if ((err=e2010.Initialise())==NO_ERRORS) 
	{
		//����������� ��������� ������ ReadThreadData
		e2010.m_pReadThreadData->pSpectrDlg = pSpectrDlg;
		//�������� ����� ����� ������
		e2010.m_hReadThread=CreateThread(NULL, 0x2000,
			                             e2010.ServiceReadThread,
										 e2010.m_pReadThreadData,
										 0,&e2010.m_ReadTid);
		if(!e2010.m_hReadThread) {StopAll(Control);PrintError(37);return 0x0;}
	
	}
	else { StopAll(Control); PrintError(err); return 0;}
	// ��������� �������
	while(1)
	{
		PrintInEdit(pSpectrDlg->GetDlgItem(IDC_EDIT11),Cicle);// ����� ����� �����
		Point = 0;
		while((m_TaskArray[CHOPPER_FREQUENCY][Point]+
				m_TaskArray[HSM_FREQUENCY][Point]+
				m_TaskArray[CARRIAGE_POSITION][Point]+
				m_TaskArray[MEASUREMENT_TIME][Point])!=0)
		{
			//������� ������ ���������� ������� � ���������
			CSpectrDlg::m_IsSysInMeasConditions = false;
			// ��������� ������� ���������
			if ((err=chopper.SetFrequency(m_TaskArray[CHOPPER_FREQUENCY][Point]))!=NO_ERRORS) 
			{
				// ���� err = -1 ������ ������ STOP
				if (err!=-1) PrintError(err); 
				//��������� ��� �������
				StopAll(Control);
				//������� �� ������
				return 0;
			} 
			// ��������� ������� � ������ �������
			if (!sampPosSys.SetPosition(m_TaskArray[CARRIAGE_POSITION][Point])) 
			{
				//��������� ��� �������
				StopAll(Control);
				//������� �� ������
				return 0;
			}
			// ��������� ��������
			if ((err=highSpeedMotor.SetFrequency(m_TaskArray[HSM_FREQUENCY][Point]))!=NO_ERRORS) 
			{
				// ���� err = -1 ������ ������ STOP
				if (err!=-1) PrintError(err); 
				//��������� ��� �������
				StopAll(Control);
				//������� �� ������
				return 0;
			} 
			//�������� ������ ���������� ������� � ���������
			CSpectrDlg::m_IsSysInMeasConditions = true;
			// ���������� ������ ���������
			CDataAcqSystem::ResetTheTimer();
			while(m_CurMeasTime<=m_TaskArray[MEASUREMENT_TIME][Point])
			{
				if (CSpectrDlg::m_PressBn==STOP) 
				{
					//��������� ��� �������
					StopAll(Control); 
					//������� �� ������
					return 0;
				}
				Sleep(10);
			}
			Point++;
		}
		Cicle++;
	}

	return 0;
}
void StopAll(CTRL_USE_BY_MEASUREING Control)
{
	int err(0);
	if((err=Control.pE2010->Stop())!=NO_ERRORS) CSpectrDlg::PrintError(err);
	if((err=Control.pChopper->Stop())!=NO_ERRORS) CSpectrDlg::PrintError(err);
	return ;
}

//������� ������ � ����
void CSpectrDlg::PrintInEdit(CWnd *pWnd, int value)
{
	CString str;
	str.Format(_T("%i"),value);		
	pWnd->SetWindowText(str);//������� �������� ����� � ������ 
	pWnd->UpdateWindow();
}
void CSpectrDlg::PrintInEdit(CWnd *pWnd, double value)
{
	CString str;
	str.Format(_T("%.3f"),value);		
	pWnd->SetWindowText(str);//������� �������� ����� � ������ 
	pWnd->UpdateWindow();
}

void CSpectrDlg::OnBnClickedButton4()
{
	//Folder Browser code.
    BROWSEINFO bi;
    ZeroMemory(&bi, sizeof(BROWSEINFO));

    bi.hwndOwner = m_hWnd;
    bi.ulFlags   = BIF_RETURNONLYFSDIRS |
				   BIF_NEWDIALOGSTYLE ;
	
    TCHAR szFolder[MAX_PATH*2];
    szFolder[0] = _T('\0');
	bool isDirDefine = false;
	LPITEMIDLIST pidl;

    while (isDirDefine == false)
    {
		pidl = SHBrowseForFolder(&bi); // �������� ������
        if (pidl)
		{
			if (SHGetPathFromIDList(pidl, szFolder)) // �������� ����������
			{	
				WIN32_FIND_DATA fd,ft; 
				HANDLE handleDat(INVALID_HANDLE_VALUE), handleTxt(INVALID_HANDLE_VALUE);
				CString strPath;
				m_DataFilesDirectory = strPath = szFolder ;
				handleDat = FindFirstFile(strPath +_T("\\*.dat"), &fd);
				handleTxt = FindFirstFile(strPath +_T("\\*.txt"), &ft);
				// ���� � ���������� ���� ����������� �����
				//�������� ���� ��������
				if(handleDat == INVALID_HANDLE_VALUE ||
					handleTxt == INVALID_HANDLE_VALUE) 
				{
					m_EditDataFile.Clear();
					m_EditDataFile.SetWindowTextW(m_DataFilesDirectory);
					m_EditDataFile.LineScroll(0,m_DataFilesDirectory.GetLength());
					isDirDefine = true;
				}
				else 
				{
					CYesOrNotDlg dlg;
					if(dlg.DoModal()==IDOK)
					{
						CString str(_T(""));
						// ������ ��� dat �����
						do
						{
							 str = m_DataFilesDirectory + _T("\\") + fd.cFileName;
							DeleteFile(str);
						}
						while (FindNextFile(handleDat, &fd));
						// ������ ��� txt �����
						do
						{
							 str = m_DataFilesDirectory + _T("\\") + ft.cFileName;
							DeleteFile(str);
						}
						while (FindNextFile(handleTxt, &ft));
						m_EditDataFile.Clear();
						m_EditDataFile.SetWindowTextW(m_DataFilesDirectory);
						m_EditDataFile.LineScroll(0,m_DataFilesDirectory.GetLength());
						isDirDefine = true;						
					}
				}
				FindClose(handleDat);
				FindClose(handleTxt);
			}
		}
		else break;
    }

	if (pidl) 
	{
		IMalloc *pMalloc = NULL; 
		if (SUCCEEDED(SHGetMalloc(&pMalloc)) && pMalloc) 
		{  
			pMalloc->Free(pidl);  
			pMalloc->Release(); 
		}
	}
}



HBRUSH CSpectrDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �������� ����� �������� DC
	if(pWnd->GetDlgCtrlID() == IDC_EDIT10)
	{
		(m_TurbPict.GetIcon() == m_hIcYellow || 
			m_TurbPict.GetIcon() == m_hIcRed || 
			!m_IsSysInMeasConditions) ? pDC->SetTextColor(RGB(255,0,0))
						 : pDC->SetTextColor(RGB(0,180,0));
		//m_statusText.GetDC()->SetTextColor(RGB(255,0,0));
	}
	if(pWnd->GetDlgCtrlID() == IDC_STATIC5)
	{
		CString str(_T(""));
		pWnd->GetWindowText(str);
		if(str.Find(_T("stop"))>0)
			pDC->SetTextColor(RGB(255,0,0));
		else 
			pDC->SetTextColor(RGB(0,180,0));

	}
	return hbr;
}


void CSpectrDlg::OnGraphScan()
{
	if(m_pScanDlg==NULL)
	{
		m_pScanDlg = new CGraphDlg(SCAN_GRAPH,m_ExacutableFileDir);
		m_pScanDlg->Create(IDD_GRAPHDLG,this);
		m_pScanDlg->ShowWindow(SW_SHOW);// TODO: �������� ���� ��� ����������� ������
	}
	else
	{
		if(m_pScanDlg->m_hWnd == NULL)
		{ 
			m_pScanDlg = new CGraphDlg(SCAN_GRAPH,m_ExacutableFileDir);
			m_pScanDlg->Create(IDD_GRAPHDLG,this);
			m_pScanDlg->ShowWindow(SW_SHOW);// TODO: �������� ���� ��� ����������� ������
		}
	};
}


void CSpectrDlg::OnGraphOscillation()
{
	if(m_pOscilDlg==NULL)
	{
		m_pOscilDlg = new CGraphDlg(OSCIL_GRAPH,m_ExacutableFileDir);
		m_pOscilDlg->Create(IDD_GRAPHDLG,this);
		m_pOscilDlg->ShowWindow(SW_SHOW);// TODO: �������� ���� ��� ����������� ������
	}
	else
	{
		if(m_pOscilDlg->m_hWnd == NULL)
		{ 
			m_pOscilDlg = new CGraphDlg(OSCIL_GRAPH,m_ExacutableFileDir);
			m_pOscilDlg->Create(IDD_GRAPHDLG,this);
			m_pOscilDlg->ShowWindow(SW_SHOW);// TODO: �������� ���� ��� ����������� ������
		}
	};// TODO: �������� ���� ��� ����������� ������
}

afx_msg LRESULT CSpectrDlg::OnMymDrawGraph(WPARAM wParam, LPARAM lParam)
{
	if( m_pScanDlg != NULL && m_pScanDlg->m_hWnd != NULL) m_pScanDlg->RedrawWindow();
	if( m_pOscilDlg != NULL && m_pOscilDlg->m_hWnd != NULL) m_pOscilDlg->RedrawWindow();
	return 0;
}
