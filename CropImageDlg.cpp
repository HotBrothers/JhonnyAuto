// CropImageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "JhonnyAuto.h"
#include "CropImageDlg.h"

#include "JhonnyMain.h"
#include "afxdialogex.h"


// CropImageDlg dialog

IMPLEMENT_DYNAMIC(CropImageDlg, CDialogEx)
	
BEGIN_MESSAGE_MAP(CropImageDlg, CDialogEx)
		ON_WM_PAINT()
		ON_WM_LBUTTONDOWN()
		ON_WM_LBUTTONUP()
		ON_WM_MOUSEMOVE()
		ON_WM_CANCELMODE()
END_MESSAGE_MAP()




CropImageDlg::CropImageDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CropImageDlg::IDD, pParent)
{
	saveFileName = "";
	isInitial = false;
	hBit = NULL;

	//jCore = new JhonnyAutoCore(&((JhonnyMain*)AfxGetMainWnd())->searchRectPoint, 0.85);
}


CropImageDlg::~CropImageDlg()
{
	delete(jCore);

}




BOOL CropImageDlg::OnInitDialog()
{
	if(StrCmpW(saveFileName, _T("")) == 0)
	{
		AfxMessageBox(_T("���� �� ���� �̸��� �������� �ʾҽ��ϴ�."));
		OnCancel();
	}

	
	//ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	//ScreenHeight = GetSystemMetrics(SM_CYSCREEN);
	//this->SetWindowPos(&CWnd::wndTopMost, 0, 0, SEARCH_RECT_WIDTH, SEARCH_RECT_HEGIHT, NULL);
	this->MoveWindow(0, 0, SEARCH_RECT_WIDTH + GetSystemMetrics(SM_CXEDGE)*8, SEARCH_RECT_HEGIHT + GetSystemMetrics(SM_CYCAPTION) +
                                           GetSystemMetrics(SM_CYEDGE)*8);
	this->CenterWindow();
	//this->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0,SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);

	
	CStatic *p_lamp_image = (CStatic *)GetDlgItem(IDC_STATIC_SCREENSHOT);
	//CBitmap lamp_image;
	
	//SaveToFile(&hBit);
	RECT cropRT = {0,};
	RECT rectRT = ((JhonnyMain*)AfxGetMainWnd())->getDlgRectRect();
	RECT targetMainRect = {0,};
	if(IsWindow(((JhonnyMain*)AfxGetMainWnd())->pTargetMainWindow->GetSafeHwnd()) == false)
	{
		AfxMessageBox(_T("���� Ÿ�� �����찡 �����ϴ�. ���ο� Ÿ�� ������� ��ȯ�մϴ�."));
		((JhonnyMain*)AfxGetMainWnd())->setTargetMainWndFromRectDlg();
	}
	((JhonnyMain*)AfxGetMainWnd())->pTargetMainWindow->GetWindowRect(&targetMainRect);
	//((JhonnyMain*)AfxGetMainWnd())->pTargetMainWindow->ClientToScreen(&targetMainRect);
	HWND targetMainWnd = ((JhonnyMain*)AfxGetMainWnd())->pTargetMainWindow->GetSafeHwnd();
	POINT distance;
	distance.x = targetMainRect.left - rectRT.left;
	distance.y = targetMainRect.top - rectRT.top;
	cropRT.left   = distance.x;
	cropRT.top    = distance.y;
	cropRT.right  = (rectRT.right - rectRT.left) + distance.x;
	cropRT.bottom = (rectRT.bottom - rectRT.top) + distance.y;
	jCore->captureScreen(targetMainWnd, cropRT, &hBit);
	//lamp_image.Attach(hBit);


	HBITMAP h_old_bitmap = p_lamp_image->SetBitmap(hBit);
 
	// Picture ��Ʈ���� ������ ����ϴ� �̹����� �־��ٸ� �����Ѵ�.
	if(h_old_bitmap != NULL) ::DeleteObject(h_old_bitmap);

 
	// Picture ��Ʈ�ѿ� �̹����� �����ϱ� ���ؼ� �����ߴ� �̹����� Picture ��Ʈ���� ����ϱ�
	// ������ lamp_image ��ü�� ����Ǹ鼭 �������� �ʵ��� ������ �����Ѵ�.
	// �� �ڵ带 ������� �ʾƵ� �Ǵ� ��ó�� �������� �ش� Picture ��Ʈ���� �������ٰ�
	// �ٽ� ���̰� �Ǵ� ��� �׸��� �׷����� �ʴ´�. ���� �� ����ؾ� �մϴ�.
	//lamp_image.Detach();
	/*
	CBitmap bitmap;
	CDC* pDC = this->GetDC();

	CImage image;
	image.Attach(bitmap);
	image.Save(imageDest, Gdiplus::ImageFormatBMP);
	*/


	m_bDragging = false;
	
	//setScr

	return true;

}

