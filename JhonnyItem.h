#pragma once

#include <windows.h>

#include "opencv/cv.h"
#include "opencv/highgui.h" 

class JhonnyItem
{
public:
	JhonnyItem(){};
	JhonnyItem(TCHAR* _id, TCHAR* bitmapPath);
	//JhonnyItem(TCHAR* _id, TCHAR* _name, TCHAR* _note, TCHAR* bitmapPath);

	IplImage* getIplImage(){ return ipl;}
	TCHAR* getID(){return id;}
	void setID(TCHAR* _id){StrCpyW(id, _id);} 
	/*
	TCHAR* getName(){return name;}
	void setName(TCHAR* _name){StrCpyW(name, _name);}
	TCHAR* getNote(){return note;}
	void setNote(TCHAR* _note){StrCpyW(note, _note);}
	*/
	TCHAR* getBitmapPath(){return bitmapPath;}
	void setBitmapPath(TCHAR* _bitmapPath){StrCpyW(bitmapPath, _bitmapPath);}

	
	TCHAR* generateItemID()
	{
		//srand(time(NULL));
		TCHAR itemID[10];
		// a == 97
		for(int i=0; i<9; i++)
			itemID[i] = rand()%10 + '0';
		itemID[9] = '\0';

		return itemID;
	}

public:
	TCHAR id[10];
	TCHAR bitmapPath[MAX_PATH];
	//TCHAR name[100];
	//TCHAR note[MAX_PATH];
	IplImage* ipl;
};