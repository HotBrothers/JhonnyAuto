#pragma once
#include "EventAction.h"


// EventDlgPressKey dialog

class EventDlgPressKey : public CDialogEx
{
	DECLARE_DYNAMIC(EventDlgPressKey)

public:
	EventDlgPressKey(CWnd* pParent = NULL);   // standard constructor
	virtual ~EventDlgPressKey();

// Dialog Data
	enum { IDD = IDD_DIALOG_EVENT_PRESS_KEY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	WORD vk;
	WORD mod;
	CHotKeyCtrl hotkeyKey;

	EventPressKey* modKey;
	void setEventPressKey(EventPressKey* _modKey) { modKey = _modKey;}

	CString getHotkeyName(WORD wCode, WORD wModifiers);
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
