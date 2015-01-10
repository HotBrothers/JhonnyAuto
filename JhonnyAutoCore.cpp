#include "stdafx.h"
#include "JhonnyMain.h"
#include "JhonnyAutoCore.h"

#define TIMER_ID 4001

using namespace cv;



JhonnyAutoCore::JhonnyAutoCore()
{
	isPlaying = false;
	threshold = 0.85;
}

JhonnyAutoCore::JhonnyAutoCore( double _threshold)
{
	isPlaying = false;
	threshold = _threshold;
}


int JhonnyAutoCore::doMatching(JhonnyItem* item, JhonnyItem* ifItems, TCHAR* name, int* x, int* y, CString* strLine)
{
	HBITMAP hBit = NULL;
	IplImage* dest;
	CString result;

	captureScreen(&hBit, FALSE);

	if(hBit == NULL )
	{
		strLine->Format(_T("�̹���(NULL) ����"));
		DeleteObject(hBit);
		cvReleaseImage(&dest);
		return -1;
	}

	hBitmap2Ipl( &hBit, &dest);
	CvPoint searchRect[4];

	double score = 0.0f;
	double ifScore = 0.0f;
	bool satifyCondition=false;
	bool isMatching = false;

	if(threshold <= 0.0f && threshold >= 1.0f)
		threshold = 0.90f;

	isMatching = matching(dest, item->getIplImage(), searchRect, &score, threshold);

	/*
	if(isPlaying == false)
	{
		DeleteObject(hBit);
		cvReleaseImage(&dest);
		return 0;
	}
	*/

	if(isMatching == false)
	{
		strLine->Format(_T("����-��Ī[�̹�����ġ] : %s(%d%%)"), name, (int)(score*100));

		DeleteObject(hBit);
		cvReleaseImage(&dest);
		return -1;
	}

	if(score >= threshold)
	{

		if(ifItems != NULL)
		{
			if(matching(dest, ifItems->getIplImage(), NULL, &ifScore, threshold) == true)
				satifyCondition = true;
		}
		else
			satifyCondition = true;
	}

	if(satifyCondition == false)
	{
		strLine->Format(_T("����-����ã��[�̹�����ġ] : %s(%d%%)"), name, (int)(score*100));
		DeleteObject(hBit);
		cvReleaseImage(&dest);
		return -1;
	}

	result.Format(_T("����(%d%%)"), (int)(score*100));
	*strLine += result;

	

	int minX = 9999;
	int minY = 9999;			
	int maxX = -9999;
	int maxY = -9999;
	for(int i=0; i<4; i++)
	{
		if(searchRect[i].x < minX)
			minX = searchRect[i].x;
		if(searchRect[i].y < minY)
			minY = searchRect[i].y;
		if(searchRect[i].x > maxX)
			maxX = searchRect[i].x;
		if(searchRect[i].y > maxY)
			maxY = searchRect[i].y;
	}
	int centerX = minX + (maxX-minX)/2.0;
	int centerY = minY + (maxY-minY)/2.0;
		
	*x = centerX;
	*y = centerY;


	DeleteObject(hBit);
	cvReleaseImage(&dest);
	

	return (DWORD)0;
}


