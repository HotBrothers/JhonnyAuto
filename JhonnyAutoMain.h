#pragma once
#include "afxcmn.h"
#include "JhonnyItem.h"
#include "JhonnyRunItem.h"
#include "JhonnyAutoCore.h"

#include "afxwin.h"

// JhonnyAutoMain dialog

class JhonnyAutoMain : public CDialog
{
	DECLARE_DYNAMIC(JhonnyAutoMain)

public:
	JhonnyAutoMain(CWnd* pParent = NULL);   // standard constructor
	virtual ~JhonnyAutoMain();


// Dialog Data
	enum { IDD = IDD_DIALOG_MAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	bool isStartItemsList;
	int nIndexLeftSel;
	std::vector<JhonnyItem*> items;
	std::vector<JhonnyRunItem> runs;
	std::vector<JhonnyRunItem*> runsPool;
	char listItemDirName[100];
	char runItemDirName[100];
	char rootPath[MAX_PATH];
	JhonnyAutoCore core;
	static std::map<UINT_PTR, JhonnyAutoMain*> m_JhonnyAutoMainMap; //declaration

	HWND hWnd;
	CListCtrl listItems;
	CListCtrl runItems;
	CListCtrl eventItems;

	CImageList imageItems;
	CImageList* pImageItemsDrag;
	
	int insertJhonnyItem(CListCtrl* list ,JhonnyItem* item, int index);
	int insertJhonnyRunItem(CListCtrl* parent, CListCtrl*  list ,JhonnyRunItem* item, int index);

	int deleteTempData();

	int doLoad();
	int loadRunItems();
	int loadListItems();
	int uncompressFiles(char* savePath);

	int doSave();
	int saveRunItems();
	int saveListItems();
	int compressFiles(char* savePath);
	int doStartTimer(UINT idEvent, DWORD dwTime);
	static void CALLBACK runTimerProc( HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime );




	afx_msg void OnBnClickedStart();
	virtual BOOL OnInitDialog();
//	virtual void OnCancel();
//	virtual BOOL DestroyWindow();
	afx_msg void OnClose();
	afx_msg void OnBnClickedAdd();
	
//	afx_msg void OnLvnBeginrdragListItems(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	
	afx_msg void OnLvnBegindragListItems(NMHDR *pNMHDR, LRESULT *pResult);
	CButton btnAdd;
	CButton btnStart;
	CString editLog;
	CSliderCtrl sliderThreshold;
	CString stringThreshold;
	afx_msg void OnNMReleasedcaptureSliderThreshold(NMHDR *pNMHDR, LRESULT *pResult);
	CStatic staticThreshold;
	CEdit editLogCtrl;
//	afx_msg void OnHdnItemdblclickListRunItems(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnNMDblclkListItems(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListRunItems(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnMenuExit();
	afx_msg void OnMenuSave();
	afx_msg void OnMenuLoad();
	afx_msg void OnNMClickListItems(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickListRunItems(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMenuInfo();
	afx_msg void OnMenuHelp();
	afx_msg void OnMenuNew();
	afx_msg void OnLvnBegindragListRunItems(NMHDR *pNMHDR, LRESULT *pResult);
	int radioTimeRun;
	afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
	
};
