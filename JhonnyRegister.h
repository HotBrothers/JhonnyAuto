#pragma once
#include "afxcmn.h"
#include "EventAction.h"



// JhonnyRegister dialog

class JhonnyRegister : public CDialog
{
	DECLARE_DYNAMIC(JhonnyRegister)

public:
	JhonnyRegister(CWnd* pParent = NULL);   // standard constructor
	virtual ~JhonnyRegister();

// Dialog Data
	enum { IDD = IDD_DIALOG_REGISTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
//	afx_msg void OnDestroy();
	virtual BOOL DestroyWindow();
	
	afx_msg void OnStnClickedStaticItemImage();
	afx_msg void OnBnClickedOk();
	
	bool isIfImageSetted;
	bool isImageChanged;
	bool ifImageDeleted;
	int dlgMode;
	void setDlgMode(int _dlgMode) {dlgMode = _dlgMode;}
	//void setGoto(int _find, int _nofind) {find = _find; nofind = _nofind;}
	

	HBITMAP hBitSelect;
	JhonnyItem item;
	JhonnyItem ifItem;
	CString itemName;
	bool isNowLoading;
	int imgIndex;
	CFont* redFont;
	//int find;
	//int nofind;

	EventImageTouch* modImageTouch; 
	EventImageWait* modImageWait; 

	//CImageList imgReglist;
	
	void setEventImageTouch(EventImageTouch* _modImageTouch){ modImageTouch = _modImageTouch; }
	void setEventImageWait(EventImageWait* _modImageWait){ modImageWait = _modImageWait; }
	virtual BOOL OnInitDialog();
//	afx_msg void OnHdnItemclickListRegItemlist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedListRegItemlist(NMHDR *pNMHDR, LRESULT *pResult);
	CString editFindGoto;
	CString editNofindGoto;
	//afx_msg void OnStnClickedStaticIfItem();
//	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

//	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonIfDelete();
};
