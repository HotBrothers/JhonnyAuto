// EventDlgTouch.cpp : implementation file
//

#include "stdafx.h"
#include "JhonnyAuto.h"
#include "JhonnyMain.h"
#include "EventDlgTouch.h"
#include "afxdialogex.h"
#include "InputMouse.h"

// EventDlgTouch dialog

IMPLEMENT_DYNAMIC(EventDlgTouch, CDialogEx)

EventDlgTouch::EventDlgTouch(CWnd* pParent /*=NULL*/)
	: CDialogEx(EventDlgTouch::IDD, pParent)
	, editCoordX(_T(""))
	, editCoordY(_T(""))
	, checkIsRight(FALSE)
	, checkIsAbsolute(FALSE)
	, editTouchName(_T(""))
	, checkIsDrag(FALSE)
	, editDragCoordX(_T(""))
	, editDragCoordY(_T(""))
{
	modTouch = NULL;
	isGetMouseCoordMode = false;
	checkIsAbsolute = false;
}

EventDlgTouch::~EventDlgTouch()
{
}

void EventDlgTouch::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TOUCH_X, editCoordX);
	DDX_Text(pDX, IDC_EDIT_TOUCH_Y, editCoordY);
	DDX_Check(pDX, IDC_CHECK_TOUCH_IS_RIGHT, checkIsRight);
	DDX_Check(pDX, IDC_CHECK_ABS, checkIsAbsolute);
	DDX_Control(pDX, IDC_BUTTON_GET_COORD, btnGetCoord);
	DDX_Text(pDX, IDC_EDIT_TOUCH_NAME, editTouchName);
	DDX_Check(pDX, IDC_CHECK_DRAG, checkIsDrag);
	DDX_Text(pDX, IDC_EDIT_TOUCH_DRAG_X, editDragCoordX);
	DDX_Text(pDX, IDC_EDIT_TOUCH_DRAG_Y, editDragCoordY);
	DDX_Control(pDX, IDC_BUTTON_DRAG_GET_COORD, btnDragGetCoord);
}


BEGIN_MESSAGE_MAP(EventDlgTouch, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_GET_COORD, &EventDlgTouch::OnBnClickedButtonGetCoord)
//	ON_WM_LBUTTONUP()
ON_BN_CLICKED(IDC_CHECK_ABS, &EventDlgTouch::OnBnClickedCheckAbs)
ON_BN_CLICKED(IDC_CHECK_DRAG, &EventDlgTouch::OnBnClickedCheckDrag)
ON_BN_CLICKED(IDC_BUTTON_DRAG_GET_COORD, &EventDlgTouch::OnBnClickedButtonDragGetCoord)
END_MESSAGE_MAP()


// EventDlgTouch message handlers


BOOL EventDlgTouch::OnInitDialog()
{
	CDialogEx::OnInitDialog();



	// TODO:  Add extra initialization here
	//btnGetCoord.setEnab
	

	if(modTouch != NULL)
	{
		CString tmp;
		if(modTouch->name == NULL)
			editTouchName.SetString(_T(""));
		else 
			editTouchName.SetString(modTouch->name);
		tmp.Format(_T("%d"), modTouch->x);
		editCoordX.SetString( tmp);
		tmp.Format(_T("%d"), modTouch->y);
		editCoordY.SetString( tmp);
		if(modTouch->isDrag)
		{
			tmp.Format(_T("%d"), modTouch->dragX);
			editDragCoordX.SetString( tmp);
			tmp.Format(_T("%d"), modTouch->dragY);
			editDragCoordY.SetString( tmp);

			GetDlgItem(IDC_STATIC_DRAG_START)->ShowWindow(TRUE);
			GetDlgItem(IDC_STATIC_DRAG_END)->ShowWindow(TRUE);
			GetDlgItem(IDC_STATIC_DRAG_ARROW)->ShowWindow(TRUE);
			GetDlgItem(IDC_STATIC_DRAG_X)->ShowWindow(TRUE);
			GetDlgItem(IDC_STATIC_DRAG_Y)->ShowWindow(TRUE);
			GetDlgItem(IDC_EDIT_TOUCH_DRAG_X)->ShowWindow(TRUE);
			GetDlgItem(IDC_EDIT_TOUCH_DRAG_Y)->ShowWindow(TRUE);
			GetDlgItem(IDC_BUTTON_DRAG_GET_COORD)->ShowWindow(TRUE);
		}
		checkIsDrag = modTouch->isDrag;
		checkIsAbsolute = modTouch->isAbsolute;
		checkIsRight = modTouch->isRightClick;
		

		UpdateData(false);
	}
	else
	{
		checkIsAbsolute = true;
		UpdateData(false);
	}
	
	
	/*
	CString tmp;
	tmp.Format("t: %d",((JhonnyMain*)AfxGetMainWnd())->searchRectPoint.x );
	OutputDebugString(tmp);
	*/
	btnGetCoord.EnableWindow(checkIsAbsolute);
	btnDragGetCoord.EnableWindow(checkIsAbsolute);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void EventDlgTouch::OnBnClickedButtonGetCoord()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	//CString temp = editTouchName;

	ShowWindow(false);
	InputMouse dlgInput;
	if(dlgInput.DoModal() == IDOK)
	{

		int tx = dlgInput.x;
		int ty = dlgInput.y;

		if(tx < 0 || ty < 0 || tx > SEARCH_RECT_WIDTH || ty > SEARCH_RECT_HEGIHT)
		{
			AfxMessageBox(_T("화면 안의 좌표를 클릭해 주세요!"));
			ShowWindow(SW_SHOW);
			return ;
		}

		CString tmp;
		tmp.Format(_T("%d"), tx );
		editCoordX.SetString( tmp);
		tmp.Format(_T("%d"), ty );
		editCoordY.SetString( tmp);

	
		//editTouchName = temp;
		UpdateData(false);
	}

//	isGetMouseCoordMode = TRUE;
	ShowWindow(true);
}


