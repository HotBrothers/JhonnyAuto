// ViewerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "JhonnyAuto.h"

#include "ViewerDlg.h"
#include "afxdialogex.h"
#include "MfcUtil.h"

#define BTN_START_ID 15800
#define BTN_PAUSE_ID 15801
#define BTN_STOP_ID  15802

// ViewerDlg dialog

IMPLEMENT_DYNAMIC(ViewerDlg, CDialogEx)

ViewerDlg::ViewerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(ViewerDlg::IDD, pParent)
{
	wndTarget = NULL;
	thumbnail = NULL;
	viewerRectDlg = NULL;
	isPlay = true;
	isStop = false;
}

ViewerDlg::~ViewerDlg()
{
	
}

void ViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ViewerDlg, CDialogEx)
	ON_WM_CLOSE()
	ON_WM_MOVE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// ViewerDlg message handlers
// icon 15x15

BOOL ViewerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	const int width = 800;
	const int height = 480;			// capture 450 + player bar 27 + status bar 3				
	RECT wndRect = {0, 0, width, height};
	AdjustWindowRect(&wndRect, WS_OVERLAPPEDWINDOW, FALSE);
	int reWidth = wndRect.right - wndRect.left;
	int reHeight = wndRect.bottom - wndRect.top;
	this->SetWindowPos(&CWnd::wndTopMost, 0, 0, reWidth, reHeight, 0);
	
	btnStart.Create(NULL, WS_CHILD|WS_VISIBLE | BS_OWNERDRAW, CRect(30, 460,50,50), this, BTN_START_ID);
	btnStart.LoadBitmaps(IDB_BITMAP_PLAY, IDB_BITMAP_PLAY, IDB_BITMAP_PLAY, IDB_BITMAP_PLAY);
	btnStart.SetHoverBitmapID(IDB_BITMAP_PLAY_ON);  
	btnStart.SizeToContent();
	btnStart.ShowWindow(false);

	btnPause.Create(NULL, WS_CHILD|WS_VISIBLE | BS_OWNERDRAW, CRect(30, 460,50,50), this, BTN_PAUSE_ID);
	btnPause.LoadBitmaps(IDB_BITMAP_PAUSE, IDB_BITMAP_PAUSE, IDB_BITMAP_PAUSE, IDB_BITMAP_PAUSE);
	btnPause.SetHoverBitmapID(IDB_BITMAP_PAUSE_ON);  
	btnPause.SizeToContent();
	

	btnStop.Create(NULL, WS_CHILD|WS_VISIBLE | BS_OWNERDRAW, CRect(78, 460,50,50), this, BTN_STOP_ID);
	btnStop.LoadBitmaps(IDB_BITMAP_STOP, IDB_BITMAP_STOP, IDB_BITMAP_STOP, IDB_BITMAP_STOP_INV);
	btnStop.SetHoverBitmapID(IDB_BITMAP_STOP_ON);  
	btnStop.SizeToContent();

	CStatic* player = new CStatic();
	player->Create(_T("123"), WS_CHILD|WS_VISIBLE|SS_CENTER, CRect(0, 0,50,100), this);

	CStatic * stcTemp;

	stNow.Create(_T("123"), WS_CHILD|WS_VISIBLE|SS_CENTER, CRect(148, 460,50,10), this);
	stNow.SetTextColor(RGB(255,255,255)); //Changes the Edit Box text to Blue
    stNow.SetBkColor(RGB(0,0,0));  //By default your background color is the
 

	//btnStart.SetWindowPos(NULL, 15, 453, 0, 0, SWP_NOSIZE);
	
	
	return TRUE;

}

