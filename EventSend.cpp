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
	case MOUSE_LDRAG:
		bDragFlag = TRUE;
		break;
	case MOUSE_RDRAG:
		bDragFlag = TRUE;
		dwEvent = MK_RBUTTON;
		dwMsgDown = WM_RBUTTONDOWN;
		dwMsgUp = WM_RBUTTONUP;
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
		/*
		PostMessage(hWnd, dwMsgDown, dwEvent, lParam1);
		PostMessage(hWnd, WM_MOUSEMOVE, dwEvent, lParam2);
		PostMessage(hWnd, dwMsgUp, dwEvent, lParam2);
		*/

		//SetCursorPos(bx, by);
		//mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, ::GetMessageExtraInfo());
		PostMessage(hWnd, dwMsgDown, dwEvent, lParam1);

		int bx, by;
		int ax, ay;

		bx = posX1;
		by = posY1;
		ax = posX2;
		ay = posY2;

		if(bx == ax)
			ax+=1;
		if(by == ay)
			ay+=1;


		wait(300);
		//직선의 방정식
		double a = double (ay - by) / (ax - bx);
		double b = by - a*bx;

			
		int x_pos = bx;
		int y_pos = by;

		LPARAM lParamMove;
		bool isWide = abs(ax-bx) >= abs(ay-by) ? true : false;
		if(isWide)
		{
			int sign = (ax < bx) ? -1 : 1;
			for(int i = 0; i < abs(ax-bx); i++)
			{
				wait(4);
				x_pos = int(bx + sign*i);
				y_pos = int(a*(bx + sign*i) + b);

				LPARAM lParamMove = MAKELPARAM(x_pos, y_pos);
				PostMessage(hWnd, WM_MOUSEMOVE, dwEvent, lParamMove);
				//SetCursorPos(x_pos, y_pos);
			}
		}
		else
		{
			int sign = (ay < by) ? -1 : 1;
			for(int i = 0; i < abs(ay-by); i++)
			{
				wait(4);
				y_pos = int(by + sign*i);
				x_pos = int(((by + sign*i) - b) / a);
				
				LPARAM lParamMove = MAKELPARAM(x_pos, y_pos);
				PostMessage(hWnd, WM_MOUSEMOVE, dwEvent, lParamMove);
				//SetCursorPos(x_pos, y_pos);
			}
		}

			
			
		PostMessage(hWnd, WM_MOUSEMOVE, dwEvent, lParam2);	
		//SetCursorPos(ax, ay);
		wait(300);
		PostMessage(hWnd, dwMsgUp, dwEvent, lParam2);
		//mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, ::GetMessageExtraInfo());
	}
	else
	{
		LPARAM lParam = MAKELPARAM(posX1, posY1);
		PostMessage(hWnd, dwMsgDown, dwEvent, lParam);
		//PostMessage(hWnd, dwMsgUp, dwEvent, lParam);
		PostMessage(hWnd, dwMsgUp, 0, lParam);
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

void EventSend::wait(DWORD dwMillisecond)
{
	MSG msg;
	DWORD dwStart;
	dwStart = GetTickCount();

	while(GetTickCount() - dwStart < dwMillisecond)
	{
		while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}