//void EventDlgTouch::OnLButtonUp(UINT nFlags, CPoint point)
//{
//	// TODO: Add your message handler code here and/or call default
//
//	CDialogEx::OnLButtonUp(nFlags, point);
//}


void EventDlgTouch::OnBnClickedCheckAbs()
{
	UpdateData();
	// TODO: Add your control notification handler code here
	checkIsAbsolute = IsDlgButtonChecked(IDC_CHECK_ABS);
	btnGetCoord.EnableWindow(checkIsAbsolute);
	btnDragGetCoord.EnableWindow(checkIsAbsolute);

	CString tmp;
	tmp.Format(_T("%d"), 0 );
	editCoordX.SetString( tmp);
	editCoordY.SetString( tmp);
	editDragCoordX.SetString( tmp);
	editDragCoordY.SetString( tmp);
	//editTouchName = temp;
	UpdateData(false);

}




void EventDlgTouch::OnBnClickedCheckDrag()
{
	// TODO: Add your control notification handler code here
	checkIsAbsolute = IsDlgButtonChecked(IDC_CHECK_DRAG);
	if(checkIsAbsolute)
	{
		GetDlgItem(IDC_STATIC_DRAG_START)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_DRAG_END)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_DRAG_ARROW)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_DRAG_X)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_DRAG_Y)->ShowWindow(TRUE);
		GetDlgItem(IDC_EDIT_TOUCH_DRAG_X)->ShowWindow(TRUE);
		GetDlgItem(IDC_EDIT_TOUCH_DRAG_Y)->ShowWindow(TRUE);
		GetDlgItem(IDC_BUTTON_DRAG_GET_COORD)->ShowWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_STATIC_DRAG_START)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_DRAG_END)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_DRAG_ARROW)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_DRAG_X)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_DRAG_Y)->ShowWindow(FALSE);
		GetDlgItem(IDC_EDIT_TOUCH_DRAG_X)->ShowWindow(FALSE);
		GetDlgItem(IDC_EDIT_TOUCH_DRAG_Y)->ShowWindow(FALSE);
		GetDlgItem(IDC_BUTTON_DRAG_GET_COORD)->ShowWindow(FALSE);
	}
	
}


void EventDlgTouch::OnBnClickedButtonDragGetCoord()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	//CString temp = editTouchName;

	ShowWindow(false);
	InputMouse dlgInput;
	if(dlgInput.DoModal() == IDOK)
	{

		int tx = dlgInput.x;
		int ty = dlgInput.y;

		if(tx < 0 || ty < 0 || tx > SEARCH_RECT_WIDTH || ty > SEARCH_RECT_HEGIHT)
		{
			AfxMessageBox(_T("화면 안의 좌표를 클릭해 주세요!"));
			ShowWindow(SW_SHOW);
			return ;
		}

		CString tmp;
		tmp.Format(_T("%d"), tx );
		editDragCoordX.SetString( tmp);
		tmp.Format(_T("%d"), ty );
		editDragCoordY.SetString( tmp);

	
		//editTouchName = temp;
		UpdateData(false);
	}
//	isGetMouseCoordMode = TRUE;
	ShowWindow(true);
}
