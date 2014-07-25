

#pragma once


// ���������� ���� CGraphDlg

class CGraphDlg : public CDialog
{
	DECLARE_DYNAMIC(CGraphDlg)

public:
	CGraphDlg(CWnd* pParent = NULL);   // ����������� �����������
	CGraphDlg(BYTE graphID,CString dirName, CWnd* pParent = NULL);   // ��� �����������
	virtual ~CGraphDlg();
	
	// ������ ����������� ����
	enum { IDD = IDD_GRAPHDLG };
	// ����������� ���������
	afx_msg void OnPaint();
	afx_msg void OnBar();
	afx_msg void OnDatapoints();
	virtual BOOL OnInitDialog();
	// ������ ������� ����������
	inline void SetDataDirectory(CString dirName);
	void SetMinMaxScanPoints(void);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // ��������� DDX/DDV
	DECLARE_MESSAGE_MAP()
	virtual void OnCancel();
	CString m_GraphFileDir;
	BYTE m_graphID;
	bool m_IsBarPoint;
	//������ ������ ���������, � ������� ������, ������ ��� ���������
	inline CPoint DrawRefFrame(CPaintDC *pDC, float *minMaxPoints, RECT canvasRect, float *outScale );
	// ������ ����� � �������
	inline void DrawDataPoint(CPaintDC *pDC, CPoint point, int errorY);
	inline int ReadGraphDataFile(CString fileName, float *outMinMaxPoints, float *outDataPoints);
private:
	float m_MinMaxScanPoints[102];
	
	
};