BOOL ViewerDlg::doDwmCapture(RECT src)
{
	return false;
	
	


	const int width = 800;
	const int height = 450;

	if(wndTarget == NULL)
	{
		
		return FALSE;
		
	}
	
	CString caption = _T("죠니매크로 뷰어 : ");
	CString wndName;
	CWnd* pTarget = FromHandle(wndTarget);
	pTarget->GetWindowTextW(wndName);
	caption += wndName;
	this->SetWindowText(caption);
	this->SetIcon(MfcUtil::GetAppIcon(wndTarget), false);
	
	

	HRESULT hr = S_OK;
	// Register the thumbnail
	hr = DwmRegisterThumbnail(this->GetSafeHwnd(), wndTarget, &thumbnail);
	if (SUCCEEDED(hr))
	{
		//this->SetWindowPos(NULL, 0, 0, 800, 450, SWP_NOMOVE);
		// Specify the destination rectangle size
		RECT dest = {0,};
			
		dest.left = 0;
		dest.top = 0;
		dest.right = width;
		dest.bottom = height;

		src.right = src.left + width;
		src.bottom = src.top + height;	

		// Set the thumbnail properties for use
		DWM_THUMBNAIL_PROPERTIES dskThumbProps;
		//dskThumbProps.dwFlags = DWM_TNP_SOURCECLIENTAREAONLY | DWM_TNP_VISIBLE | DWM_TNP_OPACITY | DWM_TNP_RECTDESTINATION;
		dskThumbProps.dwFlags = DWM_TNP_VISIBLE | DWM_TNP_RECTDESTINATION | DWM_TNP_RECTSOURCE | DWM_TNP_SOURCECLIENTAREAONLY | DWM_TNP_OPACITY;
		dskThumbProps.fSourceClientAreaOnly = FALSE; 
		dskThumbProps.fVisible = TRUE;
		dskThumbProps.opacity = (255 * 100)/100;
		dskThumbProps.rcSource = src;
		dskThumbProps.rcDestination = dest;

		// Display the thumbnail
		hr = DwmUpdateThumbnailProperties(thumbnail,&dskThumbProps);

		if (SUCCEEDED(hr))
		{
			//DwmFlush();
			//DwmInvalidateIconicBitmaps(this->GetSafeHwnd());
			//writeDX(this->GetSafeHwnd());
			//writeDX(this->GetSafeHwnd());
			//Capture(

				
			//InitD3D(this->GetSafeHwnd());
			//captureDX(this->GetSafeHwnd());
			// ...
			//target->ShowWindow(SW_HIDE);



			viewerRectDlg = new ViewerRectDlg();
			viewerRectDlg->Create(IDD_DIALOG_VIEWER_RECT, this);
			viewerRectDlg->ShowWindow(SW_SHOW);

		}
	}
}


//BOOL ViewerDlg::DestroyWindow()
//{
//	// TODO: Add your specialized code here and/or call the base class
//	
//	return CDialogEx::DestroyWindow();
//}


void ViewerDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	DwmUnregisterThumbnail(thumbnail);
	MfcUtil::ShowTaskbarIcon(wndMain);
	MfcUtil::ShowTaskbarIcon(wndTarget);
	::ShowWindow(wndMain, SW_RESTORE);
	::SetWindowPos(wndTarget, NULL, returnTargetWindow.left, returnTargetWindow.top, 0, 0, SWP_NOSIZE);
	CDialogEx::OnClose();
}


void ViewerDlg::OnMove(int x, int y)
{
	CDialogEx::OnMove(x, y);
	if(viewerRectDlg != NULL)
		viewerRectDlg->SetWindowPos(NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_NOSENDCHANGING );
	// TODO: Add your message handler code here
}


BOOL ViewerDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	CRect rect;
    GetClientRect(&rect);
    CBrush myBrush(RGB(26, 26, 26));    // dialog background color <- 요기 바꾸면 됨.
	CBrush myBrush2(RGB(119, 119, 119));    // dialog background color <- 요기 바꾸면 됨.
    CBrush *pOld = pDC->SelectObject(&myBrush);
	

    BOOL bRes  = pDC->PatBlt(0, 0, rect.Width(), rect.Height(), PATCOPY);

	pDC->SelectObject(&myBrush2);
	bRes  = pDC->PatBlt(0, 450, rect.Width(), 3, PATCOPY);

    pDC->SelectObject(pOld);    // restore old brush

    return bRes;                       // CDialog::OnEraseBkgnd(pDC);
	return CDialogEx::OnEraseBkgnd(pDC);
}


BOOL ViewerDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	 if (HIWORD(wParam) == BN_CLICKED) 
    { 
         switch (LOWORD(wParam))    // Button ID 
         { 
             case BTN_START_ID: 
				btnStart.ShowWindow(FALSE);
				btnPause.ShowWindow(TRUE);
				btnStop.EnableWindow(true);
				isStop = false;
				isPlay = true;
				break; 
			case BTN_PAUSE_ID: 
				btnStart.ShowWindow(TRUE);
				btnPause.ShowWindow(FALSE);
				isPlay = false;
				break;
			case BTN_STOP_ID: 
				btnStop.EnableWindow(false);
				btnStart.ShowWindow(TRUE);
				btnPause.ShowWindow(FALSE);
				isStop = true;
				isPlay = false;
				break;
         } 
    } 
	return CDialogEx::OnCommand(wParam, lParam);
}
