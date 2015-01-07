#pragma once

/*
#define WS_EX_LAYERED          0x00080000 
#define LWA_COLORKEY            0x00000001 
#define LWA_ALPHA              0x00000002 
#define ULW_COLORKEY            0x00000001 
#define ULW_ALPHA              0x00000002 
#define ULW_OPAQUE              0x00000004 
*/
// JhonnyLoadingDlg dialog

class JhonnyLoadingDlg : public CDialog
{
	DECLARE_DYNAMIC(JhonnyLoadingDlg)

public:
	JhonnyLoadingDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~JhonnyLoadingDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_LOADING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};
