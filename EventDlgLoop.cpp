// EventDlgLoop.cpp : implementation file
//

#include "stdafx.h"
#include "JhonnyAuto.h"
#include "EventDlgLoop.h"
#include "afxdialogex.h"


// EventDlgLoop dialog

IMPLEMENT_DYNAMIC(EventDlgLoop, CDialogEx)

EventDlgLoop::EventDlgLoop(CWnd* pParent /*=NULL*/)
	: CDialogEx(EventDlgLoop::IDD, pParent)
	, editGotoIndex(_T(""))
	, editLoopNum(_T(""))
{
	modLoop = NULL;
}

EventDlgLoop::~EventDlgLoop()
{
}

void EventDlgLoop::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_LOOP_GOTOINDEX, editGotoIndex);
	DDX_Text(pDX, IDC_EDIT_LOOP_LOOPNUM, editLoopNum);
	DDV_MaxChars(pDX, editLoopNum, 9);
}


BEGIN_MESSAGE_MAP(EventDlgLoop, CDialogEx)
END_MESSAGE_MAP()


// EventDlgLoop message handlers


BOOL EventDlgLoop::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  Add extra initialization here
	
	if(modLoop != NULL)
	{
		CString tmp;
		tmp.Format(_T("%d"), modLoop->goToIndex);
		editGotoIndex.SetString( tmp);
		tmp.Format(_T("%d"), modLoop->loopCount);
		editLoopNum.SetString( tmp );
		UpdateData(false);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
