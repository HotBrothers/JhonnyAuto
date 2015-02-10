#pragma once

#include "ViewerRectDlg.h"
#include "CBitmapButtonTrans.h"
#include "ColorStatic.h"

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
	ViewerRectDlg* viewerRectDlg;
	virtual BOOL OnInitDialog();
	HTHUMBNAIL thumbnail;
	HWND wndTarget;
	HWND wndMain;
	RECT returnTargetWindow;
	bool isPlay;
	bool isStop;

	CBitmapButtonTrans btnStart;
	CBitmapButtonTrans btnPause;
	CBitmapButtonTrans btnStop;
	CColorStatic* stNow;
	CColorStatic* stTotal;
//	CListCtrl actionItem;
	CString strNowNum;
	CString strTotalNum;

	void doPlay();
	void doPause();
	void doStop();

	void setNowNum(CString _strNowNum); 
	void setNowTotal(CString _strTotalNum); 
	void setTargetWindow(HWND hWnd){ wndTarget = hWnd;}
	void setReturnTargetWindow(RECT rect){ returnTargetWindow = rect;}
	void setMainWindow(HWND hWnd){ wndMain = hWnd;}
	BOOL doDwmCapture(RECT src);
//	virtual BOOL DestroyWindow();
	afx_msg void OnClose();
	afx_msg void OnMove(int x, int y);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};
