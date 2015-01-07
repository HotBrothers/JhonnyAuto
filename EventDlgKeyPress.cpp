// EventDlgKeyPress.cpp : implementation file
//

#include "stdafx.h"
#include "JhonnyAuto.h"
#include "EventDlgKeyPress.h"
#include "afxdialogex.h"


// EventDlgKeyPress dialog

IMPLEMENT_DYNAMIC(EventDlgKeyPress, CDialogEx)

EventDlgKeyPress::EventDlgKeyPress(CWnd* pParent /*=NULL*/)
	: CDialogEx(EventDlgKeyPress::IDD, pParent)
{

}

EventDlgKeyPress::~EventDlgKeyPress()
{
}

void EventDlgKeyPress::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HOTKEY1, hotkeyPressKey);
}


BEGIN_MESSAGE_MAP(EventDlgKeyPress, CDialogEx)
END_MESSAGE_MAP()


// EventDlgKeyPress message handlers
