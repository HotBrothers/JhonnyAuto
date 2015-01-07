#include "stdafx.h"
#include "JhonnyRunItem.h"

JhonnyRunItem::JhonnyRunItem(JhonnyItem* _item)
{
	item = _item;
	ifItems.clear();
	priority = 1;
	repeatTime = 3.0;
	StrCpyW(runID, generateItemID());
	virtualKeyCode = 0;
	modifiers = 0;
	leftClick = 1;
	rightClick = 0;
	moveX = 0;
	moveY = 0;
	sortNum = 0;
}

JhonnyRunItem::JhonnyRunItem(TCHAR* _id, TCHAR* _name, TCHAR* _note, int _priority, float _repeatTime, int _virtualKeyCode, int _modifiers, int _moveX, int _moveY, int _leftClick, int _rightClick, int _sortNum, JhonnyItem* _item, std::vector<JhonnyItem*> _ifItems)
{
	StrCpyW(runID, _id);
	StrCpyW(runName, _name);
	priority = _priority;
	repeatTime = _repeatTime;
	item = _item;
	ifItems = _ifItems;
	virtualKeyCode = _virtualKeyCode;
	modifiers = _modifiers;
	moveX = _moveX;
	moveY = _moveY;
	leftClick = _leftClick;
	rightClick = _rightClick;
	sortNum = _sortNum;
}

TCHAR* JhonnyRunItem::generateItemID()
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