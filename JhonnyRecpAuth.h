#pragma once

#ifdef _WIN32_WCE
#error "CDHtmlDialog is not supported for Windows CE."
#endif 

// JhonnyRecpAuth dialog

class JhonnyRecpAuth : public CDHtmlDialog
{
	DECLARE_DYNCREATE(JhonnyRecpAuth)

public:
	JhonnyRecpAuth(CWnd* pParent = NULL);   // standard constructor
	virtual ~JhonnyRecpAuth();
// Overrides
	HRESULT OnButtonOK(IHTMLElement *pElement);
	HRESULT OnButtonCancel(IHTMLElement *pElement);

	bool isAuthSuccess;

// Dialog Data
	enum { IDD = IDD_Auth, IDH = IDR_HTML_JHONNYRECPAUTH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	void Test();
	BOOL IsExternalDispatchSafe() { return TRUE; }
	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
	DECLARE_DISPATCH_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
//	virtual BOOL OnAmbientProperty(COleControlSite* pSite, DISPID dispid, VARIANT* pvar);
};
