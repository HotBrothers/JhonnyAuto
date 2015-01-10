#pragma once
#include "JhonnyRunItem.h"
#include "JhonnyAutoCore.h"
#include "EventSend.h"
#define NOT_FOUND 515
#define ID_IMAGE_TOUCH 0
#define ID_IMAGE_SEARCH 1
#define ID_LOOP 2
#define ID_WAIT 3
#define ID_TOUCH 4
#define ID_PRESS_KEY 5
#define ID_SEPARATOR 6
#define ID_GOTO 7
#define ID_RETURN 8

static int beforeX, beforeY;

class EventAction
{

// "이미지 터치하기", "이미지 찾기", "반복하기", "기다리기", "터치하기", "키누르기"
public:
	int eventID;
	CString eventLog;


	virtual int doAction(JhonnyAutoCore* core);
	virtual int doReset();
	virtual int getEventID();
	virtual TCHAR* getID();
	virtual int getSortNum();
	
	TCHAR* generateItemID();
	void wait(DWORD dwMillisecond);
	
	/*
	bool doTouch();
	bool waitImage();
	bool doLoop(int backIndex);
	bool doWait(int millisec);
	bool pushKey(int vk, int mod);
	*/
};

class EventImageTouch : public EventAction
{
public:
	EventImageTouch(): eventID(ID_IMAGE_TOUCH) {}
	EventImageTouch(TCHAR* _name, JhonnyItem* _item) : eventID(ID_IMAGE_TOUCH) 
		{ StrCpyW(name, _name); item = _item;  ifItems = NULL; StrCpyW(id, generateItemID()); }
	EventImageTouch(TCHAR* _name, JhonnyItem* _item, JhonnyItem* _ifItem) : eventID(ID_IMAGE_TOUCH) 
		{ StrCpyW(name, _name); item = _item;  ifItems = _ifItem; StrCpyW(id, generateItemID()); }
	
	const int eventID;
	TCHAR id[10];
	int doAction(JhonnyAutoCore* core);
	int doReset();
	int getEventID(){return eventID;}
	TCHAR* getID(){return id;}
	int getSortNum(){return sortNum;}
	int sortNum;

	//JhonnyRunItem* run;
	TCHAR name[MAX_PATH];
	JhonnyItem* item;
	JhonnyItem* ifItems;
	
};


class EventImageWait : public EventAction
{
public:
	EventImageWait(): eventID(ID_IMAGE_SEARCH) {}
	/*
	EventImageWait(TCHAR* _name, JhonnyItem* _item, int _findGotoIndex, int _nofindGotoIndex) : eventID(1) 
		{ StrCpyW(name, _name); item = _item; 
		  findGotoIndex = _findGotoIndex;  nofindGotoIndex = _nofindGotoIndex;  ifItems = NULL; 
		  StrCpyW(id, generateItemID()); StrCpyW(findGotoID, _T("")); StrCpyW(nofindGotoID, _T(""));}

		EventImageWait(TCHAR* _name, JhonnyItem* _item, int _findGotoIndex, int _nofindGotoIndex, TCHAR* _findGotoID, TCHAR* _nofindGotoID) : eventID(1) 
		{ StrCpyW(name, _name); item = _item; 
		  findGotoIndex = _findGotoIndex;  nofindGotoIndex = _nofindGotoIndex;  ifItems = _ifItem; 
		  StrCpyW(id, generateItemID()); StrCpyW(findGotoID, _findGotoID); StrCpyW(nofindGotoID, _nofindGotoID);}
		  */

	EventImageWait(TCHAR* _name, JhonnyItem* _item, JhonnyItem* _ifItem, int _findGotoIndex, int _nofindGotoIndex) : eventID(ID_IMAGE_SEARCH) 
		{ StrCpyW(name, _name); item = _item; 
		  findGotoIndex = _findGotoIndex;  nofindGotoIndex = _nofindGotoIndex;  ifItems = _ifItem; 
		  StrCpyW(id, generateItemID()); StrCpyW(findGotoID, _T("")); StrCpyW(nofindGotoID, _T(""));}

	EventImageWait(TCHAR* _name, JhonnyItem* _item, JhonnyItem* _ifItem, int _findGotoIndex, int _nofindGotoIndex, TCHAR* _findGotoID, TCHAR* _nofindGotoID) : eventID(ID_IMAGE_SEARCH) 
		{ StrCpyW(name, _name); item = _item; 
		  findGotoIndex = _findGotoIndex;  nofindGotoIndex = _nofindGotoIndex;  ifItems = _ifItem; 
		  StrCpyW(id, generateItemID()); StrCpyW(findGotoID, _findGotoID); StrCpyW(nofindGotoID, _nofindGotoID);}

	const int eventID;
	TCHAR id[10];
	int doAction(JhonnyAutoCore* core);
	int doReset();
	int getEventID(){return eventID;}
	TCHAR* getID(){return id;}
	int getSortNum(){return sortNum;}
	int sortNum;

	TCHAR name[MAX_PATH];
	JhonnyItem* item;
	JhonnyItem* ifItems;
	int findGotoIndex;
	TCHAR findGotoID[10];
	int nofindGotoIndex;
	TCHAR nofindGotoID[10];
	
};



