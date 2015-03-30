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
	DDX_Control(pDX, IDC_OPTION_HOTKEY_KEY, modOptPlay);
}


BEGIN_MESSAGE_MAP(JhonnyOptionDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &JhonnyOptionDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// JhonnyOptionDlg message handlers


BOOL JhonnyOptionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CString tmp;
	tmp.Format(_T("%f"), modOptDelay);
	editOptionDelay.SetString( tmp);
	modOptPlay.SetHotKey(vk, mod);
	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
}


void JhonnyOptionDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	modOptPlay.GetHotKey(vk, mod);
	modOptDelay = _wtof(editOptionDelay);
	CDialogEx::OnOK();
}




BOOL JhonnyOptionDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->wParam == VK_RETURN)
	{
		modOptPlay.SetHotKey(VK_RETURN, NULL);
		return FALSE;
	}
	else if(pMsg->wParam == VK_ESCAPE)
	{
		modOptPlay.SetHotKey(VK_ESCAPE, NULL);
		return FALSE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


