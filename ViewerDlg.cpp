// ViewerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "JhonnyAuto.h"
#include "JhonnyMain.h"

#include "ViewerDlg.h"
#include "afxdialogex.h"
#include "MfcUtil.h"

#define BTN_START 15800
#define BTN_PAUSE 15801
#define BTN_STOP  15802
#define LVS_ACTION_ITEM 15901

// ViewerDlg dialog

JhonnyMain* main;
IMPLEMENT_DYNAMIC(ViewerDlg, CDialogEx)

ViewerDlg::ViewerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(ViewerDlg::IDD, pParent)
{
	wndTarget = NULL;
	thumbnail = NULL;
	viewerRectDlg = NULL;
	isPlay = true;
	isStop = false;
	strNowNum = _T("1");
	strTotalNum = _T(" / 1");
	strItemName = _T("");
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
	main = ((JhonnyMain*)AfxGetMainWnd());
	
	const int width = 800;
	const int height = 480;			// capture 450 + player bar 27 + status bar 3				
	RECT wndRect = {0, 0, width, height};
	AdjustWindowRect(&wndRect, WS_OVERLAPPEDWINDOW, FALSE);
	int reWidth = wndRect.right - wndRect.left;
	int reHeight = wndRect.bottom - wndRect.top;
	this->SetWindowPos(&CWnd::wndTopMost, 0, 0, reWidth, reHeight, 0);


	
	btnStart.Create(NULL, WS_CHILD|WS_VISIBLE | BS_OWNERDRAW, CRect(30, 460,50,50), this, BTN_START);
	btnStart.LoadBitmaps(IDB_BITMAP_PLAY, IDB_BITMAP_PLAY, IDB_BITMAP_PLAY, IDB_BITMAP_PLAY);
	btnStart.SetHoverBitmapID(IDB_BITMAP_PLAY_ON);  
	btnStart.SizeToContent();
	btnStart.ShowWindow(false);

	btnPause.Create(NULL, WS_CHILD|WS_VISIBLE | BS_OWNERDRAW, CRect(30, 460,50,50), this, BTN_PAUSE);
	btnPause.LoadBitmaps(IDB_BITMAP_PAUSE, IDB_BITMAP_PAUSE, IDB_BITMAP_PAUSE, IDB_BITMAP_PAUSE);
	btnPause.SetHoverBitmapID(IDB_BITMAP_PAUSE_ON);  
	btnPause.SizeToContent();
	

	btnStop.Create(NULL, WS_CHILD|WS_VISIBLE | BS_OWNERDRAW, CRect(78, 460,50,50), this, BTN_STOP);
	btnStop.LoadBitmaps(IDB_BITMAP_STOP, IDB_BITMAP_STOP, IDB_BITMAP_STOP, IDB_BITMAP_STOP_INV);
	btnStop.SetHoverBitmapID(IDB_BITMAP_STOP_ON);  
	btnStop.SizeToContent();

	//CStatic* player = new CStatic();
	//player->Create(_T("123"), WS_CHILD|WS_VISIBLE|SS_CENTER, CRect(0, 0,50,100), this);


	
	stNow = new CColorStatic();
	stNow->Create(strNowNum, WS_CHILD | WS_VISIBLE | SS_RIGHT, CRect(108, 460,138,480), this);
	stNow->SetTextColor(RGB(255,255,255)); //Changes the Edit Box text to Blue
    stNow->SetBkColor(RGB(26,26,26));  //By default your background color is the


	stTotal = new CColorStatic();
	stTotal->Create(strTotalNum, WS_CHILD | WS_VISIBLE | SS_LEFT, CRect(138, 460,178,480), this);
	stTotal->SetTextColor(RGB(142,142,142)); //Changes the Edit Box text to Blue
    stTotal->SetBkColor(RGB(26,26,26));  //By default your background color is the

	
	stItemName = new CColorStatic();
	stItemName->Create(strItemName, WS_CHILD | WS_VISIBLE | SS_LEFT | SS_WORDELLIPSIS, CRect(198, 460,800,480), this);
	stItemName->SetTextColor(RGB(236,196,70)); //Changes the Edit Box text to Blue
    stItemName->SetBkColor(RGB(26,26,26));  //By default your background color is the
 
	/*
	actionItem.Create(WS_CHILD | WS_VISIBLE | LVS_LIST | LVS_SHOWSELALWAYS |LBS_NOTIFY , CRect(188, 455,408,478), this, LVS_ACTION_ITEM);
	actionItem.InsertColumn(0, _T("아이템"), LVCFMT_LEFT, 560);
	DWORD dwExStyle = actionItem.GetExtendedStyle();
	actionItem.SetExtendedStyle ( dwExStyle | LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP );
	actionItem.ModifyStyle(0, LVS_NOCOLUMNHEADER);

	actionItem.SetBkColor(CLR_NONE);
	actionItem.SetTextBkColor(CLR_NONE);
	actionItem.SetTextBkColor(RGB(255,255,255));
	actionItem.SetImageList(&main->imglist, LVSIL_NORMAL);
	actionItem.SetImageList(&main->imglist, LVSIL_SMALL);
	
	
	int index = _ttoi(strNowNum)-1;
	actionItem.DeleteAllItems();
	LVITEM lvItem;
	TCHAR szBuffer[512];
	::ZeroMemory(&lvItem, sizeof(lvItem));
	::ZeroMemory(szBuffer, sizeof(szBuffer));

	lvItem.mask = LVIF_TEXT | LVIF_IMAGE;
	//lvItem.mask = LVIF_TEXT ;
	lvItem.iItem = index;
	lvItem.pszText = szBuffer;
	lvItem.cchTextMax = 512;
	//main->listPlaylist.GetItem(&lvItem);
	actionItem.InsertItem(&lvItem);
	*/

	/*
	//TTCHAR szBuffer[256];
	::ZeroMemory(&lvItem, sizeof(lvItem));
	//::ZeroMemory(szBuffer, sizeof(szBuffer));
	lvItem.mask = LVIF_TEXT | LVIF_IMAGE;
		
	lvItem.iItem = index;
	lvItem.iImage = index;
	lvItem.pszText = main->events[index];
	lvItem.cchTextMax = MAX_PATH;
	actionItem.InsertItem(&lvItem);
	*/
	return TRUE;

}