BOOL SaveToFile(HBITMAP hBitmap, LPCTSTR lpszFileName)
{
	HDC hDC;

	int iBits;

	WORD wBitCount;

	DWORD dwPaletteSize=0, dwBmBitsSize=0, dwDIBSize=0, dwWritten=0;

	BITMAP Bitmap;

	BITMAPFILEHEADER bmfHdr;

	BITMAPINFOHEADER bi;

	LPBITMAPINFOHEADER lpbi;

	HANDLE fh, hDib, hPal,hOldPal=NULL;

	hDC = CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
	iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);
	DeleteDC(hDC);
	if (iBits <= 1)
		wBitCount = 1;
	else if (iBits <= 4)
		wBitCount = 4;
	else if (iBits <= 8)
		wBitCount = 8;
	else
		wBitCount = 24;
	GetObject(hBitmap, sizeof(Bitmap), (LPSTR)&Bitmap);
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = Bitmap.bmWidth;
	bi.biHeight = Bitmap.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = wBitCount;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrImportant = 0;
	bi.biClrUsed = 0;
	dwBmBitsSize = ((Bitmap.bmWidth * wBitCount + 31) / 32) * 4 * Bitmap.bmHeight;

	hDib = GlobalAlloc(GHND,dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
	*lpbi = bi;

	hPal = GetStockObject(DEFAULT_PALETTE);
	if (hPal)
	{ 
		hDC = GetDC(NULL);
		hOldPal = SelectPalette(hDC, (HPALETTE)hPal, FALSE);
		RealizePalette(hDC);
	}


	GetDIBits(hDC, hBitmap, 0, (UINT) Bitmap.bmHeight, (LPSTR)lpbi + sizeof(BITMAPINFOHEADER) 
		+dwPaletteSize, (BITMAPINFO *)lpbi, DIB_RGB_COLORS);

	if (hOldPal)
	{
		SelectPalette(hDC, (HPALETTE)hOldPal, TRUE);
		RealizePalette(hDC);
		ReleaseDC(NULL, hDC);
	}

	fh = CreateFile(lpszFileName, GENERIC_WRITE,0, NULL, CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL); 

	if (fh == INVALID_HANDLE_VALUE)
		return FALSE; 

	bmfHdr.bfType = 0x4D42; // "BM"
	dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;
	bmfHdr.bfSize = dwDIBSize;
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;

	WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);

	WriteFile(fh, (LPSTR)lpbi, dwDIBSize, &dwWritten, NULL);
	GlobalUnlock(hDib);
	GlobalFree(hDib);
	CloseHandle(fh);
	return TRUE;
}


bool JhonnyAutoCore::captureScreen(HBITMAP* hBitmap, BOOL isFullScreen)
{
	// �־ �ǳ�
	//if(hBitmap != NULL)
	//	DeleteObject(*hBitmap);

	int ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	int ScreenHeight = GetSystemMetrics(SM_CYSCREEN);
	int startX = 0;
	int startY = 0;

	/*
	if(isFullScreen==FALSE && rectDlgPt != NULL)
	{
		startX = rectDlgPt->x;
		startY = rectDlgPt->y;
	}

	*/


	HDC h_screen_dc = ::GetDC(NULL);

	// ���� ��ũ���� �ػ󵵸� ��´�.
	//int width = ::GetDeviceCaps(h_screen_dc, HORZRES);
	//int height = ::GetDeviceCaps(h_screen_dc, VERTRES);

	// DIB�� ������ �����Ѵ�.
	BITMAPINFO dib_define;
	dib_define.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	dib_define.bmiHeader.biWidth = SEARCH_RECT_WIDTH;
	dib_define.bmiHeader.biHeight = SEARCH_RECT_HEGIHT;
	dib_define.bmiHeader.biPlanes = 1;
	dib_define.bmiHeader.biBitCount = 24;
	dib_define.bmiHeader.biCompression = BI_RGB;
	dib_define.bmiHeader.biSizeImage = (((SEARCH_RECT_WIDTH * 24 + 31) & ~31) >> 3) * SEARCH_RECT_HEGIHT;
	dib_define.bmiHeader.biXPelsPerMeter = 0;
	dib_define.bmiHeader.biYPelsPerMeter = 0;
	dib_define.bmiHeader.biClrImportant = 0;
	dib_define.bmiHeader.biClrUsed = 0;

	// DIB�� ���� �̹��� ��Ʈ ������ ������ ������ ����
	BYTE *p_image_data = NULL;

	// dib_define�� ���ǵ� �������� DIB�� �����Ѵ�.
	HBITMAP h_bitmap = ::CreateDIBSection(h_screen_dc, &dib_define, DIB_RGB_COLORS, (void **)&p_image_data, 0, 0);

	// �̹����� �����ϱ� ���ؼ� ���� DC�� �����Ѵ�. ���� DC������ ���������� ��Ʈ�ʿ� �����Ͽ�
	// �̹��� ������ ���� �� ���� �����̴�.
	HDC h_memory_dc = ::CreateCompatibleDC(h_screen_dc);

	// ���� DC�� �̹����� ������ ��Ʈ���� �����Ѵ�.
	HBITMAP h_old_bitmap = (HBITMAP)::SelectObject(h_memory_dc, h_bitmap);

	// ���� ��ũ�� ȭ���� ĸ���Ѵ�.
	//::BitBlt(h_memory_dc, 0, 0, width, height, h_screen_dc, 0, 0, SRCCOPY);

	if(isFullScreen == TRUE)
		::BitBlt(h_memory_dc, startX, startY, ScreenWidth, ScreenHeight, h_screen_dc, 0, 0, SRCCOPY);
	else
		::BitBlt(h_memory_dc, -startX, -startY, SEARCH_RECT_WIDTH+startX, SEARCH_RECT_HEGIHT+startY, h_screen_dc, 0, 0, SRCCOPY);

	// ������ ��Ʈ������ �����Ѵ�.
	::SelectObject(h_memory_dc, h_old_bitmap);

	// ���� DC�� �����Ѵ�.
	DeleteDC(h_memory_dc);
	//if(!SaveToFile(h_bitmap, _T("rhkdwnd.bmp")))
	//	return -1;
	*hBitmap = h_bitmap;
	

	// ����ߴ� ��Ʈ�ʰ� DC �� �����Ѵ�.
	//if(NULL != h_bitmap) DeleteObject(h_bitmap);
	if(NULL != h_screen_dc) ::ReleaseDC(NULL, h_screen_dc);

	return true;

	/*
	HDC hScrDC, hMemDC;

	hScrDC=CreateDC(_T("DISPLAY"),NULL,NULL,NULL);


	hMemDC = CreateCompatibleDC(hScrDC);
	if(isFullScreen == TRUE)
		*bitmap = CreateCompatibleBitmap(hScrDC, ScreenWidth, ScreenHeight);
	else 
		*bitmap = CreateCompatibleBitmap(hScrDC, SEARCH_RECT_WIDTH, SEARCH_RECT_HEGIHT);
	SelectObject(hMemDC, *bitmap);

	//BitBlt(hMemDC, startX, startY, SEARCH_RECT_WIDTH, SEARCH_RECT_HEGIHT, hScrDC, 0, 0, SRCCOPY);
	
	if(isFullScreen == TRUE)
		BitBlt(hMemDC, startX, startY, ScreenWidth, ScreenHeight, hScrDC, 0, 0, SRCCOPY);
	else
		BitBlt(hMemDC, -startX, -startY, SEARCH_RECT_WIDTH+startX, SEARCH_RECT_HEGIHT+startY, hScrDC, 0, 0, SRCCOPY);
		
	//SaveToFile(*bitmap, "save.bmp");
	DeleteDC(hMemDC);
	DeleteDC(hScrDC);
	//InvalidateRect(hWnd, NULL, TRUE);
	*/
}

