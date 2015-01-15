#pragma once
#include "afxcmn.h"
#include "resource.h"


#define WS_EX_LAYERED          0x00080000 
#define LWA_COLORKEY            0x00000001 
#define LWA_ALPHA              0x00000002 
#define ULW_COLORKEY            0x00000001 
#define ULW_ALPHA              0x00000002 
#define ULW_OPAQUE              0x00000004 


// JhonnyRectDlg dialog

class JhonnyRectDlg : public CDialog
{
	DECLARE_DYNAMIC(JhonnyRectDlg)

public:
	void* main;
	bool isMovable;
	JhonnyRectDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~JhonnyRectDlg();
	void drawingPoint(CPoint point, BOOL _m_bTouch);
	void setMoveable(bool _isMovable) { isMovable = _isMovable;}
// Dialog Data
	enum { IDD = IDD_DIALOG_RECT };
	virtual BOOL OnInitDialog();
	afx_msg void OnMove(int x, int y);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	
	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void DrawEllipse(HWND hWnd);
	afx_msg void OnPaint();
	CPoint m_pt;
	BOOL m_bDraw;
	BOOL m_bTouch;

	DECLARE_MESSAGE_MAP()
	
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
