// SpectrDlg.h : header file
//

#pragma once
#include "MyDefines.h"
#include "afxwin.h"
#include "windows.h"
#include "Chopper.h"
#include "E2010.h"
#include "_Include/Lusbapi.h"
#include "GraphDlg.h"
#include "HighSpeedMotor.h"
#include "SampPosSystem.h"
#pragma comment (lib,"..\\_Lib\\lusbapi.lib")

//структура объектов используемых в измерении
struct CTRL_USE_BY_MEASUREING
{
	CChopper *pChopper;
	CE2010 *pE2010;
	CSampPosSystem *pSPS;
	CHighSpeedMotor *pHSM;
};


// CSpectrDlg dialog
class CSpectrDlg : public CDialog
{
// Construction
public:
	CSpectrDlg(CWnd* pParent = NULL);	// standard constructor


	// Dialog Data
	enum { IDD = IDD_SPECTR_DIALOG };

	CEdit m_EditText;

	static CString m_IniFileName; // путь к ини файлу
	static short m_TaskArray[4][100];   // массив задач
    static WORD m_PressBn; // идентификатор нажатой кнопки
	static short m_CurMeasTime; // текущее время измерения в точке
	static HICON m_hIcGreen;
	static HICON m_hIcRed;
	static HICON m_hIcYellow;
	CEdit m_EditDataFile;
	//Диалоговые окна для представления данных
	CGraphDlg* m_pScanDlg;
	CGraphDlg* m_pOscilDlg;
	 //обработчики меню
	afx_msg void OnMainSettings();
//	afx_msg void OnMainTaskfile();
	afx_msg void OnGraphScan();
	afx_msg void OnGraphOscillation();
	// обработчики кнопок
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedPause();
	afx_msg void OnBnClickedStop();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	virtual void OnCancel();

	// флажок завершения работы потока сбора данных
	static bool m_IsReadThreadComplete;
	static bool m_IsSysInMeasConditions;
	// функция вывода сообщения об ошибке
	static void PrintError(int err); 
	static void PrintInEdit(CWnd *pWnd, int value); //функция записи в окно
    static void PrintInEdit(CWnd *pWnd, double value); //функция записи в окно	
	static CString GetDirPathOfFile(CString filePath)
	{
		int n(0);
		n = filePath.GetLength();
		if(n != 0)
		{
			while (filePath.GetAt(n--)!='\\');
			return filePath.Left(++n);
		}
		else return _T("");
	}	
	// возвращает путь к директории с исполняемым файлом
	static CString GetExecutFileDir(void){return m_ExacutableFileDir;};
	// указатель на интерфейс модуля
	static ILE2010 *m_pE2010Module;
	static short m_MinE2010DacValue;
	static short m_MaxE2010DacValue;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	// Implementation
	HICON m_hIcon;
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnMymDrawGraph(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
	

private:
	bool TASK_DEFINE; // флажок успешного задания списка задач
	CString m_DataFilesDirectory; // директория для записи файла данных
	static CString m_ExacutableFileDir;
	//идентнификатор потока задания
	HANDLE m_hTaskThread; 
	DWORD m_dwTaskThreadId;
	static DWORD WINAPI TaskThreadFunc(LPVOID lpParametrs);
	CStatic m_TurbPict;	
	

	
public:
//	afx_msg void OnTaskfileAdvansedmode();
//	afx_msg void OnTaskfileScanmode();
	afx_msg void OnMenuTaskfile();
};
