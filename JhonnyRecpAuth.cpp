// JhonnyRecpAuth.cpp : implementation file
//

#include "stdafx.h"
#include "JhonnyAuto.h"
#include "JhonnyRecpAuth.h"


// JhonnyRecpAuth dialog

IMPLEMENT_DYNCREATE(JhonnyRecpAuth, CDHtmlDialog)

JhonnyRecpAuth::JhonnyRecpAuth(CWnd* pParent /*=NULL*/)
	: CDHtmlDialog(JhonnyRecpAuth::IDD, JhonnyRecpAuth::IDH, pParent)
{
	isAuthSuccess = false;
}

JhonnyRecpAuth::~JhonnyRecpAuth()
{
}

void JhonnyRecpAuth::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
	//DDX_DHtml_ElementText(pDX, _T("name"), DISPID_A_VALUE , okSign);
}


BOOL JhonnyRecpAuth::OnInitDialog()
{
	CDHtmlDialog::OnInitDialog();
	m_pBrowserApp->put_Silent(VARIANT_TRUE);
	Navigate(_T("http://karaopea.dothome.co.kr/index.html"));
	EnableAutomation();
	LPDISPATCH pDisp = GetIDispatch(FALSE);
	SetExternalDispatch(pDisp);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

BEGIN_MESSAGE_MAP(JhonnyRecpAuth, CDHtmlDialog)
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(JhonnyRecpAuth)
	DHTML_EVENT_ONCLICK(_T("ButtonOK"), OnButtonOK)
	DHTML_EVENT_ONCLICK(_T("ButtonCancel"), OnButtonCancel)
END_DHTML_EVENT_MAP()

BEGIN_DISPATCH_MAP(JhonnyRecpAuth, CDHtmlDialog)
	DISP_FUNCTION(JhonnyRecpAuth, "Test", Test, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()


void JhonnyRecpAuth::Test()
{
	isAuthSuccess = true;
}



// JhonnyRecpAuth message handlers

HRESULT JhonnyRecpAuth::OnButtonOK(IHTMLElement* /*pElement*/)
{
	OnOK();
	return S_OK;
}

HRESULT JhonnyRecpAuth::OnButtonCancel(IHTMLElement* /*pElement*/)
{
	OnCancel();
	return S_OK;
}


BOOL JhonnyRecpAuth::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
	{
		return TRUE;
	}
	else if( pMsg->message == WM_RBUTTONDOWN )
    {      
        return TRUE;
     }
	return CDHtmlDialog::PreTranslateMessage(pMsg);
}


//BOOL JhonnyRecpAuth::OnAmbientProperty(COleControlSite* pSite, DISPID dispid, VARIANT* pvar)
//{
//	// TODO: Add your specialized code here and/or call the base class
//	 if (pvar && dispid == DISPID_AMBIENT_DLCONTROL) {
//        V_VT(pvar) = VT_I8;
//        V_I8(pvar) =
//            DLCTL_DLIMAGES|
//            DLCTL_VIDEOS|
//            DLCTL_BGSOUNDS|
//            DLCTL_NO_SCRIPTS|
//            0;
//        return TRUE;
//    }
//    else
//    {
//       return CDHtmlDialog::OnAmbientProperty(pSite, dispid, pvar);
//    }
//	
//}
