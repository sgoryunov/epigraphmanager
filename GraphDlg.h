

#pragma once


// диалоговое окно CGraphDlg

class CGraphDlg : public CDialog
{
	DECLARE_DYNAMIC(CGraphDlg)

public:
	CGraphDlg(CWnd* pParent = NULL);   // стандартный конструктор
	CGraphDlg(BYTE graphID,CString dirName, CWnd* pParent = NULL);   // мой конструктор
	virtual ~CGraphDlg();
	
	// Данные диалогового окна
	enum { IDD = IDD_GRAPHDLG };
	// обработчики сообщений
	afx_msg void OnPaint();
	afx_msg void OnBar();
	afx_msg void OnDatapoints();
	virtual BOOL OnInitDialog();
	// задать рабочаю директорию
	inline void SetDataDirectory(CString dirName);
	void SetMinMaxScanPoints(void);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV
	DECLARE_MESSAGE_MAP()
	virtual void OnCancel();
	CString m_GraphFileDir;
	BYTE m_graphID;
	bool m_IsBarPoint;
	//выдает начало координат, и масштаб меняет, рисует оси координат
	inline CPoint DrawRefFrame(CPaintDC *pDC, float *minMaxPoints, RECT canvasRect, float *outScale );
	// рисует точку с ошибкой
	inline void DrawDataPoint(CPaintDC *pDC, CPoint point, int errorY);
	inline int ReadGraphDataFile(CString fileName, float *outMinMaxPoints, float *outDataPoints);
private:
	float m_MinMaxScanPoints[102];
	
	
};

