#pragma once
#include "HSMSettings.h" // делаем классы страниц видимыми
#include "ChopperSettings.h"
#include "StepMotorSettings.h"
#include "IK220Settings.h"
#include "E2010Settings.h"
#include "SpectrDlg.h"


// CSettings

class CSettings : public CPropertySheet
{

	DECLARE_DYNAMIC(CSettings)

public:
	CSettings(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CSettings(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CSettings();
	virtual  BOOL OnInitDialog( );
	void SaveToIniFile(CString fileName, bool fileExist); // функция создания и сохранения ini-file
	
protected:
	afx_msg void OnOK();
	afx_msg void OnApply();
	DECLARE_MESSAGE_MAP()
private:
	CHSMSettings	m_HSMSetDlg; // первая страница
    CChopperSettings m_ChopperSetDlg;
	CStepMotorSettings m_StepMotorSetDlg;
	CIK220Settings m_IK220SetDlg;
	CE2010Settings m_E2010SetDlg;
	
};


