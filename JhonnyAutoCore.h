
// JhonnyAutoCore.h
//

#pragma once

/*
#include <afxmt.h>
#include <windows.h>
#include <memory>
*/

#include "opencv/cv.h"
#include "opencv/highgui.h"
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "JhonnyItem.h"

#define AUTO_MAX 100

class JhonnyAutoCore
{
public:
	JhonnyAutoCore();
	JhonnyAutoCore(double _threshold);
	
	double threshold;
	bool isPlaying;
	POINT transCoord;
	HWND targetWindow;
	
	//void setTargetWindow(HWND _targetWindow) { targetWindow = _targetWindow; }
	void setThreshold(float _threshold){ threshold = _threshold;}
	void setTransCoord(POINT _transCoord){ transCoord = _transCoord;}
	
	bool captureScreen(HWND hTargetWnd, RECT rect, HBITMAP* returnBitmap);
	bool hBitmap2Ipl(HBITMAP* hBmp, IplImage** pIplImage);
	bool matching(IplImage* background, IplImage* object, CvPoint* rect ,double* returnScore, double threshold);

	//bool captureScreen(HBITMAP* hBitmap, RECT rect);

	int doMatching(void* _main, JhonnyItem* item, JhonnyItem* ifItems, TCHAR* name, int* x, int* y, CString* strLine);
	

private:
	
	HWND* pHWnd;
	CListCtrl* pListRun;
	CEdit* pEditLog;
	static void* pThis;
	DWORD threadID;														
	HANDLE threadHandle;	
	
	
	UINT_PTR pTimer;
	




	
};