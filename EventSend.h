#pragma once
#include "afxcmn.h"


enum MOUSE_MACRO_EVNT
{
	MOUSE_LCLICK = 0,	//	왼쪽 클릭
	MOUSE_RCLICK,		//	오른쪽 클릭
	MOUSE_DCLICK,		//	왼쪽 더블 클릭
	MOUSE_CTRCLICK,		//	컨트롤 왼쪽 클릭
	MOUSE_SHTCLICK,		//	시프트 왼쪽 클릭
	MOUSE_LDRAG,		//	드래그
	MOUSE_RDRAG
};

enum KEYBD_MACRO_EVNT
{
	KEYBO_NONE	=	0,		//	영문 키
	KEYBD_ALT	=	1,		//	알트
	KEYBD_SHIFT	=	2,		//	시프트
	KEYBD_CTL	=	4		//	컨트롤
};


class EventSend
{
public:

	struct extraKeyInfo
	{
		unsigned short repeatCount;
		unsigned char scanCode;
		bool extendedKey, prevKeyState, transitionState;
		bool sysdown;

		operator unsigned int()
		{
			return repeatCount | (scanCode << 16) | (extendedKey << 24) | (sysdown << 29) |
				   (prevKeyState << 30) | (transitionState << 31);
		}
	};

	


	//--------------------------------------------------------
	/** 
		@brief SendMouseEvent
		@param hWnd : 윈도우 핸들
		@param posX1 : 클릭 X 좌표
		@param posY1 : 클릭 X 좌표
		@param event : 마우스 이벤트 종류
		@param posX2 : 드래그 종료 X 좌표, 기본 값 0
		@param posY2 : 드래그 종료 X 좌표, 기본 값 0

		@return 무의미, 실패 처리 누락
		@remark 모름
		@author oshomku
	*/
	//--------------------------------------------------------
	static BOOL SendMouseEvent(HWND hWnd, WORD posX1, WORD posY1, MOUSE_MACRO_EVNT evnt, WORD posX2 = 0, WORD posY2 = 0);

	//--------------------------------------------------------
	/** 
		@brief SendKeybdEvent
		@param hWnd : 윈도우 핸들
		@param wKeyValue : VK_XXX
		@param event : 키보드 이벤트 종류, Default - KEYBD_NONE)
		@return 무의미, 실패 처리 누락
		@remark 
		- event 는 KEYBD_SHIFT | KEYBD_CTL 으로 입력시 SHIFT + CTRL + S 
		@author oshomku
	*/
	//--------------------------------------------------------
	static BOOL SendKeybdEvent(HWND hWnd, WORD wKeyValue, int event = 0);
	static void wait(DWORD dwMillisecond);
};