#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "EventAction.h"
#include "JhonnyAutoCore.h"
#include "JhonnyRectDlg.h"
#include "JhonnyLoadingDlg.h"
#include "ViewerDlg.h"

#include "JhonnyAuto.h"
#include "JhonnyRunItem.h"
#include "JhonnyRegister.h"
#include "JhonnyAbout.h"
#include "JhonnyOptionDlg.h"

#include "ParseAPI.h"
#include "MfcUtil.h"

#include "zip.h"
#include "unzip.h"

#include "EventSend.h"
#include "EventAction.h"
#include "EventDlgLoop.h"
#include "EventDlgWait.h"
#include "EventDlgTouch.h"
#include "EventDlgPressKey.h"
#include "EventDlgSeparator.h"
#include "EventDlgMove.h"


#include <wininet.h>
#include <fstream>  
#include <iostream>  
#include <string>  
#include <io.h>


#include "resource.h"
#include <vector>

#define SEARCH_RECT_WIDTH 800
#define SEARCH_RECT_HEGIHT 450
#define EVENT_SIZE 10
#define ICON_SIZE 32
#define GUEST_MODE_ID "geust1"

// JhonnyMain dialog

class JhonnyMain : public CDialogEx
{
	DECLARE_DYNAMIC(JhonnyMain)

public:
	JhonnyMain(CWnd* pParent = NULL);   // standard constructor
	virtual ~JhonnyMain();

// Dialog Data
	enum { IDD = IDD_DIALOG_START };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString appEnvironmentPath;
	int screenX, screenY;
	TCHAR events[EVENT_SIZE][MAX_PATH];// = {"이미지 터치하기", "이미지 찾기", "반복하기", "기다리기", "터치하기", "키누르기", "이동하기", "돌아가기"};
	CFont *m_pDlgFont;
	bool needToSave;
	bool isPause;
	int returnIndexMain;

	HCURSOR mh_scope_cursor, mh_prev_cursor; 

	ParseAPI* parse;

	int startIndex;
	int nIndexLeftSel;
	int nIndexRightSel;
	DWORD threadID;														
	HANDLE threadHandle;	
	bool threadExit;
	std::vector<EventAction*> actions;

	JhonnyAutoCore* core;
	JhonnyRectDlg* rectDlg;
	JhonnyLoadingDlg* loadingDlg;
	ViewerDlg* viewer;
	//std::vector<JhonnyItem*> items;

	TCHAR listItemDirName[100];
	TCHAR runItemDirName[100];
	TCHAR rootPath[MAX_PATH];
	TCHAR appSaveFilePath[MAX_PATH];
	TCHAR countryISO[5];
	

	CImageList imglist;
	CImageList* pImageItemsDrag;
	CListCtrl listEvents;
	CListCtrl listPlaylist;
	//POINT searchRectPoint;
	
	bool isGuestMode;
	void setGeustMode(bool isGuest);
	CString userID;
	CString userPass;
	int pcounter;

	bool isCtrlPress;

	HWND targetWindow;
	CWnd* pTargetMainWindow;
	RECT targetWindowRect;
	RECT targetMainWindowRect;
	RECT rectDlgRect;
	//POINT transCoord;

	RECT returnRectWindowRect;
	//RECT returnTargetWindowRect;
	//RECT returnTargetMainWindowRect;
	//bool isMainWindowMinimized;
	HWND getTargetHandleFromPoint(int inputX, int inputY, int *transCoordX, int *transCoordY);
	void setTargetMainWndFromRectDlg();
	//RECT getTargetMainWndGetWndRect();
	RECT getDlgRectRect();

	void setThreadExit(bool _threadExit) { threadExit = _threadExit; }
	void setPause(bool _isPause) { isPause = _isPause; }
	void setUserID(CString _userID){userID = _userID;}
	void setUserPass(CString _userPass){userPass = _userPass;}
	CString HttpPostUserTokenBaaS(char* returnData, int length, CString id, CString pass, bool* activated);
	int HttpGetJhonnyVersionBaaS(TCHAR* noticeInput, TCHAR* versionInput);

	int registerImage(JhonnyItem* item, int index);

	void setListItemFocus(int status);
	void selectItem(CListCtrl* plist, int index);
	void doPlay();
	int insertEvent(TCHAR* eventItem, TCHAR* eventItemInfo, int index);
	int modifyEvent(int index, TCHAR* eventItemInfo);

	int doLoad();
	int uncompressFiles(TCHAR* loadPath);
	int loadListItems(std::vector<JhonnyItem*>* items);
	int loadRunItems(std::vector<JhonnyItem*>* items);
	int parsingRunItems(std::vector<CString>* data, std::vector<JhonnyItem*>* items);

	int doSave(TCHAR* path);
	int compressFiles(TCHAR* savePath);
	int deleteTempData();
	int saveRunItems(CString* log);
	int saveListItems(CString* log);
	BOOL refreshGoToLink();

	// 옵션 변수들
	float optDelay;
	
	WORD optVk;
	WORD optMod;
	int loadOption();
	int saveOption();

	int getFiles(TCHAR* path, TCHAR* extensionType, std::vector<TCHAR*>* fileList);
	BOOL deleteDir(CString dir);
	std::string TCHARToString(const TCHAR* ptsz);
	CString getLogTime();

	virtual BOOL OnInitDialog();
//	afx_msg void OnLvnBeginrdragListEvent(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLvnBegindragListEvent(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonPlay();



	void playCore();
	static DWORD WINAPI threadDoPlay(LPVOID parameter)
	{
		JhonnyMain* pClass = (JhonnyMain*)parameter;
		if(pClass == NULL)
			return 0;

		pClass->playCore();
		return 0;
	}

	
	 int hotKeyID;

	CButton btnStart;
	CStatic staticLog;
	CString logData;
	CEdit editLogbox;
	afx_msg void OnNMDblclkListPlaylist(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnMenuLoad();


	

	afx_msg void OnMenuSave();
	afx_msg void OnMenuNew();
	afx_msg void OnMenuExit();
	afx_msg void OnMenuHelp();
	afx_msg void OnMenuInfo();
	afx_msg void OnLvnBegindragListPlaylist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMenuOtherSave();
//	virtual BOOL DestroyWindow();
	afx_msg void OnClose();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMenuMacroHome();
	CSliderCtrl sliderThreshold;
	int editThreshold;


	afx_msg void OnNMReleasedcaptureSliderThreshold(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnKillfocusEditThreshold();
	afx_msg void OnNMCustomdrawListPlaylist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);

	bool isplayAndStopEnable;
	void playAndStop();
	void saveTextFile(TCHAR* infoDest, TCHAR* id);
	afx_msg void OnMenuLogSave();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnMenuOption();
};
