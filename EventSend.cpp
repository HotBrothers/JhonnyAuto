#include "stdafx.h"
#include "EventSend.h"

BOOL EventSend::SendMouseEvent(HWND hWnd, WORD posX1, WORD posY1, MOUSE_MACRO_EVNT evnt, WORD posX2, WORD posY2)
{
	//	Default
	DWORD dwEvent = MK_LBUTTON;
	DWORD dwMsgDown = WM_LBUTTONDOWN;
	DWORD dwMsgUp = WM_LBUTTONUP;
	BOOL bDragFlag = FALSE;
	
	//	Event 구분
	switch(evnt)
	{
	case MOUSE_LCLICK:
		dwEvent = MK_LBUTTON;
		dwMsgDown = WM_LBUTTONDOWN;
		dwMsgUp = WM_LBUTTONUP;
		break;
	case MOUSE_RCLICK:
		dwEvent = MK_RBUTTON;
		dwMsgDown = WM_RBUTTONDOWN;
		dwMsgUp = WM_RBUTTONUP;
		break;
	case MOUSE_DCLICK:
		//	별도 처리
		dwEvent = MK_LBUTTON;
		dwMsgDown = WM_LBUTTONDBLCLK;
		dwMsgUp = WM_LBUTTONUP;
		break;
	case MOUSE_CTRCLICK:
		dwEvent = MK_LBUTTON | MK_CONTROL;
		dwMsgDown = WM_LBUTTONDOWN;
		dwMsgUp = WM_LBUTTONUP;
		break;
	case MOUSE_SHTCLICK:
		dwEvent = MK_LBUTTON | MK_SHIFT;
		dwMsgDown = WM_LBUTTONDOWN;
		dwMsgUp = WM_LBUTTONUP;
		break;
	case MOUSE_DRAG:
		// 별도 처리
		bDragFlag = TRUE;
		break;
	default:
		dwEvent = MK_LBUTTON;
		dwMsgDown = WM_LBUTTONDOWN;
		dwMsgUp = WM_LBUTTONUP;
		break;
	}

	//	PostMessage 실패 처리 필요
	if(bDragFlag)
	{
		LPARAM lParam1 = MAKELPARAM(posX1, posY1);
		LPARAM lParam2 = MAKELPARAM(posX2, posY2);
		PostMessage(hWnd, dwMsgDown, dwEvent, lParam1);
		PostMessage(hWnd, WM_MOUSEMOVE, dwEvent, lParam2);
		PostMessage(hWnd, dwMsgUp, dwEvent, lParam2);
	}
	else
	{
		LPARAM lParam = MAKELPARAM(posX1, posY1);
		PostMessage(hWnd, dwMsgDown, dwEvent, lParam);
		PostMessage(hWnd, dwMsgUp, dwEvent, lParam);
	}

	// 리턴 처리 필요
	return TRUE;
}

BOOL EventSend::SendKeybdEvent(HWND hWnd, DWORD dwKeyValue, KEYBD_MACRO_EVNT evnt)
{
	//	Default
	DWORD dwEvent = MK_LBUTTON;
	DWORD dwMsgDown = WM_KEYDOWN;
	DWORD dwMsgUp = WM_KEYUP;
	DWORD dwExtendKey = 0;
	
	// 리턴 처리 필요
	return TRUE;
}