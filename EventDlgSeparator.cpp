// EventDlgSeparator.cpp : implementation file
//

#include "stdafx.h"
#include "JhonnyAuto.h"
#include "EventDlgSeparator.h"
#include "afxdialogex.h"


// EventDlgSeparator dialog

IMPLEMENT_DYNAMIC(EventDlgSeparator, CDialogEx)

EventDlgSeparator::EventDlgSeparator(CWnd* pParent /*=NULL*/)
	: CDialogEx(EventDlgSeparator::IDD, pParent)
	, editSeparatorName(_T(""))
{
	modSeparator = NULL;
}

EventDlgSeparator::~EventDlgSeparator()
{
}

void EventDlgSeparator::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SEPARATOR_NAME, editSeparatorName);
}


BEGIN_MESSAGE_MAP(EventDlgSeparator, CDialogEx)
	ON_BN_CLICKED(IDOK, &EventDlgSeparator::OnBnClickedOk)
END_MESSAGE_MAP()


// EventDlgSeparator message handlers

// EventDlgWait message handlers


void EventDlgSeparator::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}


BOOL EventDlgSeparator::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	if(modSeparator != NULL)
	{
		
		//CString tmp;
		//tmp.Format("%d", (int)(modWait->millisec/1000.0));
		editSeparatorName.SetString( modSeparator->name);
		UpdateData(false);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

