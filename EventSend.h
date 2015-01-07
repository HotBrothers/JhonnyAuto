#pragma once
#include "afxcmn.h"


enum MOUSE_MACRO_EVNT
{
	MOUSE_LCLICK = 0,	//	왼쪽 클릭
	MOUSE_RCLICK,		//	오른쪽 클릭
	MOUSE_DCLICK,		//	왼쪽 더블 클릭
	MOUSE_CTRCLICK,		//	컨트롤 왼쪽 클릭
	MOUSE_SHTCLICK,		//	시프트 왼쪽 클릭
	MOUSE_DRAG			//	드래그
};

enum KEYBD_MACRO_EVNT
{
	KEYBO_NONE = 0,		//	영문 키
	KEYBD_ALT,			//	알트
	KEYBD_SHIFT,		//	시프트
	KEYBD_CTL			//	컨트롤
};


class EventSend
{
	

public:
	static BOOL SendMouseEvent(HWND hWnd, WORD posX1, WORD posY1, MOUSE_MACRO_EVNT evnt, WORD posX2 = 0, WORD posY2 = 0);
	static BOOL SendKeybdEvent(HWND hWnd, DWORD dwKeyValue, KEYBD_MACRO_EVNT evnt);
};