// ViewerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "JhonnyAuto.h"
#include "ViewerDlg.h"
#include "afxdialogex.h"
#include "MfcUtil.h"

// ViewerDlg dialog

IMPLEMENT_DYNAMIC(ViewerDlg, CDialogEx)

ViewerDlg::ViewerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(ViewerDlg::IDD, pParent)
{
	wndTarget = NULL;
	thumbnail = NULL;
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
END_MESSAGE_MAP()


// ViewerDlg message handlers


BOOL ViewerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	const int width = 800;
	const int height = 450;
	RECT wndRect = {0, 0, width, height};
	AdjustWindowRect(&wndRect, WS_OVERLAPPEDWINDOW, FALSE);
	int reWidth = wndRect.right - wndRect.left;
	int reHeight = wndRect.bottom - wndRect.top;
	this->SetWindowPos(&CWnd::wndTopMost, 0, 0, reWidth, reHeight, 0);
	

	
	
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
