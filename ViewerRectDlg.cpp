// ViewerRectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "JhonnyAuto.h"
#include "ViewerRectDlg.h"
#include "afxdialogex.h"

#define WM_DRAW_POINT	(WM_USER + 1000)
#define TIMER_ID 86051501

// ViewerRectDlg dialog

IMPLEMENT_DYNAMIC(ViewerRectDlg, CDialogEx)

ViewerRectDlg::ViewerRectDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(ViewerRectDlg::IDD, pParent)
{
	m_bDraw = FALSE;
	m_bTouch = FALSE;
}

ViewerRectDlg::~ViewerRectDlg()
{
}

void ViewerRectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ViewerRectDlg, CDialogEx)
	ON_WM_TIMER()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// ViewerRectDlg message handlers


BOOL ViewerRectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	const int width = 800;
	const int height = 450;
	RECT wndRect = {0, 0, width, height};
	AdjustWindowRect(&wndRect, NULL, FALSE);
	int reWidth = wndRect.right - wndRect.left;
	int reHeight = wndRect.bottom - wndRect.top;
	this->SetWindowPos(&CWnd::wndTopMost, 0, 0, reWidth, reHeight, SWP_NOMOVE);

	
	//this->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0,SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);
	 typedef BOOL(WINAPI *SLWA)(HWND,COLORREF,BYTE,DWORD);
    SLWA pSetLayeredWindowAttributes = NULL;
    HINSTANCE hmodUSER32 = LoadLibrary(_T("USER32.DLL"));
    
    pSetLayeredWindowAttributes =    (SLWA)GetProcAddress(hmodUSER32,"SetLayeredWindowAttributes");

    HWND hwnd = this->m_hWnd;
    SetWindowLong(hwnd,GWL_EXSTYLE,GetWindowLong(hwnd,GWL_EXSTYLE) | WS_EX_LAYERED);
    BYTE byAlpha = 70; //투명도 설정
    COLORREF cr = GetSysColor(COLOR_BTNFACE);
    SetLayeredWindowAttributes( cr, byAlpha, LWA_COLORKEY );
	

	return TRUE;  
}

void ViewerRectDlg::DrawEllipse(HWND hWnd)
{
	/*
	
		*/

	PAINTSTRUCT ps;
	HDC hDC = ::BeginPaint(hWnd, &ps);
	hDC = ::GetDC(hWnd);
	if (hDC)
	{
		HPEN hNewP = NULL;
		HBRUSH hNewB = NULL;
		HPEN hOldP = NULL;
		HBRUSH hOldB = NULL;

		hNewP = CreatePen(PS_SOLID,3, RGB(0,192,228));  
		hNewB = CreateSolidBrush(RGB(230,86,122));  



		hOldP = (HPEN)SelectObject(hDC, hNewP);  
		if(m_bTouch)
			hOldB = (HBRUSH)SelectObject(hDC, hNewB);
		else
			hOldB = (HBRUSH)SelectObject(hDC, GetStockObject(NULL_BRUSH));
		
		  
		if(m_bDraw)
		{
			if(m_bTouch)
				Ellipse(hDC,m_pt.x+8,m_pt.y+8,m_pt.x-8,m_pt.y-8 );// gets pixel of 100
			else
				Rectangle(hDC, m_pt.x+20,m_pt.y+20,m_pt.x-20,m_pt.y-20);
				//Ellipse(hDC,m_pt.x+20,m_pt.y+20,m_pt.x-20,m_pt.y-20 );// gets pixel of 100

			SetTimer(TIMER_ID, 500, NULL);
			InvalidateRect(NULL);
		}

		
		SelectObject(hDC,hOldP);  //displace pen/brush from DC  
		SelectObject(hDC,hOldB);		
		DeleteObject(hNewP); //now can be deleted  
		DeleteObject(hNewB); 
		

		

		::ReleaseDC(hWnd,hDC);  
		::EndPaint(hWnd, &ps);  
		::ValidateRect(hWnd, NULL);  
		
	}
}




void ViewerRectDlg::OnPaint()
{
	DrawEllipse(this->GetSafeHwnd());
}

void ViewerRectDlg::drawingPoint(CPoint point, BOOL _m_bTouch)
{
	m_bDraw = TRUE;
	m_pt = point;
	m_bTouch = _m_bTouch;
	InvalidateRect(NULL);
}





void ViewerRectDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	CTime time = CTime::GetCurrentTime();
	switch(nIDEvent)
    {
        case TIMER_ID:
		{
			
			CPoint point(0,0);
			m_bDraw = FALSE;
			InvalidateRect(NULL);
			//CDialog::OnLButtonUp(0, point);
			
		}
        break;
    }
	CDialog::OnTimer(nIDEvent);
}