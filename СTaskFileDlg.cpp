// СTaskFileDlg.cpp: файл реализации
//

#include "stdafx.h"
#include "Spectr.h"
#include "SpectrDlg.h"
#include "СTaskFileDlg.h"
#include "afxdialogex.h"
#include "Resource.h"
#include "ScanTaskDlg.h"
#include "MyDefines.h"




// диалоговое окно СTaskFileDlg

IMPLEMENT_DYNAMIC(СTaskFileDlg, CDialog)

СTaskFileDlg::СTaskFileDlg(CWnd* pParent /*=NULL*/) // конструктор класса TaskFileDlg
	: CDialog(СTaskFileDlg::IDD, pParent)
{
	m_TaskFileName.Empty();
	
}

СTaskFileDlg::~СTaskFileDlg()
{
}

void СTaskFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EditOpen, m_EditOpen);
	DDX_Control(pDX, IDC_LIST1, m_listedit);
	DDX_Control(pDX, IDC_OpenTaskFile, m_bOpen);
	DDX_Control(pDX, IDOK, m_bOK);
	DDX_Control(pDX, IDC_SAVE, m_bSave);
	DDX_Control(pDX, IDC_EDIT2, m_EditChopper);
}


BEGIN_MESSAGE_MAP(СTaskFileDlg, CDialog)
	ON_BN_CLICKED(IDC_OpenTaskFile, &СTaskFileDlg::OnBnClickedOpentaskfile)
	ON_BN_CLICKED(IDC_SAVE, &СTaskFileDlg::OnBnClickedSave)
	ON_BN_CLICKED(IDC_INSERT, &СTaskFileDlg::OnBnClickedInsert)
	ON_BN_CLICKED(IDC_REMOVE, &СTaskFileDlg::OnBnClickedRemove)
	ON_BN_CLICKED(IDC_REPLACE, &СTaskFileDlg::OnBnClickedReplace)
	ON_BN_CLICKED(IDOK, &СTaskFileDlg::OnOK)
	ON_COMMAND(ID_GENERATETASKLIST_SCAN, &СTaskFileDlg::OnGeneratetasklistScan)
	ON_COMMAND(ID_GENERATETASKLIST_FOURIER, &СTaskFileDlg::OnGeneratetasklistFourier)
END_MESSAGE_MAP()


// обработчики сообщений СTaskFileDlg


void СTaskFileDlg::OnBnClickedOpentaskfile()
{
		CString fname; //Переменная для хранения имени файла
		CString ref;
		m_listedit.ResetContent();// очищаем listbox

		CFileDialog fDialog(TRUE,_T("txt"),NULL, OFN_HIDEREADONLY,_T(" Text Files (*.txt) |*.txt||"));  
	    fDialog.DoModal();
		fname=m_TaskFileName=fDialog.GetPathName();
		if(!fname.IsEmpty())
		{
			m_EditOpen.Clear(); // зачистим окошко
			m_EditOpen.SetWindowText(fname);
			m_EditOpen.LineScroll(0,fname.GetLength()); //встанем в конец строчки
			//m_EditOpen.GetWindowText(fname); //Получаем имя файла
			CStdioFile File(fname,CFile::modeRead|CFile::typeText);
			do
			{
				File.ReadString(ref);
				if (!ref.IsEmpty()) m_listedit.AddString(ref);
			}
			while(!ref.IsEmpty());
			File.Close();
			m_bOK.EnableWindow(true); // активируем кнопку ОК
			m_bSave.EnableWindow(false); // дизактивируем кнопку save
		}
        
}

void СTaskFileDlg::OnBnClickedSave() // сохраняем файл задания
{
	CString fname; //Переменная для хранения имени файла

	CFileDialog fDialog(FALSE,_T("txt"),NULL, OFN_EXPLORER,_T(" Text Files (*.txt) |*.txt||"));  
	fDialog.DoModal();

	fname=m_TaskFileName=fDialog.GetPathName();
	m_EditOpen.Clear(); // зачистим окошко
	m_EditOpen.SetWindowText(fname);// выведем названия фйла
	m_EditOpen.LineScroll(0,fname.GetLength()); //встанем в конец строчки
	
	CStdioFile File(fname,CFile::modeCreate | CFile::modeWrite | CFile::typeText);
	CString ref;
	
	int indexMax=m_listedit.GetCount(); // узнаем количество строк в listbox
	// пишем в файл
	for(int index=0; index<indexMax; index++)
	{
		m_listedit.GetText(index,ref); //берем первую строчку из listbox
		if (!ref.IsEmpty()) File.WriteString(ref+_T("\n"));// пишем ее в file
	}
	File.Close();
	m_bSave.EnableWindow(false); // дизактивируем кнопку save
	m_bOK.EnableWindow(true); //активируем кнопку ОК
}

