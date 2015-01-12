#include "stdafx.h"
#include "EventAction.h"
#include "JhonnyMain.h"


// ���� 
int EventAction::doAction(void* ss)
{
	OutputDebugString(_T("parent call\n"));
	return 0;
}

int EventAction::doReset()
{
	OutputDebugString(_T("parent call\n"));
	return 0;
}

int EventAction::getEventID()
{
	OutputDebugString(_T("parent call\n"));
	return 0;
}

TCHAR*  EventAction::getID()
{
	OutputDebugString(_T("parent call\n"));
	return 0;
}

int EventAction::getSortNum()
{
	OutputDebugString(_T("parent call\n"));
	return 0;
}


TCHAR* EventAction::generateItemID()
{
	//srand((unsigned)time(NULL));
	TCHAR itemID[10];
	// a == 97
	for(int i=0; i<9; i++)
		itemID[i] = rand()%10 + '0';
	itemID[9] = '\0';

	CString strTmp;
	strTmp.Format(_T("run item : %s generated.\n"), itemID);
	OutputDebugString(strTmp);

	return itemID;
	
}

// �̹��� ��ġ
int EventImageTouch::doAction(void* _main)
{
	JhonnyMain* main = (JhonnyMain*)_main;
	int x = 0;
	int y = 0;
	eventLog.Format(_T("[�̹�����ġ] : %s, "), name);
	if(main->core->doMatching(main->pTargetMainWindow->GetSafeHwnd(), main->getDlgRectRect(), item, ifItems, name, &x, &y, &eventLog) == 0)
	{
		int tx = 0; //= main->transCoord.x + x;
		int ty = 0; //= main->transCoord.y + y;

		/*
		if(tx < core->transCoord.x || ty < core->transCoord.y || tx > core->transCoord.x + SEARCH_RECT_WIDTH || ty > core->transCoord.y + SEARCH_RECT_HEGIHT)
		{
			eventLog.Format(_T("����-�����ʰ�[�̹�����ġ] : %s, (%d,%d)"), name, tx, ty);
			return 0;
		}
		*/
		
		EventSend::SendMouseEvent(main->targetWindow, tx, ty, MOUSE_LCLICK);

		/*
		SetCursorPos(tx, ty);
		mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, ::GetMessageExtraInfo());
		mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, ::GetMessageExtraInfo());
		mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, ::GetMessageExtraInfo());
		mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, ::GetMessageExtraInfo());
		mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, ::GetMessageExtraInfo());
		mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, ::GetMessageExtraInfo());
		*/
	}
	return 0;
}

int EventImageTouch::doReset()
{
	return 0;
}

// �̹��� ã��
int EventImageWait::doAction(void* _main)
{
	int x = 0;
	int y = 0;
	CString log;
	JhonnyMain* main = (JhonnyMain*)_main;
	eventLog.Format(_T("[�̹���ã��] : %s, "), name);
	if(main->core->doMatching(main->pTargetMainWindow->GetSafeHwnd(), main->getDlgRectRect(), item, ifItems, name, &x, &y, &eventLog) == 0)
	{
		
		int tx = 0; //= main->transCoord.x + x;
		int ty = 0; //= main->transCoord.y + y;
		/*
		if(tx < core->transCoord.x || ty < core->transCoord.y || tx > core->transCoord.x + SEARCH_RECT_WIDTH || ty > core->transCoord.y + SEARCH_RECT_HEGIHT)
		{
			eventLog.Format(_T("����-�����ʰ�[�̹���ã��] : %s, (%d,%d)"), name, tx, ty);
			return 0;
		}
		*/
		SetCursorPos(tx, ty);
		return findGotoIndex;
	}
	else 
		return nofindGotoIndex;
}

int EventImageWait::doReset()
{
	return 0;
}

// �ݺ��ϱ�
int EventLoop::doAction(void* _main)
{
	if(nowLoop > 0 )
	{
		nowLoop--;
		eventLog.Format(_T("[�ݺ�] : %d�� �̵�, ���� �ݺ� %d"), goToIndex, nowLoop);
		//eventLog.Format(_T("%d�� �̵��մϴ�. �ݺ� Ƚ���� %d�� ���ҽ��ϴ�."), goToIndex, nowLoop);
		return goToIndex;
	}
	else
	{
		eventLog.Format(_T("[�ݺ�] : �ݺ� ��, �������� �̵�"), goToIndex, nowLoop);
		nowLoop = loopCount;
		return 0;
	}
}

int EventLoop::doReset()
{
	nowLoop = loopCount;
	return 0;
}

// ��ٸ���
int EventWait::doAction(void* _main)
{
	eventLog.Format(_T("[���] : %d�� ���"), (int)(millisec/1000));
	Sleep(millisec);
	return 0;
}

int EventWait::doReset()
{
	return 0;
}

