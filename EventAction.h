#pragma once
#include "JhonnyRunItem.h"
#include "JhonnyAutoCore.h"
#include "EventSend.h"
#define NOT_FOUND 515

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
	EventImageTouch(): eventID(0) {}
	EventImageTouch(TCHAR* _name, JhonnyItem* _item) : eventID(0) 
		{ StrCpyW(name, _name); item = _item;  ifItems = NULL; StrCpyW(id, generateItemID()); }
	EventImageTouch(TCHAR* _name, JhonnyItem* _item, JhonnyItem* _ifItem) : eventID(0) 
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
	EventImageWait(): eventID(1) {}
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

	EventImageWait(TCHAR* _name, JhonnyItem* _item, JhonnyItem* _ifItem, int _findGotoIndex, int _nofindGotoIndex) : eventID(1) 
		{ StrCpyW(name, _name); item = _item; 
		  findGotoIndex = _findGotoIndex;  nofindGotoIndex = _nofindGotoIndex;  ifItems = _ifItem; 
		  StrCpyW(id, generateItemID()); StrCpyW(findGotoID, _T("")); StrCpyW(nofindGotoID, _T(""));}

	EventImageWait(TCHAR* _name, JhonnyItem* _item, JhonnyItem* _ifItem, int _findGotoIndex, int _nofindGotoIndex, TCHAR* _findGotoID, TCHAR* _nofindGotoID) : eventID(1) 
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
	EventLoop(): eventID(2){}
	EventLoop(int _loopCount, int _goToIndex) : eventID(2) 
		{ loopCount = _loopCount; goToIndex = _goToIndex; StrCpyW(id, generateItemID()); nowLoop = _loopCount;  StrCpyW(gotoID, _T(""));}
	EventLoop(int _loopCount, int _goToIndex, TCHAR* _gotoID) : eventID(2) 
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
	EventWait(): eventID(3){}
	EventWait(int _millisec) : eventID(3) { millisec = _millisec; StrCpyW(id, generateItemID());}
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

	EventTouch(): eventID(4){}
	/*
	EventTouch(TCHAR* _name, int _x, int _y, BOOL _isAbsolute, BOOL _isRightClick ) : eventID(4) 
			{ StrCpyW(name, _name); x = _x; y = _y;  isAbsolute = _isAbsolute; isRightClick = _isRightClick; StrCpyW(id, generateItemID());  isDrag = false;}*/
	EventTouch(TCHAR* _name, int _x, int _y, int _dragX, int _dragY, BOOL _isAbsolute, BOOL _isDrag, BOOL _isRightClick ) : eventID(4) 
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
	EventPressKey(): eventID(5){}
	EventPressKey(WORD _vk, int _mod) : eventID(5) 
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

	EventSeparator(): eventID(6){}
	EventSeparator(TCHAR* _name) : eventID(6) 
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

	EventGoto(): eventID(7){}
	EventGoto(int _goToIndex) : eventID(7) 
			{goToIndex = _goToIndex; StrCpyW(id, generateItemID()); StrCpyW(gotoID, _T(""));}
	EventGoto(int _goToIndex, TCHAR* _gotoID) : eventID(7) 
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

	EventReturn(): eventID(8){}
	EventReturn(int* _returnIndex) : eventID(8) 
			{returnIndex = _returnIndex; StrCpyW(id, generateItemID()); StrCpyW(returnID, _T(""));}
	EventReturn(int* _returnIndex, TCHAR* _returnID) : eventID(8) 
			{returnIndex = _returnIndex; StrCpyW(id, generateItemID()); StrCpyW(returnID, _returnID);}
	int doAction(JhonnyAutoCore* core);
	int doReset();
	int getEventID(){return eventID;}
	TCHAR* getID(){return id;}
	int getSortNum(){return sortNum;}

	int sortNum;
};