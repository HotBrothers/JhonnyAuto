#pragma once
#include "JhonnyRunItem.h"
#include "afxwin.h"
#include "afxcmn.h"


// JhonnyRegisterRun dialog

class JhonnyRegisterRun : public CDialog
{
	DECLARE_DYNAMIC(JhonnyRegisterRun)

public:
	JhonnyRegisterRun(CWnd* pParent = NULL);   // standard constructor
	virtual ~JhonnyRegisterRun();

// Dialog Data
	enum { IDD = IDD_DIALOG_REGISTER_RUNS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	HBITMAP hBitSelect;
	JhonnyRunItem* run;
	std::vector<JhonnyItem*> backupIfItems;
	//std::vector<JhonnyItem*>* items;

	void setRunItem(JhonnyRunItem* _run){run = _run;}
	//void setIndex(int _index){index = _index;}
	//void setListItems(std::vector<JhonnyItem*>* _items){items = _items;}
	//void setListCtrlPointer(std::vector<JhonnyItem*>* _items){items = _items;}
	
	int index;
	CString runName;
	float runRepeatTime;
	CSliderCtrl sliderPriority;
	int runPriority;
	afx_msg void OnNMReleasedcaptureSliderPriority(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
	CEdit editRunsName;
	CStatic staticPriority;
	CListCtrl listIfItems;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedCancel();
	CHotKeyCtrl hotkeyAction;
	BOOL actionLeftClick;
	BOOL actionRightClick;
	int editMoveX;
	int editMoveY;
};
