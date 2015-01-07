#pragma once
#include "afxcmn.h"


enum MOUSE_MACRO_EVNT
{
	MOUSE_LCLICK = 0,	//	���� Ŭ��
	MOUSE_RCLICK,		//	������ Ŭ��
	MOUSE_DCLICK,		//	���� ���� Ŭ��
	MOUSE_CTRCLICK,		//	��Ʈ�� ���� Ŭ��
	MOUSE_SHTCLICK,		//	����Ʈ ���� Ŭ��
	MOUSE_DRAG			//	�巡��
};

enum KEYBD_MACRO_EVNT
{
	KEYBO_NONE = 0,		//	���� Ű
	KEYBD_ALT,			//	��Ʈ
	KEYBD_SHIFT,		//	����Ʈ
	KEYBD_CTL			//	��Ʈ��
};


class EventSend
{
	

public:
	static BOOL SendMouseEvent(HWND hWnd, WORD posX1, WORD posY1, MOUSE_MACRO_EVNT evnt, WORD posX2 = 0, WORD posY2 = 0);
	static BOOL SendKeybdEvent(HWND hWnd, DWORD dwKeyValue, KEYBD_MACRO_EVNT evnt);
};