void CropImageDlg::OnMouseLeave()
{
	CDialog::OnMouseLeave();
	OnCancel();
}



void CropImageDlg::OnPaint() 
{
    CPaintDC dc(this); // device context for painting
    if (IsIconic()) {
 
    } else {
        // Ŭ���̾�Ʈ DC�� �簢���� �׸���.
		/*
		CStatic *p_lamp_image = (CStatic *)GetDlgItem(IDC_STATIC_SCREENSHOT);
		CDC* p = p_lamp_image->GetWindowDC();
		p_lamp_image->ReleaseDC(p);
		*/
		
		/*
       CDC MemDC;
		BITMAP bmpInfo;
		HBITMAP hBit = NULL;
		CBitmap bmp;
		CBitmap* pOldBmp = NULL;
		JhonnyAutoCore jac;

		MemDC.CreateCompatibleDC(&dc);
		jac.captureScreen(&hBit);
		bmp.Attach(hBit);
		bmp.GetBitmap(&bmpInfo);
		pOldBmp = MemDC.SelectObject(&bmp);
		dc.BitBlt(0,0,bmpInfo.bmWidth,bmpInfo.bmHeight, &MemDC, 0, 0, SRCCOPY);
		MemDC.SelectObject(pOldBmp);
		lamp_image.Detach();
		*/
        // CDialog::OnPaint();
    }
} 


void CropImageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_SCREENSHOT, imgShot);
	
}




// CropImageDlg message handlers


void CropImageDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CDialogEx::OnLButtonDown(nFlags, point);
	SetCapture();

    m_initialPoint = point;
    
    CRect rect(point, point);
    
    CDC *pDC = GetDC();
    pDC->DrawDragRect(&rect, CSize(1,1), NULL, CSize(1,1), NULL, NULL);
    m_lastRect = rect;
    ReleaseDC(pDC);

	
}


void CropImageDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (GetCapture() == this)
    {
		
        CDC *pDC = GetDC();
		
        CRect rect(0,0,0,0);
		pDC->DrawDragRect(rect, CSize(1,1), &m_lastRect, CSize(1,1), NULL, NULL);
		if(m_lastRect.Width() < 40 || m_lastRect.Height() < 40)
		{

			
			CFont Font;
			CFont* pOldFont;
			Font.CreateFont(13,0,0,0,0,FALSE,FALSE,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,
					CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH, _T("����ü"));
			pOldFont = (CFont*)pDC->SelectObject(&Font);
			
			pDC->SetTextColor(RGB(42,128,185));
			pDC->SetBkMode( TRANSPARENT );
			pDC->DrawText( _T("�ʹ� �۽��ϴ�"), CRect( point.x, point.y, point.x+200+1, point.y+20+1 ), DT_VCENTER | DT_SINGLELINE  );
			
			
			pDC->SetTextColor(RGB(52,152,219));
			pDC->SetBkMode( TRANSPARENT );
			pDC->DrawText( _T("�ʹ� �۽��ϴ�"), CRect( point.x, point.y, point.x+200, point.y+20 ), DT_VCENTER | DT_SINGLELINE  );

			
			pDC->SelectObject(pOldFont);
			Font.DeleteObject();
			
			ReleaseDC(pDC);
			ReleaseCapture();
			CDialogEx::OnLButtonUp(nFlags, point);
			return;
		}

		if(AfxMessageBox(_T("������ �������� ����Ͻðڽ��ϱ�?"),MB_OKCANCEL, 0) == 1)
		{
			TCHAR path[MAX_PATH];
			TCHAR imagePath[MAX_PATH];
			TCHAR infoPath[MAX_PATH];
			GetCurrentDirectory(MAX_PATH, path);
			StrCatW(path, _T("\\"));
			JhonnyMain* pPrentWnd = (JhonnyMain*)GetParent()->GetParent();
			StrCatW(path, pPrentWnd->listItemDirName);
			StrCpyW(imagePath, path);
			
			CString bmpName;
			bmpName.Format(_T("\\%s.bmp"), saveFileName);
			StrCatW(imagePath, bmpName);
			//MessageBox(imagePath);
			/*
			StrCpyW(infoPath, path);
			StrCatW(infoPath, "\\temp.txt");
			*/

			
			
			IplImage* obj;
			jCore->hBitmap2Ipl(&hBit, &obj);
			cvSetImageROI(obj, cvRect(m_lastRect.left, m_lastRect.top, m_lastRect.Width(), m_lastRect.Height()));


			int len = 256;
			char ctemp[256];
			WideCharToMultiByte(CP_ACP, 0, imagePath, len, ctemp, len, NULL, NULL);
			cvSaveImage(ctemp, obj);
			cvResetImageROI(obj);
			cvReleaseImage(&obj);
			DeleteObject(hBit);

		
			EndDialog(IDOK);


		}
		else 
		{
			pDC->DrawDragRect(rect, CSize(1,1), &m_lastRect, CSize(1,1), NULL, NULL);
			 
		}
        ReleaseDC(pDC);
		ReleaseCapture();
		
		
    }

	CDialogEx::OnLButtonUp(nFlags, point);
}


