#pragma once
#include "afxwin.h"
#include "JhonnyAutoCore.h"
#include "JhonnyMain.h"

// CropImageDlg dialog

class CropImageDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CropImageDlg)

public:
	CropImageDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CropImageDlg();
	CString saveFileName;
	CStatic imgShot;
	HDC hdc;
	PAINTSTRUCT ps;
	HDC hScrDC, hMemDC;
	HBITMAP hOldBitmap;
	BITMAP bmp;
	CPoint m_initialPoint;
	CRect m_lastRect;
	//int ScreenWidth;
	//int ScreenHeight;
	bool m_bDragging;
	bool isInitial;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnCancelMode();


	void setSaveFileName(TCHAR* name) { saveFileName.Format(_T("%s"),name); } 
// Dialog Data
	enum { IDD = IDD_CROPDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnPaint();
	afx_msg BOOL OnInitDialog();
	afx_msg void OnMouseLeave();
	DECLARE_MESSAGE_MAP()

private:
	JhonnyAutoCore* jCore;
	HBITMAP hBit;
};
