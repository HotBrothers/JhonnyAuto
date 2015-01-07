#pragma once
#include "EventAction.h"

// EventDlgMove dialog

class EventDlgMove : public CDialogEx
{
	DECLARE_DYNAMIC(EventDlgMove)

public:
	EventDlgMove(CWnd* pParent = NULL);   // standard constructor
	virtual ~EventDlgMove();

// Dialog Data
	enum { IDD = IDD_DIALOG_EVENT_MOVE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString editGotoIndex;

	EventGoto* modGoto;
	int modIndex;
	int modLoopNum;
	void setEventGoto(EventGoto* _modGoto) { modGoto = _modGoto; }
	virtual BOOL OnInitDialog();

	
};
