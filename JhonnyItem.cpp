#include "stdafx.h"
#include "JhonnyItem.h"


JhonnyItem::JhonnyItem(TCHAR* _id, TCHAR* _bitmapPath)
{
	StrCpyW(id, _id);
	StrCpyW(bitmapPath, _bitmapPath);

	int len = 256;
	char ctemp[256];
	WideCharToMultiByte(CP_ACP, 0, _bitmapPath, len, ctemp, len, NULL, NULL);
	ipl = cvLoadImage(ctemp);
	

}