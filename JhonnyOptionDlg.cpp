// JhonnyOptionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "JhonnyAuto.h"
#include "JhonnyOptionDlg.h"
#include "afxdialogex.h"


// JhonnyOptionDlg dialog

IMPLEMENT_DYNAMIC(JhonnyOptionDlg, CDialogEx)

JhonnyOptionDlg::JhonnyOptionDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(JhonnyOptionDlg::IDD, pParent)
	, editOptionDelay(_T(""))
{

}

JhonnyOptionDlg::~JhonnyOptionDlg()
{
}

void JhonnyOptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_OPTION_DELAY, editOptionDelay);
	DDV_MaxChars(pDX, editOptionDelay, 10);
}


BEGIN_MESSAGE_MAP(JhonnyOptionDlg, CDialogEx)
END_MESSAGE_MAP()


// JhonnyOptionDlg message handlers


BOOL JhonnyOptionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CString tmp;
	tmp.Format(_T("%f"), *modOptDelay);
	editOptionDelay.SetString( tmp);
	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
}
