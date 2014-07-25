// GraphDlg.cpp: файл реализации
//

#include "stdafx.h"
#include "Spectr.h"
#include "SpectrDlg.h"
#include "GraphDlg.h"
#include "afxdialogex.h"
#include "MyDefines.h"

#include <math.h>
#include <fstream>

using std::ifstream;
using std::ios;

#define X				0
#define Y				1
#define ERR_Y			2

// диалоговое окно CGraphDlg

IMPLEMENT_DYNAMIC(CGraphDlg, CDialog)

CGraphDlg::CGraphDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGraphDlg::IDD, pParent)
{
	
}

CGraphDlg::CGraphDlg(BYTE graphID,CString dirName,CWnd* pParent /*=NULL*/)
	: CDialog(CGraphDlg::IDD, pParent),m_graphID(graphID),m_GraphFileDir(dirName)
{
	// обнулим граничные точки скана
	ZeroMemory(m_MinMaxScanPoints,102*sizeof(float));
}

CGraphDlg::~CGraphDlg()
{
}

void CGraphDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGraphDlg, CDialog)
	
	ON_WM_PAINT()
	ON_COMMAND(ID_BAR, &CGraphDlg::OnBar)
	ON_COMMAND(ID_DATAPOINTS, &CGraphDlg::OnDatapoints)
END_MESSAGE_MAP()




void CGraphDlg::OnCancel()
{
	// TODO: добавьте специализированный код или вызов базового класса
	DestroyWindow();
}


void CGraphDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// получим размер клиентской области
	RECT rect;
	this->GetClientRect(&rect);
	// зададим максимальные и минимальный точки графика, и заведем переменную для масштаба
	float minMaxPoints[4]={0.}, scale[2]={0.};
	float dataPoints[1000]={NULL}; // 0 это х, 1 - у,2 - ошибка, 
	// прочтем данные из файла
	CString dialogName;
	int N(0);
	if(m_graphID == SCAN_GRAPH)
		N = ReadGraphDataFile(m_GraphFileDir + _T("\\ScanData.txt"), minMaxPoints, dataPoints); // N количество точек для отрисовки
	if(m_graphID == OSCIL_GRAPH)
		N = ReadGraphDataFile(m_GraphFileDir + _T("\\OscilData.txt"), minMaxPoints, dataPoints); // N количество точек для отрисовки
	switch( N )
	{
	case -1:
		dc.SetBkColor(RGB(255,255,0));
		dc.TextOutW(int(rect.right/2)-70,int(rect.bottom/2)-10,_T("Can't open graph file!"));
		return;
	case 0:
		dc.SetBkColor(RGB(255,255,0));
		dc.TextOutW(int(rect.right/2)-70,int(rect.bottom/2)-10,_T("File is empty!"));
		return;
	case 1:
		// рисуем гистограмму
		if(m_IsBarPoint)
		{
			// нарисуем оси координат и получим значения начала координат и масштаб
			CPoint origin = DrawRefFrame(&dc, minMaxPoints, rect, scale);
			//посчитаем толшину бара
			int barWidth = 10;
			// создадим жирную кисть для бара
			LOGBRUSH logBrush;
			logBrush.lbStyle = BS_SOLID;
			logBrush.lbColor = RGB(0,0,0);
			CPen pen(PS_SOLID|PS_GEOMETRIC|PS_ENDCAP_SQUARE, barWidth, &logBrush);
			CPen *pOldPen = dc.SelectObject(&pen);
			dc.MoveTo(rect.right/2,rect.bottom);
			dc.LineTo(rect.right/2,rect.bottom);
			dc.SelectObject(pOldPen);
			return;
		}
	}
	// нарисуем оси координат и получим значения начала координат и масштаб
	CPoint origin = DrawRefFrame(&dc, minMaxPoints, rect, scale);
	// выведем данные
	CPoint point;
	// рисуем гистограмму
	if(m_IsBarPoint)
	{
		//посчитаем толшину бара
		int barWidth = int((minMaxPoints[2]-minMaxPoints[0])*scale[0]/(N-1))+1;
		if(barWidth >50) barWidth = 30;
		// создадим жирную кисть для бара
		LOGBRUSH logBrush;
		logBrush.lbStyle = BS_SOLID;
		logBrush.lbColor = RGB(0,0,200);
		CPen pen(PS_SOLID|PS_GEOMETRIC|PS_ENDCAP_SQUARE, barWidth, &logBrush);
		CPen *pOldPen = dc.SelectObject(&pen);
		for(int i(0);i,i<N;i++)
		{
			point.x = int((dataPoints[3*i+X]-minMaxPoints[0])*scale[0]);
			point.y = int((dataPoints[3*i+Y])*scale[1]);
			point+=origin;
			dc.MoveTo(point.x,origin.y);
			dc.LineTo(point);
		}
		dc.SelectObject(pOldPen);
	}
	// рисуем точки с ошибкой
	else
	{
		CBrush pointBrush;
		CPen pointPen(PS_SOLID,1,RGB(0,0,200));
		CPen *pOldPen = dc.SelectObject(&pointPen);
		pointBrush.CreateSolidBrush(RGB(0,0,200));
		CBrush *oldBrush = dc.SelectObject(&pointBrush);
		for(int i(0);i<N;i++)
		{
			point.x = int((dataPoints[3*i+X]-minMaxPoints[0])*scale[0]);
			point.y = int((dataPoints[3*i+Y])*scale[1]);
			DrawDataPoint(&dc,origin + point ,dataPoints[3*i+ERR_Y]*scale[1]);
		}
		// вернем старую кисть
		dc.SelectObject(oldBrush);
		dc.SelectObject(pOldPen);
	}
	// Не вызывать CDialog::OnPaint() для сообщений рисования
}
inline void CGraphDlg::DrawDataPoint(CPaintDC * pDC, CPoint point, int errorY)
{
		int radPoint(3);
		// рисование точек данных
		pDC->Ellipse(point.x-radPoint,point.y-radPoint,point.x+radPoint,point.y+radPoint);
		//нарисуем ошибку
		pDC->MoveTo(point.x-radPoint,point.y-errorY);
		pDC->LineTo(point.x+radPoint,point.y-errorY);
		pDC->MoveTo(point.x,point.y-errorY);
		pDC->LineTo(point.x,point.y+errorY);
		pDC->MoveTo(point.x-radPoint,point.y+errorY);
		pDC->LineTo(point.x+radPoint,point.y+errorY);
}
inline CPoint CGraphDlg::DrawRefFrame(CPaintDC * pDC,float *minMAXPoints, RECT canvasRect , float *outScale)
{
	CBrush brush;
	CPen pen;
	CPoint origin, vertAxeEnd(50,15), horizAxeEnd(canvasRect.right-35,canvasRect.bottom - 30); //frameWidth.x - отступ от рамки под осью
									//frameWidth.y - растояние от торца стрелки оси до границы рамки на которую она указывает
	// масштаб
	if(minMAXPoints[2] != minMAXPoints[0])
	{
		(m_graphID == SCAN_GRAPH) ? outScale[0] = (horizAxeEnd.x - vertAxeEnd.x-30)/(m_MinMaxScanPoints[1]-m_MinMaxScanPoints[0])
									:outScale[0] = (horizAxeEnd.x - vertAxeEnd.x-30)/(minMAXPoints[2]-minMAXPoints[0]);
		outScale[1] = -(horizAxeEnd.y - vertAxeEnd.y-25)/(minMAXPoints[3]);
	}
	// зададим начало координат
	(m_graphID == SCAN_GRAPH) ? origin.SetPoint(vertAxeEnd.x+10,horizAxeEnd.y)
								: origin.SetPoint(vertAxeEnd.x,horizAxeEnd.y);
	brush.CreateSolidBrush(RGB(255,255,255));
	CBrush *pOldBrush = pDC->SelectObject(&brush);
	//инициализируем перо
	pen.CreatePen(PS_SOLID,3,RGB(0,0,0));
	// закрасим клиентскую область в белый цвет
	pDC->Rectangle(&canvasRect);
	CPen *pOldPen = pDC->SelectObject(&pen);
	// рисуем оси координат
	pDC->MoveTo(vertAxeEnd);
	pDC->LineTo(vertAxeEnd.x-5,vertAxeEnd.y+10);
	pDC->MoveTo(vertAxeEnd.x+5,vertAxeEnd.y+10);
	pDC->LineTo(vertAxeEnd);
	pDC->LineTo(vertAxeEnd.x,horizAxeEnd.y);
	pDC->LineTo(horizAxeEnd);
	pDC->LineTo(horizAxeEnd.x-10,horizAxeEnd.y-5);
	pDC->MoveTo(horizAxeEnd.x-10,horizAxeEnd.y+5);
	pDC->LineTo(horizAxeEnd);
	// нарисуем шкалу по оси х
	if(m_graphID == SCAN_GRAPH)
	{
		CString str(_T(""));
		pDC->SetBkColor(RGB(255,255,255));
		pDC->TextOutW(horizAxeEnd.x,horizAxeEnd.y+8,_T("[mm]"));
		pDC->MoveTo(origin.x, horizAxeEnd.y);
		pDC->LineTo(origin.x, horizAxeEnd.y+10);
		str.Format(_T("%i"),int(m_MinMaxScanPoints[0]));
		pDC->TextOutW(origin.x - 12,horizAxeEnd.y+12,str);
		WORD i(1);
		while(m_MinMaxScanPoints[i] != 0.)
		{
			pDC->MoveTo(origin.x + int((m_MinMaxScanPoints[i]-m_MinMaxScanPoints[0])*outScale[0]), horizAxeEnd.y);
			pDC->LineTo(origin.x + int((m_MinMaxScanPoints[i]-m_MinMaxScanPoints[0])*outScale[0]), horizAxeEnd.y+10);
			str.Format(_T("%i"),int(m_MinMaxScanPoints[i]));
			pDC->TextOutW(origin.x + int((m_MinMaxScanPoints[i]-m_MinMaxScanPoints[0])*outScale[0])-12,horizAxeEnd.y+12,str);
			i++;
		}
	}
	else 
	{
		pDC->SetBkColor(RGB(255,255,255));
		pDC->TextOutW(vertAxeEnd.x + (horizAxeEnd.x - vertAxeEnd.x)/2 - 30,horizAxeEnd.y+8,_T("[channels]"));
		CPen penGrid(PS_SOLID,1,RGB(0,0,0));
		pDC->SelectObject(&penGrid);
		const WORD n(12u);
		float stp = (minMAXPoints[2]-minMAXPoints[0])/n;
		for(WORD i(0); i<=n; i++)
		{
			pDC->MoveTo(origin.x + int(stp*i*outScale[0]), origin.y);
			pDC->LineTo(origin.x + int(stp*i*outScale[0]), vertAxeEnd.y+10);
		}
	}
	// нарусуе шкалу по оси у
	CPen penGrid(PS_SOLID,1,RGB(0,0,0));
	CString str(_T(""));
	//pDC->SetBkColor(RGB(255,255,255));
	pDC->TextOutW(vertAxeEnd.x-43,vertAxeEnd.y-12,_T("[cps]"));
	pDC->SelectObject(&penGrid);
	float stp = minMAXPoints[3]/5;
	for(WORD i(0); i<=5; i++)
	{
		pDC->MoveTo(vertAxeEnd.x-10,horizAxeEnd.y+int(stp*i*outScale[1]));
		pDC->LineTo(horizAxeEnd.x-10,horizAxeEnd.y+int(stp*i*outScale[1]));
		str.Format(_T("%.2f"),stp*i);
		pDC->TextOutW(vertAxeEnd.x-45,horizAxeEnd.y+int(stp*i*outScale[1])-15,str);
	}
	//востановим началные установки
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
	return origin;
}
inline int CGraphDlg::ReadGraphDataFile(CString fileName, float *outMinMaxPoints, float *outDataPoints)
{
	int num(0);
	float x,y,errY;
	ifstream inClientData(fileName, ios::in);
	if(!inClientData) return -1;
	
	if(inClientData>>x>>y>>errY)
	{
		//    xMin                 xMan
		outMinMaxPoints[0] = outMinMaxPoints[2] = x;
		//    yMin                yMax
		outMinMaxPoints[1] = outMinMaxPoints[3] = y;
	}
	else 
	{
		inClientData.close();
		return 0;
	}
	if(!(inClientData>>x>>y>>errY))
	{
		outDataPoints[X] = x;  //  x
		outDataPoints[Y] = y;//  y
		outDataPoints[ERR_Y] = errY;
		inClientData.close();
		// если известно задание то сразу забъем граничные точки скана
		if(m_graphID == SCAN_GRAPH && m_MinMaxScanPoints[0] != m_MinMaxScanPoints[1])
		{
			outMinMaxPoints[0] = m_MinMaxScanPoints[0];
			outMinMaxPoints[2] = m_MinMaxScanPoints[1];
		}
		return 1;
	}
	inClientData.seekg(0);
	while(inClientData>>x>>y>>errY) 
	{	
		outDataPoints[3*num+X] = x;  //  x
		outDataPoints[3*num+Y] = y;//  y
		outDataPoints[3*num+ERR_Y] = errY;// error y
		//поиск граничных точек
		if(outMinMaxPoints[0]>x) outMinMaxPoints[0] = x; 
		else 
			if(outMinMaxPoints[2]<x) outMinMaxPoints[2] = x;
		if(outMinMaxPoints[1]>y) outMinMaxPoints[1] = y;
		else 
			if(outMinMaxPoints[3]<y) outMinMaxPoints[3] = y;
		num++;
	}
	// если известно задание то сразу забъем граничные точки скана
	if(m_graphID == SCAN_GRAPH && m_MinMaxScanPoints[0] != m_MinMaxScanPoints[1])
	{
		outMinMaxPoints[0] = m_MinMaxScanPoints[0];
		outMinMaxPoints[2] = m_MinMaxScanPoints[1];
	}
	// закроем файл
	inClientData.close();
	return num;
}

