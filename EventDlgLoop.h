#pragma once

#include "EventAction.h"

// EventDlgLoop dialog

class EventDlgLoop : public CDialogEx
{
	DECLARE_DYNAMIC(EventDlgLoop)

public:
	EventDlgLoop(CWnd* pParent = NULL);   // standard constructor
	virtual ~EventDlgLoop();

// Dialog Data
	enum { IDD = IDD_DIALOG_EVENT_LOOP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString editGotoIndex;
	CString editLoopNum;

	EventLoop* modLoop;
	int modIndex;
	int modLoopNum;
	void setEventLoop(EventLoop* _modLoop) { modLoop = _modLoop; }
	virtual BOOL OnInitDialog();
};
