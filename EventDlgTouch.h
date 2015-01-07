#pragma once
#include "EventAction.h"
#include "afxwin.h"

// EventDlgTouch dialog

class EventDlgTouch : public CDialogEx
{
	DECLARE_DYNAMIC(EventDlgTouch)

public:
	EventDlgTouch(CWnd* pParent = NULL);   // standard constructor
	virtual ~EventDlgTouch();

// Dialog Data
	enum { IDD = IDD_DIALOG_EVENT_TOUCH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString editCoordX;
	CString editCoordY;
	CString editTouchName;
	BOOL checkIsRight;
	BOOL isGetMouseCoordMode;

	EventTouch* modTouch;
	void setEventTouch(EventTouch* _modTouch) { modTouch = _modTouch;}
	virtual BOOL OnInitDialog();
	
	BOOL checkIsAbsolute;
	afx_msg void OnBnClickedButtonGetCoord();
//	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedCheckAbs();
	CButton btnGetCoord;
	
	BOOL checkIsDrag;
	afx_msg void OnBnClickedCheckDrag();
	CString editDragCoordX;
	CString editDragCoordY;
	CButton btnDragGetCoord;
	afx_msg void OnBnClickedButtonDragGetCoord();
};