void CGraphDlg::OnBar()
{
	m_IsBarPoint = true;
	this->RedrawWindow();
	// TODO: добавьте свой код обработчика команд
}


void CGraphDlg::OnDatapoints()
{
	m_IsBarPoint = false;
	this->RedrawWindow();
	// TODO: добавьте свой код обработчика команд
}


BOOL CGraphDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if(m_graphID == SCAN_GRAPH)
	{
		this->SetWindowTextW(_T("Scan"));
		m_IsBarPoint = false;
		SetMinMaxScanPoints();
	}
	if(m_graphID == OSCIL_GRAPH)
	{
		this->SetWindowTextW(_T("Oscillations"));
		m_IsBarPoint = true;
	}
	// TODO:  Добавить дополнительную инициализацию

	return TRUE;  // return TRUE unless you set the focus to a control
	// Исключение: страница свойств OCX должна возвращать значение FALSE
}


// задать рабочаю директорию
inline void CGraphDlg::SetDataDirectory(CString dirName)
{
	m_GraphFileDir = dirName;
}
void CGraphDlg::SetMinMaxScanPoints(void)
{
	WORD point(1);
	m_MinMaxScanPoints[0] = m_MinMaxScanPoints[1] = CSpectrDlg::m_TaskArray[CARRIAGE_POSITION][0];
	while((CSpectrDlg::m_TaskArray[CHOPPER_FREQUENCY][point]+
				CSpectrDlg::m_TaskArray[HSM_FREQUENCY][point]+
				CSpectrDlg::m_TaskArray[CARRIAGE_POSITION][point]+
				CSpectrDlg::m_TaskArray[MEASUREMENT_TIME][point])!=0)
	{
		//поиск граничных точек
		if(m_MinMaxScanPoints[0] > CSpectrDlg::m_TaskArray[CARRIAGE_POSITION][point])
			m_MinMaxScanPoints[0] = CSpectrDlg::m_TaskArray[CARRIAGE_POSITION][point]; 
		else 
			if(m_MinMaxScanPoints[1] < CSpectrDlg::m_TaskArray[CARRIAGE_POSITION][point]) 
			m_MinMaxScanPoints[1] = CSpectrDlg::m_TaskArray[CARRIAGE_POSITION][point];
		point++;
	}
	// заполним массив только отличными друг от друга точками
	// при этом первые две оставим граничными
	if(m_MinMaxScanPoints[0] != m_MinMaxScanPoints[1] && point>2)
	{
		WORD difNum(2);
		for(WORD i(0);i<point;i++)
		{
			WORD j(0);
			for(j;j<difNum;j++)
				if(CSpectrDlg::m_TaskArray[CARRIAGE_POSITION][i] == m_MinMaxScanPoints[j])
					break;
				else;
			if(j == difNum) m_MinMaxScanPoints[difNum++] = CSpectrDlg::m_TaskArray[CARRIAGE_POSITION][i];	
		}
	}
}