void СTaskFileDlg::OnBnClickedInsert()
{
	CString text1,text2,text3,text4;
	CString ftext;

	GetDlgItem(IDC_EDIT2)->GetWindowText(text1);
	GetDlgItem(IDC_EDIT3)->GetWindowText(text2);
	GetDlgItem(IDC_EDIT4)->GetWindowText(text3);
	GetDlgItem(IDC_EDIT1)->GetWindowText(text4);
	if (!text1.IsEmpty()&!text2.IsEmpty()&!text3.IsEmpty()&!text4.IsEmpty())
	{
		ftext=(_T(" Chopper = "))+text1+(_T(" HSMotor = "))+text2+(_T(" Carriage Position = "))+text3+(_T(" Measurement Time = "))+text4;
		((CListBox*)GetDlgItem(IDC_LIST1))->AddString(ftext);
		m_bSave.EnableWindow(true); // активируем кнопку save
		m_bOK.EnableWindow(false); // дизактивируем кнопку ОК
	}
	else AfxMessageBox(_T("Don't define measurement parameters!"));
}


void СTaskFileDlg::OnBnClickedRemove()
{
	  int index;

      index = m_listedit.GetCurSel();
	  m_listedit.DeleteString(index);
	  if ((m_listedit.GetCount()==0) & (index==0)) 
	  {
		  m_bOK.EnableWindow(false);
		  m_bSave.EnableWindow(false);
	  }
}


void СTaskFileDlg::OnBnClickedReplace() //добавление новой строки вместо выделенной
{
	int index=0;    
	CString text1,text2,text3,text4;
	CString ftext;
	
	index = m_listedit.GetCurSel();  
	if (index!=-1) // проверяем заполнение listbox)
	{
		//добавление новой строки
		GetDlgItem(IDC_EDIT2)->GetWindowText(text1);
		GetDlgItem(IDC_EDIT3)->GetWindowText(text2);
		GetDlgItem(IDC_EDIT4)->GetWindowText(text3);
		GetDlgItem(IDC_EDIT1)->GetWindowText(text4);
		// проверяем заполнение окон параметров измерения
		if (!text1.IsEmpty()&!text2.IsEmpty()&!text3.IsEmpty()&!text4.IsEmpty())
		{
			m_listedit.DeleteString(index);//удаление строки
			ftext=(_T(" Chopper = "))+text1+(_T(" "))+(_T("HSMotor = "))+text2+(_T(" "))+(_T("Carriage Position = "))+text3+(_T(" "))+(_T("Measurement Time = "))+text4;
			m_listedit.InsertString(index,ftext);
			m_bSave.EnableWindow(true); // активируем кнопку save
			m_bOK.EnableWindow(false); // дизактивируем кнопку ОК
		}
		else AfxMessageBox(_T("Don't define measurement parameters!"));
	}
	else AfxMessageBox(_T("Don't specify string in tasklist!"));
}

void СTaskFileDlg::OnOK()
{
	CString ref;
	int indexMax=m_listedit.GetCount(); // узнаем количество строк в listbox
	int indexIn(0),symbNum(0), strLength(0); 

	CString str;
	str=_T(" Chopper = ");
	int ChopperStrLenght(str.GetLength());
	str=_T(" HSMotor = ");
	int HSMotorStrLenght(str.GetLength());
	str=_T(" Carriage Position = ");
	int CarriagePositionStrLenght(str.GetLength());
	str=_T(" Measurement Time = ");
	int MeasurementTimeStrLenght(str.GetLength());
	
	// пишем в массив
	memset(CSpectrDlg::m_TaskArray,0, sizeof (short)*400); //обнуляем массив задач
	for(int index=0; index<indexMax; index++)
	{
		m_listedit.GetText(index,ref); //берем первую строчку из listbox

		indexIn=ChopperStrLenght;
		symbNum=ref.Find(_T(" HSMotor = "))-indexIn;
		CSpectrDlg::m_TaskArray[CHOPPER_FREQUENCY][index]=StrToInt(ref.Mid(indexIn, symbNum));

		indexIn=ref.Find(_T(" HSMotor = "))+HSMotorStrLenght;
		symbNum=ref.Find(_T(" Carriage Position = "))-indexIn;
		CSpectrDlg::m_TaskArray[HSM_FREQUENCY][index]=StrToInt(ref.Mid(indexIn, symbNum));

		indexIn=ref.Find(_T(" Carriage Position = "))+CarriagePositionStrLenght;
		symbNum=ref.Find(_T(" Measurement Time = "))-indexIn;
		CSpectrDlg::m_TaskArray[CARRIAGE_POSITION][index]=StrToInt(ref.Mid(indexIn, symbNum));

		indexIn=ref.Find(_T(" Measurement Time = "))+MeasurementTimeStrLenght;
		symbNum=ref.GetLength()-indexIn;
		CSpectrDlg::m_TaskArray[MEASUREMENT_TIME][index]=StrToInt(ref.Mid(indexIn, symbNum));

	}
	CDialog::OnOK(); // закрываем диалоговое окно
	
	
}


void СTaskFileDlg::OnGeneratetasklistScan()
{
	CScanTaskDlg taskDlg;// = new CScanTaskDlg(_T("Scan list"));
	taskDlg.SetDlgType(SCAN_TASK);
	if (taskDlg.DoModal()==IDOK)
	{

	}
}


void СTaskFileDlg::OnGeneratetasklistFourier()
{
	CScanTaskDlg taskDlg;// = new CScanTaskDlg(_T("Scan list"));
	taskDlg.SetDlgType(FOURIER_TASK);
	if (taskDlg.DoModal()==IDOK)
	{

	}
}