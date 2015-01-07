// EventDlgWait.cpp : implementation file
//

#include "stdafx.h"
#include "JhonnyAuto.h"
#include "EventDlgWait.h"
#include "afxdialogex.h"


// EventDlgWait dialog

IMPLEMENT_DYNAMIC(EventDlgWait, CDialogEx)

EventDlgWait::EventDlgWait(CWnd* pParent /*=NULL*/)
	: CDialogEx(EventDlgWait::IDD, pParent)
	, editWaitTime(_T(""))
{
	modWait = NULL;
}

EventDlgWait::~EventDlgWait()
{
}

void EventDlgWait::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_WAIT_TIME, editWaitTime);
}


BEGIN_MESSAGE_MAP(EventDlgWait, CDialogEx)
	ON_BN_CLICKED(IDOK, &EventDlgWait::OnBnClickedOk)
END_MESSAGE_MAP()


// EventDlgWait message handlers


void EventDlgWait::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}


BOOL EventDlgWait::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	if(modWait != NULL)
	{
		
		CString tmp;
		tmp.Format(_T("%d"), (int)(modWait->millisec/1000.0));
		editWaitTime.SetString( tmp);
		UpdateData(false);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
