#pragma once
#include "afxcmn.h"


enum MOUSE_MACRO_EVNT
{
	MOUSE_LCLICK = 0,	//	���� Ŭ��
	MOUSE_RCLICK,		//	������ Ŭ��
	MOUSE_DCLICK,		//	���� ���� Ŭ��
	MOUSE_CTRCLICK,		//	��Ʈ�� ���� Ŭ��
	MOUSE_SHTCLICK,		//	����Ʈ ���� Ŭ��
	MOUSE_LDRAG,		//	�巡��
	MOUSE_RDRAG
};

enum KEYBD_MACRO_EVNT
{
	KEYBO_NONE	=	0,		//	���� Ű
	KEYBD_ALT	=	1,		//	��Ʈ
	KEYBD_SHIFT	=	2,		//	����Ʈ
	KEYBD_CTL	=	4		//	��Ʈ��
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
		@param hWnd : ������ �ڵ�
		@param posX1 : Ŭ�� X ��ǥ
		@param posY1 : Ŭ�� X ��ǥ
		@param event : ���콺 �̺�Ʈ ����
		@param posX2 : �巡�� ���� X ��ǥ, �⺻ �� 0
		@param posY2 : �巡�� ���� X ��ǥ, �⺻ �� 0

		@return ���ǹ�, ���� ó�� ����
		@remark ��
		@author oshomku
	*/
	//--------------------------------------------------------
	static BOOL SendMouseEvent(HWND hWnd, WORD posX1, WORD posY1, MOUSE_MACRO_EVNT evnt, WORD posX2 = 0, WORD posY2 = 0);

	//--------------------------------------------------------
	/** 
		@brief SendKeybdEvent
		@param hWnd : ������ �ڵ�
		@param wKeyValue : VK_XXX
		@param event : Ű���� �̺�Ʈ ����, Default - KEYBD_NONE)
		@return ���ǹ�, ���� ó�� ����
		@remark 
		- event �� KEYBD_SHIFT | KEYBD_CTL ���� �Է½� SHIFT + CTRL + S 
		@author oshomku
	*/
	//--------------------------------------------------------
	static BOOL SendKeybdEvent(HWND hWnd, WORD wKeyValue, int event = 0);
	static void wait(DWORD dwMillisecond);
};