// EventDlgMove.cpp : implementation file
//

#include "stdafx.h"
#include "JhonnyAuto.h"
#include "EventDlgMove.h"
#include "afxdialogex.h"


// EventDlgMove dialog

IMPLEMENT_DYNAMIC(EventDlgMove, CDialogEx)

EventDlgMove::EventDlgMove(CWnd* pParent /*=NULL*/)
	: CDialogEx(EventDlgMove::IDD, pParent)
	, editGotoIndex(_T(""))
{
	modGoto = NULL;
}

EventDlgMove::~EventDlgMove()
{
}

void EventDlgMove::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_MOVE_GOTOINDEX, editGotoIndex);
}


BEGIN_MESSAGE_MAP(EventDlgMove, CDialogEx)
END_MESSAGE_MAP()


// EventDlgMove message handlers


BOOL EventDlgMove::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  Add extra initialization here
	
	if(modGoto != NULL)
	{
		CString tmp;
		tmp.Format(_T("%d"), modGoto->goToIndex);
		editGotoIndex.SetString( tmp );
		UpdateData(false);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}