void CropImageDlg::OnMouseMove(UINT nFlags, CPoint point)
{

	if (GetCapture() == this)
    {
        CRect rect(m_initialPoint, point);
        rect.NormalizeRect();

        CDC *pDC = GetDC();
        pDC->DrawDragRect(&rect, CSize(1,1), &m_lastRect, CSize(1,1), NULL, NULL);
        m_lastRect = rect;
        ReleaseDC(pDC);
    }
	/*
	else 
	{
	
		if(isInitial == false)
		{
			SetCapture();

			m_initialPoint = point;
    
			CRect rect(point, point);
    
			CDC *pDC = GetDC();
			pDC->DrawDragRect(&rect, CSize(1,1), NULL, CSize(1,1), NULL, NULL);
			m_lastRect = rect;
			CFont Font;
			CFont* pOldFont;
			Font.CreateFont(40,0,0,0,0,FALSE,FALSE,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,
					CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH, _T("����ü"));
			pOldFont = (CFont*)pDC->SelectObject(&Font);
			
			pDC->SetTextColor(RGB(0,0,0));
			pDC->SetBkMode( TRANSPARENT );
			pDC->DrawText( _T("�巡���ؼ� �̹����� ������ּ���!"), CRect( 1, 1, ScreenWidth, ScreenHeight ), DT_CENTER | DT_SINGLELINE  );
		
			
			pDC->SetTextColor(RGB(0,192,228));
			pDC->SetBkMode( TRANSPARENT );
			pDC->DrawText( _T("�巡���ؼ� �̹����� ������ּ���!"), CRect( 0, 0, ScreenWidth, ScreenHeight ), DT_CENTER | DT_SINGLELINE  );
			

			
			
			pDC->SelectObject(pOldFont);
			Font.DeleteObject();
			

		
			ReleaseDC(pDC);
			ReleaseCapture();

			isInitial = true;
		}
		
	}
	*/
	

	/*
	// TODO: Add your message handler code here and/or call default
	 if(m_bDragging)
       {
              //CDC* pDC = GetDC();

			CStatic *p_lamp_image = (CStatic *)GetDlgItem(IDC_STATIC_SCREENSHOT);
			CDC* p = p_lamp_image->GetWindowDC();

              CRect rect = CRect(m_pointStart, point);
              CRect rect2 = CRect(m_pointStart, m_pointEnd);
 
              rect.NormalizeRect();
              rect2.NormalizeRect();
			  // this->SetWindowPos(NULL, 0, 0,point.x,point.y, NULL);
			  // p->Rectangle(point.x,point.y,point.x+10, point.y+10);
              p->DrawDragRect(rect, CSize(1, 1), rect2, CSize(1, 1));
 
              m_pointEnd = point;

			  p_lamp_image->ReleaseDC(p);
       }
	   */
	CDialogEx::OnMouseMove(nFlags, point);
}


void CropImageDlg::OnCancelMode()
{

	if (GetCapture() == this)
    {
        CDC *pDC = GetDC();
        CRect rect(0,0,0,0);
        pDC->DrawDragRect(rect, CSize(1,1), &m_lastRect, CSize(1,1), NULL, NULL);
        ReleaseDC(pDC);

        ReleaseCapture();
    }
	CDialogEx::OnCancelMode();

	// TODO: Add your message handler code here
}