// ��ġ�ϱ�
int EventTouch::doAction(void* _main)
{
	JhonnyMain* main = (JhonnyMain*)_main;
	CPoint point;
	//GetCursorPos(&point);
	if(!isAbsolute)
	{
		point.x = beforeX;
		point.y = beforeY;
	}
	else
	{
		point.x = 0;
		point.y = 0;
	}
	
	// ��ġ��ǥ
	int tx = point.x + x;		
	int ty = point.y + y;
	int dx = point.x + dragX;
	int dy = point.y + dragY;

	int actionTransCoordX, actionTransCoordY;
	HWND actionTargetHandle = main->getTargetHandleFromPoint(tx, ty, &actionTransCoordX, &actionTransCoordY);

	tx += actionTransCoordX;
	ty += actionTransCoordY;
	dx += actionTransCoordX;
	dy += actionTransCoordY;
	/*
	if (isAbsolute)
	{
		tx += core->search->x;
		ty += core->search->y;
		dx += core->search->x;
		dy += core->search->y;
	}
	*/
	/*
	if(isDrag)
	{
		dx += dragX;
		dy += dragY;
	}
	*/

	/*
	if(tx < 0 || ty < 0 || tx > SEARCH_RECT_WIDTH || ty > SEARCH_RECT_HEGIHT ||
		dx < 0 || dy < 0 || dx > SEARCH_RECT_WIDTH || dy > SEARCH_RECT_HEGIHT)
	{
		if(isDrag)
			eventLog.Format(_T("����-�����ʰ�[�巡��] : %s, (%d,%d)"),name, tx, ty);
		else
			eventLog.Format(_T("����-�����ʰ�[��ġ] : %s, (%d,%d)"),name, tx, ty);
		return 0;
	}
	*/
	if(isDrag)
	{
		eventLog.Format(_T("[�巡��] : %s, (%d,%d)->(%d,%d)"), name, tx, ty, dx, dy);

		

		if(isRightClick == false)
			EventSend::SendMouseEvent(actionTargetHandle, tx, ty, MOUSE_LDRAG, dx, dy);
		else
			EventSend::SendMouseEvent(actionTargetHandle, tx, ty, MOUSE_RDRAG, dx, dy);
	}
	else
	{
		eventLog.Format(_T("[��ġ] : %s, (%d,%d)"), name, tx, ty);
		if(isRightClick == false)
		{
			EventSend::SendMouseEvent(actionTargetHandle, tx, ty, MOUSE_LCLICK);
			/*
			SetCursorPos(tx, ty);
			SetCursorPos(tx, ty);
			mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, ::GetMessageExtraInfo());
			mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, ::GetMessageExtraInfo());
			mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, ::GetMessageExtraInfo());
			mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, ::GetMessageExtraInfo());
			mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, ::GetMessageExtraInfo());
			mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, ::GetMessageExtraInfo());
			*/
		}
		else
		{
			EventSend::SendMouseEvent(actionTargetHandle, tx, ty, MOUSE_RCLICK);
			/*
			SetCursorPos(tx, ty);
			SetCursorPos(tx, ty);
			mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, ::GetMessageExtraInfo());
			mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, ::GetMessageExtraInfo());
			mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, ::GetMessageExtraInfo());
			mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, ::GetMessageExtraInfo());
			mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, ::GetMessageExtraInfo());
			mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, ::GetMessageExtraInfo());
			*/
		}
	}

	

	
	
	
	return 0;
}

int EventTouch::doReset()
{
	return 0;
}

// Ű������
int EventPressKey::doAction(void* _main)
{
	eventLog.Format(_T("[Ű] : Ű�� �����ϴ�."));
	if((mod & HOTKEYF_ALT) != 0 )
	{
		keybd_event(VK_MENU, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0);   
	} 
	if((mod & HOTKEYF_CONTROL) !=0 )
	{
		keybd_event(VK_CONTROL, 0x45, KEYEVENTF_EXTENDEDKEY  | 0, 0);   
	}
	if((mod & HOTKEYF_SHIFT) !=0 )
	{
		keybd_event(VK_SHIFT, 0x45, KEYEVENTF_EXTENDEDKEY  | 0, 0);   
	}

	keybd_event(vk, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0);   
	keybd_event(vk, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);

	if((mod & HOTKEYF_ALT) != 0 )
	{
		keybd_event(VK_MENU, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0); 
	}
	if((mod & HOTKEYF_CONTROL) !=0 )
	{
		keybd_event(VK_CONTROL, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0); 
	}
	if((mod & HOTKEYF_SHIFT) !=0 )
	{
		keybd_event(VK_SHIFT, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0); 
	}
	
	return 0;
}

int EventPressKey::doReset()
{
	return 0;
}

// ���м�
int EventSeparator::doAction(void* _main)
{
	eventLog.Format(_T("------------ %s"), name);
	return 0;
}

int EventSeparator::doReset()
{
	return 0;
}

// �̵��ϱ�
int EventGoto::doAction(void* _main)
{
	eventLog.Format(_T("[�̵�] : %d�� �̵�"), goToIndex);
	return goToIndex;
}

int EventGoto::doReset()
{
	return 0;
}

// ���ư���
int EventReturn::doAction(void* _main)
{
	eventLog.Format(_T("[����] : %d������ ����"), *returnIndex + 1 );
	if( *returnIndex == 0)
		return 0;
	return *returnIndex + 1;
}

int EventReturn::doReset()
{
	*returnIndex = 0;
	return 0;
}


void EventAction::wait(DWORD dwMillisecond)
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