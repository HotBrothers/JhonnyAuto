#pragma once

#define WS_EX_LAYERED          0x00080000 
#define LWA_COLORKEY            0x00000001 
#define LWA_ALPHA              0x00000002 
#define ULW_COLORKEY            0x00000001 
#define ULW_ALPHA              0x00000002 
#define ULW_OPAQUE              0x00000004 
// ViewerRectDlg dialog

class ViewerRectDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ViewerRectDlg)

public:
	ViewerRectDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~ViewerRectDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_VIEWER_RECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CPoint m_pt;
	BOOL m_bDraw;
	BOOL m_bTouch;

	void drawingPoint(CPoint point, BOOL _m_bTouch);
	void DrawEllipse(HWND hWnd);
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPaint();
};
