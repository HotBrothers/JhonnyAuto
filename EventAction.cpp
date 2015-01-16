#include "stdafx.h"
#include "EventAction.h"
#include "JhonnyMain.h"


// 공용 
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

// 이미지 터치
int EventImageTouch::doAction(void* _main)
{
	JhonnyMain* main = (JhonnyMain*)_main;
	int tx = 0;
	int ty = 0;
	eventLog.Format(_T("[이미지터치] : %s, "), name);
	if(main->core->doMatching(main, item, ifItems, name, &tx, &ty, &eventLog) == 0)
	{
		int actionTransCoordX, actionTransCoordY;
		HWND actionTargetHandle = main->getTargetHandleFromPoint(tx, ty, &actionTransCoordX, &actionTransCoordY);
		if(actionTargetHandle == NULL )
		{
			eventLog.Format(_T("실패-매크로타겟없음[이미지터치] : %s"), name);
			return 0;
		}

		CPoint pt;
		pt.x = tx;
		pt.y = ty;
		main->rectDlg->drawingPoint(pt, TRUE);

		beforeX = tx;
		beforeY = ty;
		beforeTargetHandle = actionTargetHandle;

		tx += actionTransCoordX;
		ty += actionTransCoordY;
		
		EventSend::SendMouseEvent(actionTargetHandle, tx, ty, MOUSE_LCLICK);
	
		/*
		if(tx < core->transCoord.x || ty < core->transCoord.y || tx > core->transCoord.x + SEARCH_RECT_WIDTH || ty > core->transCoord.y + SEARCH_RECT_HEGIHT)
		{
			eventLog.Format(_T("실패-범위초과[이미지터치] : %s, (%d,%d)"), name, tx, ty);
			return 0;
		}
		*/
		
		

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

// 이미지 찾기
int EventImageWait::doAction(void* _main)
{
	int tx = 0;
	int ty = 0;
	CString log;
	JhonnyMain* main = (JhonnyMain*)_main;
	eventLog.Format(_T("[이미지찾기] : %s, "), name);
	int result = main->core->doMatching(main, item, ifItems, name, &tx, &ty, &eventLog);
	if(result == 0)
	{
		int actionTransCoordX, actionTransCoordY;
		HWND actionTargetHandle = main->getTargetHandleFromPoint(tx, ty, &actionTransCoordX, &actionTransCoordY);
		if(actionTargetHandle == NULL)
		{
			eventLog.Format(_T("실패-매크로타겟없음[이미지찾기] : %s"), name);
			return 0;
		}

		CPoint pt;
		pt.x = tx;
		pt.y = ty;
		main->rectDlg->drawingPoint(pt, FALSE);


		beforeX = tx;
		beforeY = ty;
		beforeTargetHandle = actionTargetHandle;

		tx += actionTransCoordX;
		ty += actionTransCoordY;
		/*
		if(tx < core->transCoord.x || ty < core->transCoord.y || tx > core->transCoord.x + SEARCH_RECT_WIDTH || ty > core->transCoord.y + SEARCH_RECT_HEGIHT)
		{
			eventLog.Format(_T("실패-범위초과[이미지찾기] : %s, (%d,%d)"), name, tx, ty);
			return 0;
		}
		*/
		
		return findGotoIndex;
	}
	else if( result == -1)
	{
		eventLog.Format(_T("실패-매크로타겟없음[이미지찾기] : %s"), name);
		return 0;
	}
	else 
		return nofindGotoIndex;
}

int EventImageWait::doReset()
{
	return 0;
}

// 반복하기
int EventLoop::doAction(void* _main)
{
	if(nowLoop > 0 )
	{
		nowLoop--;
		eventLog.Format(_T("[반복] : %d번 이동, 남은 반복 %d"), goToIndex, nowLoop);
		//eventLog.Format(_T("%d로 이동합니다. 반복 횟수가 %d번 남았습니다."), goToIndex, nowLoop);
		return goToIndex;
	}
	else
	{
		eventLog.Format(_T("[반복] : 반복 끝, 다음으로 이동"), goToIndex, nowLoop);
		nowLoop = loopCount;
		return 0;
	}
}

int EventLoop::doReset()
{
	nowLoop = loopCount;
	return 0;
}

// 기다리기
int EventWait::doAction(void* _main)
{
	eventLog.Format(_T("[대기] : %d초 대기"), (int)(millisec/1000));
	Sleep(millisec);
	return 0;
}

int EventWait::doReset()
{
	return 0;
}

// 터치하기
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
	
	// 터치좌표
	int tx = point.x + x;		
	int ty = point.y + y;
	int dx = point.x + dragX;
	int dy = point.y + dragY;

	int actionTransCoordX, actionTransCoordY;
	HWND actionTargetHandle = main->getTargetHandleFromPoint(tx, ty, &actionTransCoordX, &actionTransCoordY);
	if(actionTargetHandle == NULL )
	{
		eventLog.Format(_T("실패-매크로타겟없음[터치] : %s"), name);
		return 0;
	}
	CPoint pt;
	pt.x = tx;
	pt.y = ty;
	main->rectDlg->drawingPoint(pt, TRUE);
	//main->rectDlg->OnLButtonUp(0, pt);

	if(isDrag)
	{
		beforeX = dx;
		beforeY = dy;
	}
	else
	{
		beforeX = tx;
		beforeY = ty;
	}
	beforeTargetHandle = actionTargetHandle;
	
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
			eventLog.Format(_T("실패-범위초과[드래그] : %s, (%d,%d)"),name, tx, ty);
		else
			eventLog.Format(_T("실패-범위초과[터치] : %s, (%d,%d)"),name, tx, ty);
		return 0;
	}
	*/
	if(isDrag)
	{
		eventLog.Format(_T("[드래그] : %s, (%d,%d)->(%d,%d)"), name, tx, ty, dx, dy);

		

		if(isRightClick == false)
			EventSend::SendMouseEvent(actionTargetHandle, tx, ty, MOUSE_LDRAG, dx, dy);
		else
			EventSend::SendMouseEvent(actionTargetHandle, tx, ty, MOUSE_RDRAG, dx, dy);
	}
	else
	{
		eventLog.Format(_T("[터치] : %s, (%d,%d)"), name, tx, ty);
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

// 키누르기
int EventPressKey::doAction(void* _main)
{
	//WORD vkCode = VkKeyScan(L'f');

	if(IsWindow(beforeTargetHandle) == false)
	{
		eventLog.Format(_T("실패-매크로타겟없음[키누르기]"));
		return 0;
	}
	
	int optionKey = 0;
	CString outputKeyName;
	
	if((mod & HOTKEYF_CONTROL) !=0 )
	{
		optionKey |= KEYBD_CTL;
		outputKeyName += _T("CTRL+");
	}
	if((mod & HOTKEYF_SHIFT) !=0 )
	{
		optionKey |= KEYBD_SHIFT;
		outputKeyName += _T("SHIFT+");
	}
	if((mod & HOTKEYF_ALT) != 0 )
	{
		optionKey |= KEYBD_ALT;
		outputKeyName += _T("ALT+");
	}

	TCHAR buffer[5];
	BYTE keyboard_state[256];
	::GetKeyboardState( keyboard_state );
	HKL keyboard_layout = ::GetKeyboardLayout( 0 );

	
	int result = ::ToUnicodeEx( vk, MapVirtualKey(vk, MAPVK_VK_TO_VSC), keyboard_state, buffer, 4, 0, keyboard_layout );
	buffer[4] = L'\0';

	if( result > 0 ) {
    // Could convert, so do your stuff
		outputKeyName += buffer;
		eventLog.Format(_T("[키] : %s 키를 누릅니다."), outputKeyName);
		EventSend::SendKeybdEvent(beforeTargetHandle, vk, optionKey);  
	}


	

	/*
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
	*/
	return 0;
}

int EventPressKey::doReset()
{
	return 0;
}

// 구분선
int EventSeparator::doAction(void* _main)
{
	eventLog.Format(_T("------------ %s"), name);
	return 0;
}

int EventSeparator::doReset()
{
	return 0;
}

// 이동하기
int EventGoto::doAction(void* _main)
{
	eventLog.Format(_T("[이동] : %d번 이동"), goToIndex);
	return goToIndex;
}

int EventGoto::doReset()
{
	return 0;
}

// 돌아가기
int EventReturn::doAction(void* _main)
{
	eventLog.Format(_T("[복귀] : %d번으로 복귀"), *returnIndex + 1 );
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