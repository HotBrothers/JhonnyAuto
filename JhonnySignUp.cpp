// JhonnySignUp.cpp : implementation file
//

#include "stdafx.h"
#include "JhonnyAuto.h"
#include "JhonnySignUp.h"
#include "afxdialogex.h"
#include "RexInterface.h"


#include <wininet.h>
#include <fstream>  
#include <iostream>  
#include <string>  
#include <io.h>


// JhonnySignUp dialog

IMPLEMENT_DYNAMIC(JhonnySignUp, CDialogEx)

JhonnySignUp::JhonnySignUp(CWnd* pParent /*=NULL*/)
	: CDialogEx(JhonnySignUp::IDD, pParent)
{

}

JhonnySignUp::~JhonnySignUp()
{
}

void JhonnySignUp::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(JhonnySignUp, CDialogEx)
	ON_BN_CLICKED(IDOK, &JhonnySignUp::OnBnClickedOk)
END_MESSAGE_MAP()


// JhonnySignUp message handlers


BOOL JhonnySignUp::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	recpAuth.Create(IDD_ADware, this);
	recpAuth.SetWindowPos(NULL, 0, 0, 420, 700, SWP_SHOWWINDOW | SWP_NOZORDER);

	return TRUE;  // return TRUE unless you set the focus to a control
}



void JhonnySignUp::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	
	
	
}
