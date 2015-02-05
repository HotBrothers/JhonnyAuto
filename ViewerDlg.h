#pragma once


// ViewerDlg dialog

class ViewerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ViewerDlg)

public:
	ViewerDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~ViewerDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_VIEWER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	
	virtual BOOL OnInitDialog();
	HTHUMBNAIL thumbnail;
	HWND wndTarget;
	HWND wndMain;
	RECT returnTargetWindow;

	void setTargetWindow(HWND hWnd){ wndTarget = hWnd;}
	void setReturnTargetWindow(RECT rect){ returnTargetWindow = rect;}
	void setMainWindow(HWND hWnd){ wndMain = hWnd;}
	BOOL doDwmCapture(RECT src);
//	virtual BOOL DestroyWindow();
	afx_msg void OnClose();
};
