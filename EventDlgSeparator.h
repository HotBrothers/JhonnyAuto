#pragma once
#include "EventAction.h"

// EventDlgSeparator dialog

class EventDlgSeparator : public CDialogEx
{
	DECLARE_DYNAMIC(EventDlgSeparator)

public:
	EventDlgSeparator(CWnd* pParent = NULL);   // standard constructor
	virtual ~EventDlgSeparator();

// Dialog Data
	enum { IDD = IDD_DIALOG_EVENT_SEPARATOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString editSeparatorName;
	EventSeparator* modSeparator;
	void setEventSeparator(EventSeparator* _modSeparator) { modSeparator = _modSeparator; }
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();

};
