// EventDlgPressKey.cpp : implementation file
//

#include "stdafx.h"
#include "JhonnyAuto.h"
#include "EventDlgPressKey.h"
#include "afxdialogex.h"


// EventDlgPressKey dialog

IMPLEMENT_DYNAMIC(EventDlgPressKey, CDialogEx)

EventDlgPressKey::EventDlgPressKey(CWnd* pParent /*=NULL*/)
	: CDialogEx(EventDlgPressKey::IDD, pParent)
{
	modKey = NULL;
}

EventDlgPressKey::~EventDlgPressKey()
{
}

void EventDlgPressKey::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HOTKEY_KEY, hotkeyKey);
}


BEGIN_MESSAGE_MAP(EventDlgPressKey, CDialogEx)
	ON_BN_CLICKED(IDOK, &EventDlgPressKey::OnBnClickedOk)
END_MESSAGE_MAP()


// EventDlgPressKey message handlers


void EventDlgPressKey::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	
	hotkeyKey.GetHotKey(vk, mod);
	CDialogEx::OnOK();
}


CString EventDlgPressKey::getHotkeyName(WORD wCode, WORD wModifiers)
{
	
    CString strKeyName;
    if (wCode != 0 || wModifiers != 0)
    {
        if (wModifiers & HOTKEYF_CONTROL)
        {
            strKeyName += CHotKeyCtrl::GetKeyName(VK_CONTROL, FALSE);
            strKeyName += L"+";
        }
 
        if (wModifiers & HOTKEYF_SHIFT)
        {
            strKeyName += CHotKeyCtrl::GetKeyName(VK_SHIFT, FALSE);
            strKeyName += L"+";
        }
 
        if (wModifiers & HOTKEYF_ALT)
        {
            strKeyName += CHotKeyCtrl::GetKeyName(VK_MENU, FALSE);
            strKeyName += L"+";
        }
 
		
        strKeyName += CHotKeyCtrl::GetKeyName(wCode, wModifiers & HOTKEYF_EXT);
    }
 
    return strKeyName;
}

BOOL EventDlgPressKey::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	if(modKey != NULL)
	{
		hotkeyKey.SetHotKey(modKey->vk, modKey->mod);
		//UpdateData(false);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL EventDlgPressKey::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
	{
		hotkeyKey.SetHotKey(VK_RETURN, NULL);
		return FALSE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
