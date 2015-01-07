#pragma once
#include "EventAction.h"

// EventDlgWait dialog

class EventDlgWait : public CDialogEx
{
	DECLARE_DYNAMIC(EventDlgWait)

public:
	EventDlgWait(CWnd* pParent = NULL);   // standard constructor
	virtual ~EventDlgWait();

// Dialog Data
	enum { IDD = IDD_DIALOG_EVENT_WAIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString editWaitTime;
	EventWait* modWait;
	void setEventWait(EventWait* _modWait) { modWait = _modWait; }
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
