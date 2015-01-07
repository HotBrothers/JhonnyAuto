// JhonnyAdware.cpp : implementation file
//

#include "stdafx.h"
#include "JhonnyAuto.h"
#include "JhonnyAdware.h"


// JhonnyAdware dialog

IMPLEMENT_DYNCREATE(JhonnyAdware, CDHtmlDialog)

JhonnyAdware::JhonnyAdware(CWnd* pParent /*=NULL*/)
	: CDHtmlDialog(JhonnyAdware::IDD, JhonnyAdware::IDH, pParent)
{

}

JhonnyAdware::~JhonnyAdware()
{
}

void JhonnyAdware::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
}

BOOL JhonnyAdware::OnInitDialog()
{
	CDHtmlDialog::OnInitDialog();
	Navigate(_T("http://karaopea.dothome.co.kr/banner.html"));


	return TRUE;  // return TRUE  unless you set the focus to a control
}

BEGIN_MESSAGE_MAP(JhonnyAdware, CDHtmlDialog)
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(JhonnyAdware)
	DHTML_EVENT_ONCLICK(_T("ButtonOK"), OnButtonOK)
	DHTML_EVENT_ONCLICK(_T("ButtonCancel"), OnButtonCancel)
END_DHTML_EVENT_MAP()



// JhonnyAdware message handlers

HRESULT JhonnyAdware::OnButtonOK(IHTMLElement* /*pElement*/)
{
	OnOK();
	return S_OK;
}

HRESULT JhonnyAdware::OnButtonCancel(IHTMLElement* /*pElement*/)
{
	OnCancel();
	return S_OK;
}


BOOL JhonnyAdware::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_LBUTTONDOWN){
	//왼쪽 버튼이 눌렸을경우 발생할 이벤트
		const TCHAR* helpUrl = _T("http://cafe.naver.com/jhonnymacro");
		::ShellExecute(NULL, _T("open"), helpUrl, NULL, NULL, SW_SHOW);
	}
	return CDHtmlDialog::PreTranslateMessage(pMsg);
}