bool JhonnyAutoCore::hBitmap2Ipl(HBITMAP* hBmp, IplImage** pIplImage)
{
	int ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	int ScreenHeight = GetSystemMetrics(SM_CYSCREEN);
	BITMAP bmp;
	::GetObject(*hBmp, sizeof(BITMAP), &bmp);



	int buffsize = bmp.bmWidthBytes * bmp.bmHeight;
	PBYTE pImgBuff = new BYTE[buffsize];
	GetBitmapBits(*hBmp, buffsize, pImgBuff);
	*pIplImage = cvCreateImage(cvSize(bmp.bmWidth, bmp.bmHeight), 8, bmp.bmBitsPixel / 8);
	
	memcpy((*pIplImage)->imageData, pImgBuff, buffsize);
	//cvReleaseImage(&pIplImage);
	delete [] pImgBuff;
	
	return true;
	//dest = cvCloneImage(pIplImage);
	//cvReleaseImage(&pIplImage);
	//dest = pIplImage;
	//Output("%d x %d", pIplImage->width, pIplImage->height);
	


}


// surf
bool JhonnyAutoCore::matching(IplImage* background, IplImage* object, CvPoint* rect ,double* returnScore, double threshold)
{
	if(background == NULL)
		return false;
	if(object == NULL)
		return false;
	
  Mat object_mat = cvarrToMat(object);
  Mat background_mat = cvarrToMat(background);

  Mat img_object, img_scene;

  cv::cvtColor(object_mat, img_object, CV_BGR2GRAY);
  cv::cvtColor(background_mat, img_scene, CV_BGR2GRAY);
	
  if( !img_object.data || !img_scene.data )
  { std::cout<< " --(!) Error reading images " << std::endl; return false; }

  //-- Step 1: Detect the keypoints using SURF Detector
  int minHessian = 400;

  SurfFeatureDetector detector( minHessian );

  std::vector<KeyPoint> keypoints_object, keypoints_scene;

  detector.detect( img_object, keypoints_object );
  detector.detect( img_scene, keypoints_scene );

  //-- Step 2: Calculate descriptors (feature vectors)
  SurfDescriptorExtractor extractor;

  Mat descriptors_object, descriptors_scene;

  extractor.compute( img_object, keypoints_object, descriptors_object );
  extractor.compute( img_scene, keypoints_scene, descriptors_scene );

  if(descriptors_object.dims <= 1 || descriptors_object.rows <= 1 || descriptors_object.cols <= 1 )
	  return false;
  if(descriptors_scene.dims <= 1 || descriptors_scene.rows <= 1 || descriptors_scene.cols <= 1 )
	  return false;

  //-- Step 3: Matching descriptor vectors using FLANN matcher
  FlannBasedMatcher matcher;
  std::vector< DMatch > matches;
  matcher.match( descriptors_object, descriptors_scene, matches );

  double max_dist = 0; double min_dist = 100;

  //-- Quick calculation of max and min distances between keypoints
  for( int i = 0; i < descriptors_object.rows; i++ )
  { double dist = matches[i].distance;
    if( dist < min_dist ) min_dist = dist;
    if( dist > max_dist ) max_dist = dist;
  }

   if(min_dist == 0.0f)
	  min_dist = 0.001f;
  //Output("-- Max dist : %f \n", max_dist );
  //Output("-- Min dist : %f \n", min_dist );


  /*
  //-- Draw only "good" matches (i.e. whose distance is less than 3*min_dist )
  std::vector< DMatch > good_matches;

  for( int i = 0; i < descriptors_object.rows; i++ )
  { if( matches[i].distance < 3*min_dist )
     { good_matches.push_back( matches[i]); }
  }
  */
   //-- Draw only "good" matches (i.e. whose distance is less than 3*min_dist )
  std::vector<DMatch> good_matches;
  double good_matches_sum = 0.0;

  for (size_t i = 0; i < matches.size(); i++ ) {
	  if( matches[i].distance < max(2*min_dist, 0.03) ) {
		  good_matches.push_back(matches[i]);
		  good_matches_sum += matches[i].distance;
	  }
  }

  double score = (double)good_matches_sum / (double)good_matches.size();
  //Output(_T("-- score : %f \n"), 1.0f-score );
  
  // 0.90f
  if (threshold < 1.0f-score) {

	  if(rect == NULL || returnScore == NULL)
		  return true;

	  // have match
	  //-- Localize the object
	  std::vector<Point2f> obj;
	  std::vector<Point2f> scene;

	  // good match�� ������ ���������� ����
	  int gSize = good_matches.size();
	  if(gSize < 4)
	  {
		 
		  /*
		 for(int i=gSize; i<4; i++)
			  good_matches.push_back(good_matches.at(0));
			  */
		 for(int i=0; i<4; i++)
			rect[i] = keypoints_scene[ good_matches[0].trainIdx ].pt;
		 *returnScore = (1.0f-score);
		 return true;
	  }

	  for( int i = 0; i < good_matches.size(); i++ )
	  {
		//-- Get the keypoints from the good matches
		obj.push_back( keypoints_object[ good_matches[i].queryIdx ].pt );
		scene.push_back( keypoints_scene[ good_matches[i].trainIdx ].pt );
	  }
	  
	  /*
	  if(good_matches.size() < 4)
		  return false;
		*/  

	  Mat H = findHomography( obj, scene, CV_RANSAC );

	  /*
	  vector<uTCHAR> mask;
	  Mat H = findHomography( obj, scene, CV_RANSAC, 3, mask );
	  int good_matches = accumulate(mask.begin(), mask.end(), 0);
	  */

	  //-- Get the corners from the image_1 ( the object to be "detected" )
	  std::vector<Point2f> obj_corners(4);
	  obj_corners[0] = cvPoint(0,0);
	  obj_corners[1] = cvPoint( img_object.cols, 0 );
	  obj_corners[2] = cvPoint( img_object.cols, img_object.rows );
	  obj_corners[3] = cvPoint( 0, img_object.rows );
	  std::vector<Point2f> scene_corners(4);

	  perspectiveTransform( obj_corners, scene_corners, H);

	  for(int i=0; i<4; i++)
			rect[i] = scene_corners[i];
  
	  *returnScore = (1.0f-score);
	  return true;

  } 
  else
  {
	// dont have match
	if(rect == NULL || returnScore == NULL)
		return false;
	*returnScore = (1.0f-score);
		return false;
  }

  
}

