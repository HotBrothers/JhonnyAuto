#pragma once

#include "JhonnyItem.h"

class JhonnyRunItem
{

public:
	JhonnyRunItem(JhonnyItem* _item);
	JhonnyRunItem(TCHAR* _id, TCHAR* _name, TCHAR* _note, int _priority, float _repeatTime, int _virtualKeyCode, 
		int _modifiers, int _moveX, int _moveY, int _leftClick, int _rightClick, int _sortNum, JhonnyItem* _item, std::vector<JhonnyItem*> _ifItems);

	JhonnyItem* item;
	std::vector<JhonnyItem*> ifItems;

	
	TCHAR* getID(){return runID;}
	void setID(TCHAR* _runID){StrCpyW(runID, _runID);}
	TCHAR* getName(){return runName;}
	void setName(TCHAR* _runName){StrCpyW(runName, _runName);}
	TCHAR* getNote(){return runNote;}
	void setNote(TCHAR* _runNote){StrCpyW(runNote, _runNote);}
	int getPriority(){return priority;}
	void setPriority(int _priority){priority = _priority;}
	float getRepeatTime(){return repeatTime;}
	void setRepeatTime(float _repeatTime){repeatTime = _repeatTime;}
	TCHAR* generateItemID();
	int getVirtualKeyCode(){return virtualKeyCode;}
	void setVirtualKeyCode(int _virtualKeyCode){virtualKeyCode = _virtualKeyCode;}
	int getModifiers(){return modifiers;}
	void setModifiers(int _modifiers){modifiers = _modifiers;}
	int getMoveX(){return moveX;}
	void SetMoveX(int _moveX){moveX = _moveX;}
	int getMoveY(){return moveY;}
	void SetMoveY(int _moveY){moveY = _moveY;}
	int getLeftClick(){return leftClick;}
	void setLeftClick(int _leftClick){leftClick = _leftClick;}
	int getRightClick(){return rightClick;}
	void setRightClick(int _rightClick){rightClick = _rightClick;}
	int getSortNum(){return sortNum;}

private:
	TCHAR runID[10];
	TCHAR runName[100];
	TCHAR runNote[MAX_PATH];
	int priority;
	float repeatTime;
	int virtualKeyCode;
	int modifiers;
	int moveX;
	int moveY;
	int leftClick;
	int rightClick;
	int sortNum;
	int eventID;
};