class EventLoop : public EventAction
{
public:
	const int eventID;
	TCHAR id[10];
	EventLoop(): eventID(ID_LOOP){}
	EventLoop(int _loopCount, int _goToIndex) : eventID(ID_LOOP) 
		{ loopCount = _loopCount; goToIndex = _goToIndex; StrCpyW(id, generateItemID()); nowLoop = _loopCount;  StrCpyW(gotoID, _T(""));}
	EventLoop(int _loopCount, int _goToIndex, TCHAR* _gotoID) : eventID(ID_LOOP) 
		{ loopCount = _loopCount; goToIndex = _goToIndex; StrCpyW(id, generateItemID()); nowLoop = _loopCount;  StrCpyW(gotoID, _gotoID);}
	int doAction(JhonnyAutoCore* core);
	int doReset();
	int getEventID(){return eventID;}
	TCHAR* getID(){return id;}
	int getSortNum(){return sortNum;}
	int sortNum;

	int loopCount;
	int goToIndex;
	TCHAR gotoID[10];
	int nowLoop;
};



class EventWait : public EventAction
{
public:
	const int eventID;
	TCHAR id[10];
	EventWait(): eventID(ID_WAIT){}
	EventWait(int _millisec) : eventID(ID_WAIT) { millisec = _millisec; StrCpyW(id, generateItemID());}
	int doAction(JhonnyAutoCore* core);
	int doReset();
	int getEventID(){return eventID;}
	TCHAR* getID(){return id;}
	int getSortNum(){return sortNum;}

	int millisec;
	int sortNum;
};




class EventTouch : public EventAction
{
public:
	const int eventID;
	TCHAR id[10];
	TCHAR name[MAX_PATH];

	EventTouch(): eventID(ID_TOUCH){}
	/*
	EventTouch(TCHAR* _name, int _x, int _y, BOOL _isAbsolute, BOOL _isRightClick ) : eventID(4) 
			{ StrCpyW(name, _name); x = _x; y = _y;  isAbsolute = _isAbsolute; isRightClick = _isRightClick; StrCpyW(id, generateItemID());  isDrag = false;}*/
	EventTouch(TCHAR* _name, int _x, int _y, int _dragX, int _dragY, BOOL _isAbsolute, BOOL _isDrag, BOOL _isRightClick ) : eventID(ID_TOUCH) 
			{ StrCpyW(name, _name); x = _x; y = _y;  dragX = _dragX; dragY = _dragY; isAbsolute = _isAbsolute; isDrag = _isDrag; isRightClick = _isRightClick; StrCpyW(id, generateItemID()); }
	int doAction(JhonnyAutoCore* core);
	int doReset();
	int getEventID(){return eventID;}
	TCHAR* getID(){return id;}
	int getSortNum(){return sortNum;}

	int x;
	int y;
	int dragX;
	int dragY;
	BOOL isRightClick;
	BOOL isAbsolute;
	BOOL isDrag;
	int sortNum;

};




class EventPressKey : public EventAction
{
public:
	const int eventID;
	TCHAR id[10];
	EventPressKey(): eventID(ID_PRESS_KEY){}
	EventPressKey(WORD _vk, int _mod) : eventID(ID_PRESS_KEY) 
			{ vk = _vk; mod = _mod; StrCpyW(id, generateItemID());}
	int doAction(JhonnyAutoCore* core);
	int doReset();
	int getEventID(){return eventID;}
	TCHAR* getID(){return id;}
	int getSortNum(){return sortNum;}

	WORD vk;
	WORD mod;
	int sortNum;

};

class EventSeparator : public EventAction
{
public:
	const int eventID;
	TCHAR id[10];
	TCHAR name[MAX_PATH];

	EventSeparator(): eventID(ID_SEPARATOR){}
	EventSeparator(TCHAR* _name) : eventID(ID_SEPARATOR) 
			{StrCpyW(name, _name); StrCpyW(id, generateItemID());}
	int doAction(JhonnyAutoCore* core);
	int doReset();
	int getEventID(){return eventID;}
	TCHAR* getID(){return id;}
	int getSortNum(){return sortNum;}

	int sortNum;
};

class EventGoto : public EventAction
{
public:
	const int eventID;
	TCHAR id[10];
	int goToIndex;
	TCHAR gotoID[10];

	EventGoto(): eventID(ID_GOTO){}
	EventGoto(int _goToIndex) : eventID(ID_GOTO) 
			{goToIndex = _goToIndex; StrCpyW(id, generateItemID()); StrCpyW(gotoID, _T(""));}
	EventGoto(int _goToIndex, TCHAR* _gotoID) : eventID(ID_GOTO) 
			{goToIndex = _goToIndex; StrCpyW(id, generateItemID()); StrCpyW(gotoID, _gotoID);}
	int doAction(JhonnyAutoCore* core);
	int doReset();
	int getEventID(){return eventID;}
	TCHAR* getID(){return id;}
	int getSortNum(){return sortNum;}

	int sortNum;
};

class EventReturn : public EventAction
{
public:
	const int eventID;
	TCHAR id[10];
	int* returnIndex;
	TCHAR returnID[10];

	EventReturn(): eventID(ID_RETURN){}
	EventReturn(int* _returnIndex) : eventID(ID_RETURN) 
			{returnIndex = _returnIndex; StrCpyW(id, generateItemID()); StrCpyW(returnID, _T(""));}
	EventReturn(int* _returnIndex, TCHAR* _returnID) : eventID(ID_RETURN) 
			{returnIndex = _returnIndex; StrCpyW(id, generateItemID()); StrCpyW(returnID, _returnID);}
	int doAction(JhonnyAutoCore* core);
	int doReset();
	int getEventID(){return eventID;}
	TCHAR* getID(){return id;}
	int getSortNum(){return sortNum;}

	int sortNum;
};