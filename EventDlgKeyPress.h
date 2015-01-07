#pragma once
#include "afxcmn.h"


// EventDlgKeyPress dialog

class EventDlgKeyPress : public CDialogEx
{
	DECLARE_DYNAMIC(EventDlgKeyPress)

public:
	EventDlgKeyPress(CWnd* pParent = NULL);   // standard constructor
	virtual ~EventDlgKeyPress();

// Dialog Data
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CHotKeyCtrl hotkeyPressKey;
	WORD vk;
	WORD mod;
};
