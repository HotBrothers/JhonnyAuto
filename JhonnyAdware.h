#pragma once

#ifdef _WIN32_WCE
#error "CDHtmlDialog is not supported for Windows CE."
#endif 

// JhonnyAdware dialog

class JhonnyAdware : public CDHtmlDialog
{
	DECLARE_DYNCREATE(JhonnyAdware)

public:
	JhonnyAdware(CWnd* pParent = NULL);   // standard constructor
	virtual ~JhonnyAdware();
// Overrides
	HRESULT OnButtonOK(IHTMLElement *pElement);
	HRESULT OnButtonCancel(IHTMLElement *pElement);

// Dialog Data
	enum { IDD = IDD_ADware, IDH = IDR_HTML_JHONNYADWARE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
