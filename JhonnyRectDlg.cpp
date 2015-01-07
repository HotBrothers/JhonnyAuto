// JhonnyRectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "JhonnyMain.h"
	
#include "JhonnyAuto.h"
#include "JhonnyRectDlg.h"
#include "afxdialogex.h"

#define WM_DRAW_POINT	(WM_USER + 1000)
// JhonnyRectDlg dialog

IMPLEMENT_DYNAMIC(JhonnyRectDlg, CDialog)

JhonnyRectDlg::JhonnyRectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(JhonnyRectDlg::IDD, pParent)
{
	m_bDraw = FALSE;
	isMovable = true;
}

JhonnyRectDlg::~JhonnyRectDlg()
{
}

void JhonnyRectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}



BEGIN_MESSAGE_MAP(JhonnyRectDlg, CDialog)
	ON_WM_MOVE()
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_WINDOWPOSCHANGING()
END_MESSAGE_MAP()


// JhonnyRectDlg message handlers


BOOL JhonnyRectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	this->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0,SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);
	 typedef BOOL(WINAPI *SLWA)(HWND,COLORREF,BYTE,DWORD);
    SLWA pSetLayeredWindowAttributes = NULL;
    HINSTANCE hmodUSER32 = LoadLibrary(_T("USER32.DLL"));
    
    pSetLayeredWindowAttributes =    (SLWA)GetProcAddress(hmodUSER32,"SetLayeredWindowAttributes");

    HWND hwnd = this->m_hWnd;
    SetWindowLong(hwnd,GWL_EXSTYLE,GetWindowLong(hwnd,GWL_EXSTYLE) | WS_EX_LAYERED);
    BYTE byAlpha = 70; //투명도 설정
    COLORREF cr = GetSysColor(COLOR_BTNFACE);
    SetLayeredWindowAttributes( cr, byAlpha, LWA_COLORKEY );

	
	/*
		// WS_EX_LAYERED 속성을 추가한다. 
	::SetWindowLong(m_hWnd, GWL_EXSTYLE, 
			::GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);

	// RED 색상에 대해 투명 처리 및 마우스 이벤트를 처리하지 않는다. 
	::SetLayeredWindowAttributes(m_hWnd, RGB(255, 0, 0), 
									  0, LWA_COLORKEY);

	// 윈도우를 반투명 처리한다. 
	::SetLayeredWindowAttributes(m_hWnd, 0, 100, LWA_ALPHA);
	*/

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}


void JhonnyRectDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);
	((JhonnyMain*)main)->searchRectPoint.x = x;
	((JhonnyMain*)main)->searchRectPoint.y = y;
	// TODO: Add your message handler code here
}


BOOL JhonnyRectDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
	{
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}


void JhonnyRectDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: Add your message handler code here and/or call default
	
    if ((nID & 0xFFF0) == SC_CLOSE)
    {
		return ;
    }
	CDialog::OnSysCommand(nID, lParam);
}


void JhonnyRectDlg::DrawEllipse(HWND hWnd)
{
	/*
	if(!m_bDraw)
		return ;
		*/

	PAINTSTRUCT ps;
	HDC hDC = ::BeginPaint(hWnd, &ps);
	hDC = ::GetDC(hWnd);
	if (hDC)
	{
		if(m_bDraw)
		{
			HPEN hNewP = CreatePen(PS_SOLID,3, RGB(0,0,255));  
			HBRUSH hNewB = CreateSolidBrush(RGB(255,0,0));  

			HPEN hOldP = (HPEN)SelectObject(hDC, hNewP);  
			HBRUSH hOldB = (HBRUSH)SelectObject(hDC, hNewB);  

			Ellipse(hDC,m_pt.x+10,m_pt.y+10,m_pt.x-10,m_pt.y-10 );// gets pixel of 100   

			SelectObject(hDC,hOldP);  //displace pen/brush from DC  
			SelectObject(hDC,hOldB);		

			DeleteObject(hNewP); //now can be deleted  
			DeleteObject(hNewB);  

			
		}
		::ReleaseDC(hWnd,hDC);  
		::EndPaint(hWnd, &ps);  
		::ValidateRect(hWnd, NULL);  
		
	}
}


void JhonnyRectDlg::OnPaint()
{
	DrawEllipse(this->GetSafeHwnd());
}

void JhonnyRectDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_bDraw = TRUE;
	m_pt = point;
	CDialog::OnLButtonDown(nFlags, point);
}

void JhonnyRectDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_bDraw = FALSE;

	InvalidateRect(NULL);
	CDialog::OnLButtonUp(nFlags, point);
}

void JhonnyRectDlg::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
	if(!isMovable)
		lpwndpos->flags |= SWP_NOMOVE; 
	CDialog::OnWindowPosChanging(lpwndpos);
	
	// TODO: Add your message handler code here
}