BOOL ViewerDlg::doDwmCapture(RECT src)
{
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
	CBrush myBrush2(RGB(236,196,70));    // dialog background color <- 요기 바꾸면 됨.
    CBrush *pOld = pDC->SelectObject(&myBrush);
	

    BOOL bRes  = pDC->PatBlt(0, 0, rect.Width(), rect.Height(), PATCOPY);

	pDC->SelectObject(&myBrush2);
	bRes  = pDC->PatBlt(0, 450, rect.Width(), 3, PATCOPY);

    pDC->SelectObject(pOld);    // restore old brush

    return bRes;                       // CDialog::OnEraseBkgnd(pDC);
	return CDialogEx::OnEraseBkgnd(pDC);
}

void ViewerDlg::doPlay()
{
	btnStart.ShowWindow(FALSE);
	btnPause.ShowWindow(TRUE);
	btnStop.EnableWindow(true);
	btnPause.RedrawWindow();
	if(isStop == true)
		main->playAndStop();
	else      // is Pause
		main->setPause(false);
			
	isStop = false;
	isPlay = true;

}

void ViewerDlg::doPause()
{
	btnStart.ShowWindow(TRUE);
	btnPause.ShowWindow(FALSE);
	btnStart.RedrawWindow();
	isPlay = false;
	main->setPause(true);
	
}

void ViewerDlg::doStop()
{
	btnStop.EnableWindow(false);
	btnPause.ShowWindow(FALSE);
	btnStart.ShowWindow(TRUE);
	btnStart.RedrawWindow();
	

	isStop = true;
	isPlay = false;
	main->playAndStop();
	
}

BOOL ViewerDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	if (HIWORD(wParam) == BN_CLICKED) 
	{ 
		switch (LOWORD(wParam))    // Button ID 
		{ 
		case BTN_START: 
			doPlay();
			break; 
		case BTN_PAUSE: 
			doPause();
			break;
		case BTN_STOP: 
			doStop();
			break;
		} 
	} 
	//Invalidate();
	return CDialogEx::OnCommand(wParam, lParam);
}


void ViewerDlg::setNowNum(CString _strNowNum)
{ 
	
	strNowNum = _strNowNum;
	int index = _ttoi(_strNowNum)-1;
	if(stNow!=NULL) 
		stNow->SetWindowTextW(strNowNum);

	LVITEM lvItem;
	TCHAR szBuffer[512];
	::ZeroMemory(&lvItem, sizeof(lvItem));
	::ZeroMemory(szBuffer, sizeof(szBuffer));

	//lvItem.mask = LVIF_TEXT;
	lvItem.mask = LVIF_TEXT | LVIF_IMAGE;
	lvItem.iItem = index;
	lvItem.pszText = szBuffer;
	lvItem.cchTextMax = 512;
	main->listPlaylist.GetItem(&lvItem);
	lvItem.iItem = 0;

	strItemName.Format(_T("%s"), szBuffer);
	if(stItemName!=NULL)
	{
		stItemName->SetWindowTextW(strItemName);
	}
	//actionItem.SetItem(&lvItem);
	//actionItem.RedrawItems(0, actionItem.GetItemCount());
	
}

void ViewerDlg::setNowTotal(CString _strTotalNum)
{ 
	CString temp;
	strTotalNum = _strTotalNum; 
	temp = _T(" / ") + strTotalNum;
	if(stTotal!=NULL) 
		stTotal->SetWindowTextW(temp);
}


