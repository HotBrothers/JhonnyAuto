#include "stdafx.h"
#include "EventSend.h"

BOOL EventSend::SendMouseEvent(HWND hWnd, WORD posX1, WORD posY1, MOUSE_MACRO_EVNT evnt, WORD posX2, WORD posY2)
{
	//	Default
	DWORD dwEvent = MK_LBUTTON;
	DWORD dwMsgDown = WM_LBUTTONDOWN;
	DWORD dwMsgUp = WM_LBUTTONUP;
	BOOL bDragFlag = FALSE;
	
	//	Event ����
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
		//	���� ó��
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

	//	PostMessage ���� ó�� �ʿ�
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
		//������ ������
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

	// ���� ó�� �ʿ�
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
	
	//	LPARAM ��
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

	//	����
	if(NULL == foregroundThreadID || NULL == ourThreadID)
		return FALSE;

	//	�ش� �ڵ��� Ȱ��ȭ ���� ���� ���¿��� Ű��ǲ�� ������ ���ؼ�
	//	Thread������ Attach�� �Ǿ����.
	if (foregroundThreadID != ourThreadID)
		AttachThreadInput(foregroundThreadID, ourThreadID, TRUE);

	//	Ű���� ���� ����
	SetKeyboardState(byKeyState);
	
	PostMessage(hWnd, dwMsgDown, wKeyValue, lParam);

	//	Test ��� Message���� �Ŀ� Ű���� ���� �����ϴ� �ڵ�� �ٷ� ���Խ�
	//	����Ű�� �ȸ��� �� �ִ�.
	Sleep(50);
	
	//	KEYUP �޽��� ������ Ư�� �ڵ鿡���� WM_CHAR �޽��� �߻���Ŵ
	//	�ι� �Էµ� ��ó�� ���� �� ������ �Ⱥ����� ���� ���� �� ����.
	//PostMessage(hWnd, dwMsgUp, wKeyValue, 0);
 
	if((event & KEYBD_CTL) == KEYBD_CTL)
		byKeyState[VK_CONTROL] &= 0x00;
	if((event & KEYBD_SHIFT) == KEYBD_SHIFT)
		byKeyState[VK_SHIFT] &= 0x00;
	if((event & KEYBD_ALT) == KEYBD_ALT)
		byKeyState[VK_MENU] &= 0x00;

	//	Ű���� ����ó�� ����
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