// InputMouse.cpp : implementation file
//

#include "stdafx.h"
#include "JhonnyAuto.h"
#include "JhonnyMain.h"
#include "InputMouse.h"
#include "afxdialogex.h"
#include "JhonnyAutoCore.h"


// InputMouse dialog

IMPLEMENT_DYNAMIC(InputMouse, CDialogEx)

InputMouse::InputMouse(CWnd* pParent /*=NULL*/)
	: CDialogEx(InputMouse::IDD, pParent)
{
	x = y = 0;
	hBit = NULL;
	//jCore = new JhonnyAutoCore(&((JhonnyMain*)AfxGetMainWnd())->searchRectPoint, 0.85);
}

InputMouse::~InputMouse()
{
	((JhonnyMain*)AfxGetMainWnd())->rectDlg->ShowWindow(true);
}

void InputMouse::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(InputMouse, CDialogEx)
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// InputMouse message handlers


BOOL InputMouse::OnInitDialog()
{
	
	CDialogEx::OnInitDialog();
	((JhonnyMain*)AfxGetMainWnd())->rectDlg->ShowWindow(false);

	//int ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	//int ScreenHeight = GetSystemMetrics(SM_CYSCREEN);
	//this->SetWindowPos(NULL, 0, 0, ScreenWidth, ScreenHeight, NULL);

	this->MoveWindow(0, 0, SEARCH_RECT_WIDTH + GetSystemMetrics(SM_CXEDGE)*8, SEARCH_RECT_HEGIHT + GetSystemMetrics(SM_CYCAPTION) +
                                           GetSystemMetrics(SM_CYEDGE)*8);
	this->CenterWindow();

	
	CStatic *p_lamp_image = (CStatic *)GetDlgItem(IDC_STATIC_INPUT);
	CBitmap lamp_image;
	
	RECT rect = {0,};

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



	
	
	HBITMAP h_old_bitmap = p_lamp_image->SetBitmap(hBit);
 
	// Picture ��Ʈ���� ������ ����ϴ� �̹����� �־��ٸ� �����Ѵ�.
	if(h_old_bitmap != NULL) ::DeleteObject(h_old_bitmap);

 
	// Picture ��Ʈ�ѿ� �̹����� �����ϱ� ���ؼ� �����ߴ� �̹����� Picture ��Ʈ���� ����ϱ�
	// ������ lamp_image ��ü�� ����Ǹ鼭 �������� �ʵ��� ������ �����Ѵ�.
	// �� �ڵ带 ������� �ʾƵ� �Ǵ� ��ó�� �������� �ش� Picture ��Ʈ���� �������ٰ�
	// �ٽ� ���̰� �Ǵ� ��� �׸��� �׷����� �ʴ´�. ���� �� ����ؾ� �մϴ�.
	//lamp_image.Detach();
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void InputMouse::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	x = point.x;
	y=  point.y;

	CDialogEx::OnOK();
	//CDialogEx::OnLButtonUp(nFlags, point);
	((JhonnyMain*)AfxGetMainWnd())->rectDlg->ShowWindow(true);
}


BOOL InputMouse::OnEraseBkgnd(CDC* pDC)
{

	

	return true;
	//	return CDialogEx::OnEraseBkgnd(pDC);
}
