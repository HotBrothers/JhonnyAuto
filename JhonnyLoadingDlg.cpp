// JhonnyLoadingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "JhonnyAuto.h"
#include "JhonnyLoadingDlg.h"
#include "afxdialogex.h"


// JhonnyLoadingDlg dialog

IMPLEMENT_DYNAMIC(JhonnyLoadingDlg, CDialog)

JhonnyLoadingDlg::JhonnyLoadingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(JhonnyLoadingDlg::IDD, pParent)
{

}

JhonnyLoadingDlg::~JhonnyLoadingDlg()
{
}

void JhonnyLoadingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(JhonnyLoadingDlg, CDialog)
		ON_WM_SYSCOMMAND()
		ON_WM_ACTIVATE()
END_MESSAGE_MAP()


// JhonnyLoadingDlg message handlers



BOOL JhonnyLoadingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	HBITMAP hBitmap=NULL;
	hBitmap = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP_MAIN));
	((CStatic *)GetDlgItem(IDC_STATIC_LOADING))->SetBitmap(hBitmap);

	
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

BOOL JhonnyLoadingDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
	{
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}


void JhonnyLoadingDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: Add your message handler code here and/or call default
	
    if ((nID & 0xFFF0) == SC_CLOSE)
    {
		return ;
    }
	CDialog::OnSysCommand(nID, lParam);
}


void JhonnyLoadingDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);
	if( nState == WA_ACTIVE )
	{

		RECT    sttParentRect, sttClientRect;
		HWND    hParent;

		hParent = GetParent()->GetSafeHwnd();
		if( hParent == NULL ) return;

		

		::GetWindowRect( hParent, &sttParentRect );
		::GetWindowRect( this->GetSafeHwnd(), &sttClientRect );
		::SetWindowPos( this->GetSafeHwnd(), NULL
			, sttParentRect.left + ( sttParentRect.right - sttParentRect.left ) / 2 - ( sttClientRect.right - sttClientRect.left ) / 2
			, sttParentRect.top + ( sttParentRect.bottom - sttParentRect.top ) / 2 - ( sttClientRect.bottom - sttClientRect.top ) / 2
			, 0
			, 0
			, SWP_NOSIZE | SWP_NOZORDER );
		
	}
	// TODO: Add your message handler code here
}
