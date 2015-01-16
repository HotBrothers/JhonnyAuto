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

BOOL EventSend::SendKeybdEvent(HWND hWnd, WORD wKeyValue, int event)
{
	//	Default
	DWORD dwEvent = MK_LBUTTON;
	DWORD dwMsgDown = WM_KEYDOWN;
	DWORD dwMsgUp = WM_KEYUP;
	DWORD dwExtendKey = 0;

	BYTE byKeyState[256] = {0,};
	
	//	LPARAM 용
	extraKeyInfo lParam = {};
	lParam.scanCode = MapVirtualKey(wKeyValue, MAPVK_VK_TO_VSC);
	//lParam.repeatCount = 0;

	GetKeyboardState(byKeyState);

	if((event & KEYBD_CTL) == KEYBD_CTL)
		byKeyState[VK_CONTROL] |= 0x80;
	if((event & KEYBD_SHIFT) == KEYBD_SHIFT)
		byKeyState[VK_SHIFT] |= 0x80;
	if((event & KEYBD_ALT) == KEYBD_ALT)
	{
		lParam.sysdown = 1;
		dwMsgDown = WM_SYSKEYDOWN;
		byKeyState[VK_MENU] |= 0x80;
	}

	DWORD foregroundThreadID = NULL;
	DWORD ourThreadID = NULL;

	foregroundThreadID = GetWindowThreadProcessId(hWnd, NULL);
	ourThreadID = GetCurrentThreadId();

	//	실패
	if(NULL == foregroundThreadID || NULL == ourThreadID)
		return FALSE;

	//	해당 핸들이 활성화 되지 않은 상태에서 키인풋을 날리기 위해선
	//	Thread단으로 Attach가 되어야함.
	if (foregroundThreadID != ourThreadID)
		AttachThreadInput(foregroundThreadID, ourThreadID, TRUE);

	//	키보드 상태 변경
	SetKeyboardState(byKeyState);
	
	PostMessage(hWnd, dwMsgDown, wKeyValue, lParam);

	//	Test 결과 Message보낸 후에 키보드 상태 원복하는 코드로 바로 진입시
	//	조합키가 안먹힐 수 있다.
	Sleep(50);
	
	//	KEYUP 메시지 보내면 특정 핸들에서는 WM_CHAR 메시지 발생시킴
	//	두번 입력된 것처럼 보일 수 있으니 안보내는 것이 나을 수 있음.
	//PostMessage(hWnd, dwMsgUp, wKeyValue, 0);
 
	if((event & KEYBD_CTL) == KEYBD_CTL)
		byKeyState[VK_CONTROL] &= 0x00;
	if((event & KEYBD_SHIFT) == KEYBD_SHIFT)
		byKeyState[VK_SHIFT] &= 0x00;
	if((event & KEYBD_ALT) == KEYBD_ALT)
		byKeyState[VK_MENU] &= 0x00;

	//	키보드 기존처럼 변경
	SetKeyboardState(byKeyState);

	if (foregroundThreadID != ourThreadID)
		AttachThreadInput(foregroundThreadID, ourThreadID, FALSE);

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