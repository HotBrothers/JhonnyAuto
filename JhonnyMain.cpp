// JhonnyMain.cpp : implementation file
//

#include "stdafx.h"
#include "JhonnyMain.h"
/*
#include "afxcmn.h"
#include "afxdialogex.h"
*/


#define MIN_WINDOW_WIDTH 436
#define MIN_WINDOW_HEIGHT 490
#define MAX_WINDOW_WIDTH 436  // 최대 Width  3000  
#define MAX_WINDOW_HEIGHT 1080  // 최소 Height 3000  

using namespace std;

BOOL Capture(HWND hTargetWnd, LPCTSTR lpszFilePath);
//CString GetFileVersion();
bool compareObj( EventAction* first, EventAction* second );


// JhonnyMain dialog

IMPLEMENT_DYNAMIC(JhonnyMain, CDialogEx)

JhonnyMain::JhonnyMain(CWnd* pParent /*=NULL*/)
	: CDialogEx(JhonnyMain::IDD, pParent)
	, editThreshold(0)
{
	m_pDlgFont = new CFont(); 
	m_pDlgFont->CreateFont(12, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET
	, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, _T("MS Shell Dlg")); 

	isCtrlPress = false;
	needToSave = false;
	//searchRectPoint.x = 0;
	//searchRectPoint.y = 0;
	returnIndexMain = 0;
	pcounter = 0;
	targetWindow = NULL;
	pTargetMainWindow = NULL;
	//isMainWindowMinimized = false;
	
	core = new JhonnyAutoCore(0.85);
	isplayAndStopEnable = true;
	StrCpyW(rootPath, _T(""));
	screenX = (int)GetSystemMetrics(SM_CXSCREEN);
	screenY = (int)GetSystemMetrics(SM_CYSCREEN);
	isPause = false;
	parse = new ParseAPI();

	// 환경설정
	TCHAR path[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_COMMON_APPDATA, NULL, 0, path)))
	{
		StrCatW(path,_T("\\HotBrothers"));
		CreateDirectory(path, NULL);
		StrCatW(path,_T("\\JhonnyMacro"));
		CreateDirectory(path, NULL);
	}


	appEnvironmentPath = path;
	appEnvironmentPath += _T("\\");
	loadOption();
	
}


JhonnyMain::~JhonnyMain()
{
	
	deleteDir(rootPath);

	HANDLE hFile     = INVALID_HANDLE_VALUE;
    DWORD dwRetVal = 0;
    TCHAR szTempFileName[MAX_PATH];  
	dwRetVal = GetTempPath(MAX_PATH,          // length of the buffer
                           szTempFileName); // buffer for path 
	SetCurrentDirectory(szTempFileName);
	RemoveDirectory(rootPath);
	UnregisterHotKey(GetSafeHwnd(), hotKeyID);
	if(m_pDlgFont != NULL)
		delete m_pDlgFont;
}

void JhonnyMain::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_EVENT, listEvents);
	DDX_Control(pDX, IDC_LIST_PLAYLIST, listPlaylist);
	DDX_Control(pDX, IDC_BUTTON_PLAY, btnStart);
	DDX_Control(pDX, IDC_EDIT_LOGBOX, editLogbox);
	DDX_Control(pDX, IDC_SLIDER_THRESHOLD, sliderThreshold);
	DDX_Text(pDX, IDC_EDIT_THRESHOLD, editThreshold);
	DDV_MinMaxInt(pDX, editThreshold, 0, 100);
}


BEGIN_MESSAGE_MAP(JhonnyMain, CDialogEx)
//	ON_NOTIFY(LVN_BEGINRDRAG, IDC_LIST_EVENT, &JhonnyMain::OnLvnBeginrdragListEvent)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_NOTIFY(LVN_BEGINDRAG, IDC_LIST_EVENT, &JhonnyMain::OnLvnBegindragListEvent)
	ON_BN_CLICKED(IDC_BUTTON_PLAY, &JhonnyMain::OnBnClickedButtonPlay)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PLAYLIST, &JhonnyMain::OnNMDblclkListPlaylist)
	ON_COMMAND(ID_MENU_LOAD, &JhonnyMain::OnMenuLoad)
	ON_COMMAND(ID_MENU_SAVE, &JhonnyMain::OnMenuSave)
	ON_COMMAND(ID_MENU_NEW, &JhonnyMain::OnMenuNew)
	ON_COMMAND(ID_MENU_EXIT, &JhonnyMain::OnMenuExit)
	ON_COMMAND(ID_MENU_HELP, &JhonnyMain::OnMenuHelp)
	ON_COMMAND(ID_MENU_INFO, &JhonnyMain::OnMenuInfo)
	ON_NOTIFY(LVN_BEGINDRAG, IDC_LIST_PLAYLIST, &JhonnyMain::OnLvnBegindragListPlaylist)
	ON_COMMAND(ID_MENU_OTHER_SAVE, &JhonnyMain::OnMenuOtherSave)
	ON_WM_CLOSE()
	ON_WM_GETMINMAXINFO()
	ON_WM_SIZE()
	ON_COMMAND(ID_MENU_MACRO_HOME, &JhonnyMain::OnMenuMacroHome)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_THRESHOLD, &JhonnyMain::OnNMReleasedcaptureSliderThreshold)
	ON_EN_KILLFOCUS(IDC_EDIT_THRESHOLD, &JhonnyMain::OnEnKillfocusEditThreshold)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_PLAYLIST, &JhonnyMain::OnNMCustomdrawListPlaylist)
	ON_WM_HOTKEY()
	ON_COMMAND(ID_MENU_LOG_SAVE, &JhonnyMain::OnMenuLogSave)
	ON_WM_SYSCOMMAND()
	ON_COMMAND(ID_MENU_OPTION, &JhonnyMain::OnMenuOption)
END_MESSAGE_MAP()


// JhonnyMain message handlers


BOOL JhonnyMain::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	


	srand((unsigned int)time(NULL));
	CFileFind pFind;
	BOOL bRet; 

	/*
	StrCpyW(countryISO, _T(""));
    int iLen = 0;
    ::GetLocaleInfo(LOCALE_USER_DEFAULT,
                    LOCALE_SISO639LANGNAME2,
                    countryISO,
                    sizeof(countryISO) / sizeof(TCHAR));
	OutputDebugString(countryISO);
	OutputDebugString(_T("\n"));
	*/

	setlocale(LC_ALL,"");

	SendMessageToDescendants(WM_SETFONT, (WPARAM)m_pDlgFont->GetSafeHandle(), 1, TRUE, FALSE); 

	pImageItemsDrag = NULL;
	nIndexLeftSel = -1;
	nIndexRightSel = -1;

	StrCpyW(listItemDirName, _T("data\\listitem"));
	StrCpyW(runItemDirName, _T("data\\runitem"));
	StrCpyW(appSaveFilePath, _T(""));

	
	
	
	HANDLE hFile     = INVALID_HANDLE_VALUE;
    DWORD dwRetVal = 0;

	
    TCHAR szTempFileName[MAX_PATH];  
	dwRetVal = GetTempPath(MAX_PATH,          // length of the buffer
                           szTempFileName); // buffer for path 
    if (dwRetVal > MAX_PATH || (dwRetVal == 0))
    {
        if (!CloseHandle(hFile))
        {
            return (7);
        }
        return (2);
    }
	StrCatW(szTempFileName,_T("HotBrothers"));
	CreateDirectory(szTempFileName, NULL);
	StrCatW(szTempFileName,_T("\\JhonnyMacro"));
	CreateDirectory(szTempFileName, NULL);
	CTime t = CTime::GetCurrentTime();
	CString date;
	date.Format(_T("\\%d%d%d%d%d%d"), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());
	StrCatW(szTempFileName, date);
	CreateDirectory(szTempFileName, NULL);
	OutputDebugString(szTempFileName);

	TCHAR filePath[MAX_PATH];
	StrCpyW(rootPath, szTempFileName);
	

	StrCpyW(filePath, rootPath);
	StrCatW(filePath, _T("\\data"));
	bRet = pFind.FindFile(filePath); // 파일 존재여부 파악
	if(bRet == true)
		deleteDir(filePath);
	CreateDirectory(filePath, NULL);


	StrCpyW(filePath, rootPath);
	StrCatW(filePath, _T("\\"));
	StrCatW(filePath, listItemDirName);
	/*
	bRet = pFind.FindFile(filePath); // 파일 존재여부 파악
	if(bRet == true)
		DeleteDir(filePath);
		*/
	CreateDirectory(filePath, NULL);

	StrCpyW(filePath, rootPath);
	StrCatW(filePath, _T("\\"));
	StrCatW(filePath, runItemDirName);
	/*
	bRet = pFind.FindFile(filePath); // 파일 존재여부 파악
	if(bRet == true)
		DeleteDir(filePath);
		*/
	CreateDirectory(filePath, NULL);

	TCHAR list[EVENT_SIZE][MAX_PATH] = {_T("이미지 터치하기"), _T("이미지 찾기"), _T("반복하기"), _T("기다리기"), _T("터치하기"), _T("키누르기"), _T("구분선"), _T("이동하기"), _T("돌아가기")};
	for(int i=0; i<EVENT_SIZE; i++)
		StrCpyW(events[i], list[i]);
	threadExit = false;

	imglist.Create(ICON_SIZE, ICON_SIZE, ILC_COLOR32 | ILC_MASK, 0, 0);
	
	// 아이콘 추가
	imglist.Add(AfxGetApp()->LoadIcon(IDI_ICON_IMAGE_TOUCH));
	imglist.Add(AfxGetApp()->LoadIcon(IDI_ICON_IMAGE_WAIT));
	imglist.Add(AfxGetApp()->LoadIcon(IDI_ICON_LOOP));
	imglist.Add(AfxGetApp()->LoadIcon(IDI_ICON_WAIT));
	imglist.Add(AfxGetApp()->LoadIcon(IDI_ICON_TOUCH));
	imglist.Add(AfxGetApp()->LoadIcon(IDI_ICON_PRESS_KEY));
	imglist.Add(AfxGetApp()->LoadIcon(IDI_ICON_SEPARATOR));
	imglist.Add(AfxGetApp()->LoadIcon(IDI_ICON_GOTO));
	imglist.Add(AfxGetApp()->LoadIcon(IDI_ICON_RETURN));
	//imglist.Add(AfxGetApp()->LoadIcon(IDI_ICON_DRAG));


	listEvents.InsertColumn(0, _T("아이템"), LVCFMT_LEFT, 132);
	listEvents.ModifyStyle(0, LVS_NOCOLUMNHEADER );
	DWORD dwExStyle = listEvents.GetExtendedStyle();
	listEvents.SetExtendedStyle ( dwExStyle |  LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP | LVS_EX_ONECLICKACTIVATE);
	listEvents.SetImageList(&imglist, LVSIL_NORMAL);
	listEvents.SetImageList(&imglist, LVSIL_SMALL);
	
	//DWORD dwExStyle = listEvents.GetExtendedStyle();
	//listEvents.SetExtendedStyle ( dwExStyle | LVS_EX_BORDERSELECT);
	//listEvents.Arrange(LVA_SNAPTOGRID);

	
	

	
	for(int i=0; i<EVENT_SIZE; i++)
	{
		LVITEM lvItem;
		//TTCHAR szBuffer[256];
		::ZeroMemory(&lvItem, sizeof(lvItem));
		//::ZeroMemory(szBuffer, sizeof(szBuffer));
		lvItem.mask = LVIF_TEXT | LVIF_IMAGE;
		
		lvItem.iItem = i;
		lvItem.iImage = i;
		lvItem.pszText = events[i];
		lvItem.cchTextMax = MAX_PATH;
		listEvents.InsertItem(&lvItem);

	}
	
	listPlaylist.InsertColumn(0, _T("아이템"), LVCFMT_LEFT, 188);
	listPlaylist.InsertColumn(1, _T("No"), LVCFMT_LEFT, 30);
	dwExStyle = listPlaylist.GetExtendedStyle();
	listPlaylist.SetExtendedStyle ( dwExStyle | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP | LVS_EX_ONECLICKACTIVATE);
	listPlaylist.ModifyStyle(0, LVS_NOCOLUMNHEADER);
	
	

	//DWORD dwExStyle = listPlaylist.GetExtendedStyle();
	//listPlaylist.SetExtendedStyle ( dwExStyle | LVS_EX_FULLROWSELECT);



	listPlaylist.SetImageList(&imglist, LVSIL_NORMAL);
	listPlaylist.SetImageList(&imglist, LVSIL_SMALL);


	sliderThreshold.SetRange(0,100);
	sliderThreshold.SetPos(85);
	sliderThreshold.SetTic(1);

	/*
	CMenu popUpLogSave;
	popUpLogSave.LoadMenu(IDR_MENU_LOG_SAVE);
	CRect rectBtn;
	editLogbox.GetWindowRect(&rectBtn);
	if(rectBtn.PtInRect(point))
	{

	}
	CMenu* popUpLogSaveSub = popUpLogSave.GetSubMenu(0);
	*/

	TCHAR thres[50];
	int thresInt = core->threshold * 100.f;
	wsprintf(thres, _T("%d"), thresInt);
	editThreshold = thresInt;
	UpdateData(false);
	//staticThreshold.SetWindowTextA(thres);
	
	rectDlg = new JhonnyRectDlg();
	rectDlg->main = this;
	rectDlg->Create(IDD_DIALOG_RECT, this);
	// 16 : 9
	
	//rectDlg->MoveWindow(0, 0, SEARCH_RECT_WIDTH + GetSystemMetrics(SM_CXEDGE)*8, SEARCH_RECT_HEGIHT + GetSystemMetrics(SM_CYCAPTION) +  GetSystemMetrics(SM_CYEDGE)*8);
	CRect rectRT;
	rectDlg->CenterWindow();
	rectDlg->GetWindowRect(&rectRT);
	rectRT.top -= 40;
	rectDlg->SetWindowPos(NULL, rectRT.left, rectRT.top, 0, 0, SWP_NOSIZE);

	rectDlg->ShowWindow(SW_SHOW);
	
	
	loadingDlg = new JhonnyLoadingDlg();
	loadingDlg->Create(IDD_DIALOG_LOADING, this);

	pFind.Close();
	SetCurrentDirectory(rootPath);


	CRect rc;	
	this->GetClientRect(&rc);
	this->SendMessage(WM_SIZE, (WPARAM)SIZE_RESTORED, MAKELPARAM(rc.Width(), rc.Height()));


	hotKeyID = ::GlobalAddAtom(_T("jhonnyHotkey"));

	if(!::RegisterHotKey(GetSafeHwnd(), hotKeyID, optMod, optVk))   // 추가
		AfxMessageBox(_T("핫키 등록 실패"));                                  // 추가
	
	if(userID.Compare(_T(GUEST_MODE_ID)) == 0)
		setGeustMode(true);

	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void JhonnyMain::setGeustMode(bool isGuest)
{
	isGuestMode = true;
	HMENU hMenu, pMenu;
	HWND hwnd;
	hwnd = GetSafeHwnd( );
	hMenu = ::GetMenu(hwnd);
	pMenu = GetSubMenu(hMenu, 0);  // 첫번째 메뉴
	EnableMenuItem(pMenu, 1, MF_BYPOSITION|MF_DISABLED); // 두번째 submenu
	EnableMenuItem(pMenu, 2, MF_BYPOSITION|MF_DISABLED); // 세번째 submenu
	EnableMenuItem(pMenu, 3, MF_BYPOSITION|MF_DISABLED); // 세번째 submenu
}


void JhonnyMain::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(pImageItemsDrag != NULL)
	{
		ClientToScreen(&point);
		
		CWnd* pWnd = CWnd::WindowFromPoint(point);
		if(pWnd != NULL)
		{
			if(this == pWnd || IsChild(pWnd))
			{
				pImageItemsDrag->DragEnter(NULL, point);
				pImageItemsDrag->DragMove(point);
			}
			else
				pImageItemsDrag->DragLeave(NULL);
		}
	}

	CDialogEx::OnMouseMove(nFlags, point);
}


void JhonnyMain::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	/*
	if(pTargetMainWindow == NULL || pTargetMainWindow->GetSafeHwnd() == this->GetSafeHwnd())
	{
		setTargetMainWndFromRectDlg();
		CString caption = _T("[스마트폰 화면을 넣어주세요. (800 x 450)] 매크로 타겟 -> ");
		CString wndName;
		pTargetMainWindow->GetWindowTextW(wndName);
		caption += wndName;
		SetWindowText(caption);
		SetIcon(	pTargetMainWindow->GetIcon(false), false	);
	}
	*/
	if(isGuestMode == true)
	{
		delete pImageItemsDrag;
		pImageItemsDrag = NULL;
		nIndexLeftSel = -1;
		nIndexRightSel = -1;
		pImageItemsDrag->DragLeave(NULL);
		pImageItemsDrag->EndDrag();
		ReleaseCapture();
		CDialogEx::OnLButtonUp(nFlags, point);
		if(IDYES == AfxMessageBox(_T("기능 제한모드에서는 사용하실 수 없는 기능입니다."),MB_YESNO )) 
		{
			const TCHAR* helpUrl = _T("http://cafe.naver.com/jhonnymacro");
			::ShellExecute(NULL, _T("open"), helpUrl, NULL, NULL, SW_SHOW);
		}
		return;
	}

	if(pImageItemsDrag != NULL)
	{
		setTargetMainWndFromRectDlg();
		ClientToScreen(&point);
		pImageItemsDrag->DragLeave(NULL);
		pImageItemsDrag->EndDrag();
		delete pImageItemsDrag;
		pImageItemsDrag = NULL;
		ReleaseCapture();

		CWnd* pWnd = CWnd::WindowFromPoint(point);
		if(pWnd == &listPlaylist && nIndexLeftSel >= 0 )		// 액션 끼워넣기
		{

			listPlaylist.ScreenToClient(&point);
			int nIndex = listPlaylist.HitTest(point);
			bool isAbove = ( (point.y) % (ICON_SIZE+1)) > ((ICON_SIZE+1)/2.0) ? false : true;

			TCHAR itemInfo[MAX_PATH]=_T("");
			// 아래쪽에 아이템 추가
			LVITEM lvItem;
			TCHAR szBuffer[256];
			::ZeroMemory(&lvItem, sizeof(lvItem));
			::ZeroMemory(szBuffer, sizeof(szBuffer));

			lvItem.mask = LVIF_TEXT | LVIF_IMAGE;
			lvItem.iItem = nIndexLeftSel;
			lvItem.pszText = szBuffer;
			lvItem.cchTextMax = 256;
			listEvents.GetItem(&lvItem);

			int index = 0;
			if(nIndex == -1)
				index = listPlaylist.GetItemCount();
			else
				index = nIndex+1;
			if(isAbove == true && nIndex != -1)
				index--;


			CString a;
			a.Format(_T("%d\n"),index);
			OutputDebugString(a);
			int result = insertEvent(lvItem.pszText, itemInfo, index);
			
			//strIndex.Format("%d",999);
			
			if( result == 0)
			{
				listPlaylist.InsertItem(index, itemInfo, lvItem.iImage);
				/*
				if(nIndexLeftSel == 6)
					listPlaylist.InsertItem(index, itemInfo, -1);
				else
					listPlaylist.InsertItem(index, itemInfo, lvItem.iImage);
					*/
			}
			else if( result > 0 )
				listPlaylist.InsertItem(index, itemInfo, result);
				
			// 번호 다시 매기기
			CString strIndex;
			for(int i=0; i<listPlaylist.GetItemCount(); i++)
			{
				
				strIndex.Format(_T("%d"),i+1);
				listPlaylist.SetItemText(i, 1, strIndex);
			}


			CString tmp;
			tmp.Format(_T("list:%d\n image:%d\n\n"), listPlaylist.GetItemCount(), imglist.GetImageCount() - EVENT_SIZE );
			OutputDebugString(tmp);
			needToSave = true;
			//actions.push_back(new EventTouch());
		}
		else if(pWnd == &listPlaylist && nIndexRightSel >= 0)				// 플레이 리스트에서 빼서 다시 넣기
		{
			listPlaylist.ScreenToClient(&point);
			int nIndex = listPlaylist.HitTest(point);
			if( nIndexRightSel != nIndex )
			{
				bool isAbove = ( (point.y) % (ICON_SIZE+1)) > ((ICON_SIZE+1)/2.0) ? false : true;
				CString tmp;
				tmp.Format(_T("%d-%d\n"), nIndex, isAbove);
				OutputDebugString(tmp);
				
				if(isCtrlPress==true)
				{
					// 복사하기
					TCHAR itemInfo[MAX_PATH]=_T("");
					LVITEM lvItem;
					TCHAR szBuffer[256]=_T("");
					::ZeroMemory(&lvItem, sizeof(lvItem));
					::ZeroMemory(szBuffer, sizeof(szBuffer));

					lvItem.mask = LVIF_TEXT | LVIF_IMAGE;
					lvItem.iItem = nIndexRightSel;
					lvItem.pszText = szBuffer;
					lvItem.cchTextMax = 256;
					listPlaylist.GetItem(&lvItem);

					//listPlaylist.DeleteItem( nIndexRightSel);
					int index = 0;
					if(nIndex == -1)
						index = listPlaylist.GetItemCount();
					else
					{
						if(nIndexRightSel < nIndex)
							index = nIndex;
						else
							index = nIndex+1;
					}
					if(isAbove == true && nIndex != -1)
						index--;
					lvItem.iItem = index;
					EventAction* pAction = actions.at(nIndexRightSel);
					int eventID = pAction->getEventID();
					if(eventID == 0 || eventID == 1 )
						lvItem.iImage = (imglist.GetImageCount()-1)+1;			// 이미지 리스트 하나추가될 것을 예상
					listPlaylist.InsertItem(&lvItem);

					// 번호 다시 매기기
					CString strIndex;
					for(int i=0; i<listPlaylist.GetItemCount(); i++)
					{

						strIndex.Format(_T("%d"),i+1);
						listPlaylist.SetItemText(i, 1, strIndex);
					}
					needToSave = true;
					//actions.erase(actions.begin() + nIndexRightSel);

					switch (eventID)
					{
					case 0:			// 이미지 터치
						{
							EventImageTouch* evt = (EventImageTouch*)actions.at(nIndexRightSel);
							CFileFind pFind;
							BOOL bRet; 
							int copyIntID = _ttoi(evt->item->getID());

							CString copyBitmapPath;
							CString copyInfoPath;
							CString copyBitmapPathTemp = evt->item->getBitmapPath();
							int idx = copyBitmapPathTemp.ReverseFind(_T('\\'));
							copyBitmapPathTemp = copyBitmapPathTemp.Left(idx);


							// 아이템 이미지 복사
							for(;;++copyIntID)
							{
								copyBitmapPath.Format(_T("%s\\%d.bmp"),copyBitmapPathTemp, copyIntID);
								copyInfoPath.Format(_T("%s\\%d.txt"),copyBitmapPathTemp, copyIntID);
								bRet = pFind.FindFile(copyBitmapPath); // 파일 존재여부 파악
								if(bRet == true)
									continue;
								else
								{
									CopyFile(evt->item->getBitmapPath(), copyBitmapPath, true);
									break;
								}
							}

							int fileExist = _waccess(copyInfoPath, 0);
							basic_ofstream<TCHAR> output(copyInfoPath,  std::ios::out | std::ios::trunc );  
							std::locale mylocale("");   // get global locale
							output.imbue(mylocale);

							if (!output)
							{ 
								std::cerr<<"Error writing to ..."<<std::endl; 
								output.close();
								return ;
							} 
							TCHAR copyID[10];
							wsprintf(copyID, _T("%d"), copyIntID);
							output << copyID << endl;
							output.close();
							TCHAR bitmapPath[MAX_PATH];
							StrCpy(bitmapPath, copyBitmapPath);

							if( evt->ifItems == NULL)
							{
								actions.insert(actions.begin() + index, new EventImageTouch(evt->name, new JhonnyItem(copyID, bitmapPath)));
								registerImage(((EventImageTouch*)(actions.at(index)))->item, -1);

								CString tmp;
								tmp.Format(_T("list:%d\n image:%d\n\n"), listPlaylist.GetItemCount(), imglist.GetImageCount() - EVENT_SIZE );
								OutputDebugString(tmp);


								break;
							}
							else
							{
								// 이프아이템 이미지 복사
								copyIntID = _ttoi(evt->ifItems->getID());
								CString copyIfBitmapPath = _T("");
								CString copyIfInfoPath = _T("");
								CString copyIfBitmapPathTemp = evt->item->getBitmapPath();
								int idx = copyIfBitmapPathTemp.ReverseFind(_T('\\'));
								copyIfBitmapPathTemp = copyIfBitmapPathTemp.Left(idx);

								for(;;++copyIntID)
								{
									copyIfBitmapPath.Format(_T("%s\\%d.bmp"),copyIfBitmapPathTemp, copyIntID);
									copyIfInfoPath.Format(_T("%s\\%d.txt"),copyIfBitmapPathTemp, copyIntID);
									bRet = pFind.FindFile(copyIfBitmapPath); // 파일 존재여부 파악
									if(bRet == true)
										continue;
									else
									{
										CopyFile(evt->ifItems->getBitmapPath(), copyIfBitmapPath, true);
										break;
									}
								}

								int fileExist = _waccess(copyIfInfoPath, 0);
								basic_ofstream<TCHAR> output(copyIfInfoPath,  std::ios::out | std::ios::trunc );  
								std::locale mylocale("");   // get global locale
								output.imbue(mylocale);

								if (!output)
								{ 
									std::cerr<<"Error writing to ..."<<std::endl; 
									output.close();
									return ;
								} 

								TCHAR copyifID[10];
								wsprintf(copyifID, _T("%d"), copyIntID);
								output << copyifID << endl;
								output.close();

								TCHAR bitmapiFPath[MAX_PATH];
								StrCpy(bitmapiFPath, copyIfBitmapPath);
								actions.insert(actions.begin() + index, new EventImageTouch(evt->name, new JhonnyItem(copyID, bitmapPath), new JhonnyItem(copyifID, bitmapiFPath)));
								registerImage(((EventImageTouch*)(actions.at(index)))->item, -1);

								CString tmp;
								tmp.Format(_T("list:%d\n image:%d\n\n"), listPlaylist.GetItemCount(), imglist.GetImageCount() - EVENT_SIZE );
								OutputDebugString(tmp);
							}

						
						}
						break;
					case 1:			// 이미지 찾기
						{
							EventImageWait* evt = (EventImageWait*)actions.at(nIndexRightSel);
							CFileFind pFind;
							BOOL bRet; 
							int copyIntID = _ttoi(evt->item->getID());

							CString copyBitmapPath;
							CString copyInfoPath;
							CString copyBitmapPathTemp = evt->item->getBitmapPath();
							int idx = copyBitmapPathTemp.ReverseFind(_T('\\'));
							copyBitmapPathTemp = copyBitmapPathTemp.Left(idx);


							// 아이템 이미지 복사
							for(;;++copyIntID)
							{
								copyBitmapPath.Format(_T("%s\\%d.bmp"),copyBitmapPathTemp, copyIntID);
								copyInfoPath.Format(_T("%s\\%d.txt"),copyBitmapPathTemp, copyIntID);
								bRet = pFind.FindFile(copyBitmapPath); // 파일 존재여부 파악
								if(bRet == true)
									continue;
								else
								{
									CopyFile(evt->item->getBitmapPath(), copyBitmapPath, true);
									break;
								}
							}

							int fileExist = _waccess(copyInfoPath, 0);
							basic_ofstream<TCHAR> output(copyInfoPath,  std::ios::out | std::ios::trunc );  
							std::locale mylocale("");   // get global locale
							output.imbue(mylocale);

							if (!output)
							{ 
								std::cerr<<"Error writing to ..."<<std::endl; 
								output.close();
								return ;
							} 
							TCHAR copyID[10];
							wsprintf(copyID, _T("%d"), copyIntID);
							output << copyID << endl;
							output.close();
							TCHAR bitmapPath[MAX_PATH];
							StrCpy(bitmapPath, copyBitmapPath);

							if( evt->ifItems == NULL)
							{
								actions.insert(actions.begin() + index, new EventImageWait(evt->name, new JhonnyItem(copyID, bitmapPath), NULL,
															evt->findGotoIndex, evt->nofindGotoIndex,
															evt->findGotoID, evt->nofindGotoID));
								registerImage(((EventImageWait*)(actions.at(index)))->item, -1);
								CString tmp;
								tmp.Format(_T("list:%d\n image:%d\n\n"), listPlaylist.GetItemCount(), imglist.GetImageCount() - EVENT_SIZE );
								OutputDebugString(tmp);
								break;
							}
							else
							{
								// 이프아이템 이미지 복사
								copyIntID = _ttoi(evt->ifItems->getID());
								CString copyIfBitmapPath = _T("");
								CString copyIfInfoPath = _T("");
								CString copyIfBitmapPathTemp = evt->item->getBitmapPath();
								int idx = copyIfBitmapPathTemp.ReverseFind(_T('\\'));
								copyIfBitmapPathTemp = copyIfBitmapPathTemp.Left(idx);

								for(;;++copyIntID)
								{
									copyIfBitmapPath.Format(_T("%s\\%d.bmp"),copyIfBitmapPathTemp, copyIntID);
									copyIfInfoPath.Format(_T("%s\\%d.txt"),copyIfBitmapPathTemp, copyIntID);
									bRet = pFind.FindFile(copyIfBitmapPath); // 파일 존재여부 파악
									if(bRet == true)
										continue;
									else
									{
										CopyFile(evt->ifItems->getBitmapPath(), copyIfBitmapPath, true);
										break;
									}
								}

								int fileExist = _waccess(copyIfInfoPath, 0);
								basic_ofstream<TCHAR> output(copyIfInfoPath,  std::ios::out | std::ios::trunc );  
								std::locale mylocale("");   // get global locale
								output.imbue(mylocale);

								if (!output)
								{ 
									std::cerr<<"Error writing to ..."<<std::endl; 
									output.close();
									return ;
								} 

								TCHAR copyifID[10];
								wsprintf(copyifID, _T("%d"), copyIntID);
								output << copyifID << endl;
								output.close();

								TCHAR bitmapiFPath[MAX_PATH];
								StrCpy(bitmapiFPath, copyIfBitmapPath);
								actions.insert(actions.begin() + index, new EventImageWait(evt->name, new JhonnyItem(copyID, bitmapPath),
													new JhonnyItem(copyifID, bitmapiFPath), evt->findGotoIndex, evt->nofindGotoIndex,
													evt->findGotoID, evt->nofindGotoID));
								registerImage(((EventImageWait*)(actions.at(index)))->item, -1);
								CString tmp;
								tmp.Format(_T("list:%d\n image:%d\n\n"), listPlaylist.GetItemCount(), imglist.GetImageCount() - EVENT_SIZE );
								OutputDebugString(tmp);
							}

							
						}
						break;
					case 2:			// 반복하기
						{
							EventLoop* evt = (EventLoop*)actions.at(nIndexRightSel);
							actions.insert(actions.begin() + index, new EventLoop(evt->loopCount, evt->goToIndex, evt->gotoID));
						}
						break;
					case 3:			// 기다리기
						{
							EventWait* evt = (EventWait*)actions.at(nIndexRightSel);
							actions.insert(actions.begin() + index, new EventWait(evt->millisec));
						}
						break;
					case 4:			// 터치하기
						{
							EventTouch* evt = (EventTouch*)actions.at(nIndexRightSel);
							actions.insert(actions.begin() + index, new EventTouch(evt->name, evt->x, evt->y, evt->dragX, evt->dragY, evt->isAbsolute, evt->isDrag, evt->isRightClick));
						}
						break;
					case 5:			// 키누르기
						{
							EventPressKey* evt = (EventPressKey*)actions.at(nIndexRightSel);
							actions.insert(actions.begin() + index, new EventPressKey(evt->vk, evt->mod));
						}
						break;
					case 6:			// 구분선
						{
							EventSeparator* evt = (EventSeparator*)actions.at(nIndexRightSel);
							actions.insert(actions.begin() + index, new EventSeparator(evt->name));
						}
						break;
					case 7:			// 이동하기
						{
							EventGoto* evt = (EventGoto*)actions.at(nIndexRightSel);
							actions.insert(actions.begin() + index, new EventGoto(evt->goToIndex, evt->gotoID));
						}
						break;
					case 8:			// 돌아가기
						{
							EventReturn* evt = (EventReturn*)actions.at(nIndexRightSel);
							actions.insert(actions.begin() + index, new EventReturn(evt->returnIndex, evt->returnID));
						}
						break;
					default:
						break;
					}

					
				}
				else
				{
					// 이동하기

					TCHAR itemInfo[MAX_PATH]=_T("");
					LVITEM lvItem;
					TCHAR szBuffer[256]=_T("");
					::ZeroMemory(&lvItem, sizeof(lvItem));
					::ZeroMemory(szBuffer, sizeof(szBuffer));

					lvItem.mask = LVIF_TEXT | LVIF_IMAGE;
					lvItem.iItem = nIndexRightSel;
					lvItem.pszText = szBuffer;
					lvItem.cchTextMax = 256;
					listPlaylist.GetItem(&lvItem);

					listPlaylist.DeleteItem( nIndexRightSel);
					int index = 0;
					if(nIndex == -1)
						index = listPlaylist.GetItemCount();
					else
					{
						if(nIndexRightSel < nIndex)
							index = nIndex;
						else
							index = nIndex+1;
					}
					if(isAbove == true && nIndex != -1)
						index--;
					lvItem.iItem = index;
					listPlaylist.InsertItem(&lvItem);

					// 번호 다시 매기기
					CString strIndex;
					for(int i=0; i<listPlaylist.GetItemCount(); i++)
					{

						strIndex.Format(_T("%d"),i+1);
						listPlaylist.SetItemText(i, 1, strIndex);
					}
					EventAction* pAction = actions.at(nIndexRightSel);
					actions.erase(actions.begin() + nIndexRightSel);
					actions.insert(actions.begin() + index, pAction);
					needToSave = true;
				}
				
			}
			//listPlaylist.InsertItem(index, &lvItem);
		}
		else if(pWnd != &listPlaylist && nIndexRightSel >= 0)				// 플레이 리스트 액션 삭제
		{
			POSITION listItemPos = listPlaylist.GetFirstSelectedItemPosition();
			int listItemIndex = listPlaylist.GetNextSelectedItem(listItemPos);
			if(listItemIndex != -1)
			{
				if(AfxMessageBox(_T("삭제하시겠습니까?"), MB_OKCANCEL|MB_ICONASTERISK) == IDOK)
				{
					int eventID = actions.at(listItemIndex)->getEventID();
					if(eventID == 0 || eventID == 1)
					{
						// 액션 이벤트 삭제
						EventImageTouch* eit = (EventImageTouch*)actions.at(listItemIndex);
						if(eit->item->ipl != NULL)
							cvReleaseImage(&eit->item->ipl);
						delete eit->item;

						// 이미지 당기기 지우기
						LVITEM lvItem;
						::ZeroMemory(&lvItem, sizeof(lvItem));
						lvItem.iItem = listItemIndex;
						lvItem.mask = LVIF_IMAGE;
						listPlaylist.GetItem(&lvItem);
						int imageListIndex = lvItem.iImage;


						for(int i=0; i<listPlaylist.GetItemCount(); i++)
						{
							::ZeroMemory(&lvItem, sizeof(lvItem));
							lvItem.iItem = i;
							lvItem.mask = LVIF_IMAGE;
							listPlaylist.GetItem(&lvItem);
							if(lvItem.iImage >=  imageListIndex)
								lvItem.iImage = lvItem.iImage-1;
							listPlaylist.SetItem(&lvItem);
						}
						imglist.Remove(imageListIndex);

					}
					listPlaylist.DeleteItem(listItemIndex);
					actions.erase(actions.begin()+listItemIndex);

					// 번호 다시 매기기
					CString strIndex;
					for(int i=listItemIndex; i<listPlaylist.GetItemCount(); i++)
					{

						strIndex.Format(_T("%d"),i+1);
						listPlaylist.SetItemText(i, 1, strIndex);
					}

					needToSave = true;
					CString tmp;
					tmp.Format(_T("list:%d\n image:%d\n\n"), listPlaylist.GetItemCount(), imglist.GetImageCount() - EVENT_SIZE);
					OutputDebugString(tmp);
				}
			}
		}
		
		
		nIndexLeftSel = -1;
		nIndexRightSel = -1;
		refreshGoToLink();
		
	}
	

	CDialogEx::OnLButtonUp(nFlags, point);
}


void JhonnyMain::OnLvnBegindragListEvent(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	CPoint ptDrag, ptAction;
	nIndexLeftSel = pNMLV->iItem;
	pImageItemsDrag = listEvents.CreateDragImage(pNMLV->iItem, &ptDrag);
	pImageItemsDrag->SetBkColor(RGB(255,255,255));
	ptAction = pNMLV->ptAction;

	SetCapture();
	pImageItemsDrag->BeginDrag(0, ptAction - ptDrag);
	listEvents.ClientToScreen(&ptAction);
	pImageItemsDrag->DragEnter(NULL, ptAction);

	*pResult = 0;
}

void JhonnyMain::OnLvnBegindragListPlaylist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	CPoint ptDrag, ptAction, ptDown;
	nIndexRightSel = pNMLV->iItem;
	pImageItemsDrag = listPlaylist.CreateDragImage(pNMLV->iItem, &ptDrag);
	pImageItemsDrag->SetBkColor(RGB(255,255,255));
	ptAction = pNMLV->ptAction;
	//ptDown = ptDrag;
	

	SetCapture();
	pImageItemsDrag->BeginDrag(0, ptAction - ptDrag);
	listPlaylist.ClientToScreen(&ptAction);
	pImageItemsDrag->DragEnter(NULL, ptAction);

	


	*pResult = 0;
}




void JhonnyMain::setTargetMainWndFromRectDlg()
{
	RECT rect = getDlgRectRect();
	POINT pt;
	pt.x = rect.left + (rect.right - rect.left) / 2.0;
	pt.y = rect.top + (rect.bottom - rect.top) / 2.0;

	HWND tempTarget;
	CWnd* tempMain;

	tempTarget = targetWindow;
	tempMain = pTargetMainWindow;
	// 타겟 부모 윈도우 구함
	targetWindow = ::WindowFromPoint(pt);
	
	if(NULL != targetWindow)
	{
		pTargetMainWindow = CWnd::FromHandle (targetWindow);
		while(pTargetMainWindow->GetParent() != NULL)
			pTargetMainWindow = pTargetMainWindow->GetParent();

		pTargetMainWindow->GetWindowRect(&targetMainWindowRect);

	}

	if(targetWindow == NULL || pTargetMainWindow == NULL || pTargetMainWindow == this)
	{
		targetWindow = tempTarget;
		pTargetMainWindow = tempMain;
		return;
	}


	CString caption = _T("[스마트폰 화면을 넣어주세요. (800 x 450)] 매크로 타겟 ==> ");
	CString wndName;
	pTargetMainWindow->GetWindowTextW(wndName);
	caption += wndName;
	rectDlg->SetWindowText(caption);
	//rectDlg->SetIcon(pTargetMainWindow->GetIcon(false), false);
	rectDlg->SetIcon(MfcUtil::GetAppIcon(pTargetMainWindow->GetSafeHwnd()), false);
	
	
}

void JhonnyMain::playAndStop()
{
	

	CString strLine;
	if(listPlaylist.IsWindowEnabled())
	{
		/*
		if(IsIconic())
		{
			AfxMessageBox(_T("최소화 된 상태에서는 시작할 수 없습니다."));
			return;
		}
		*/


		rectDlg->setMoveable(false);

		if(IsWindow(this->GetSafeHwnd()))
		{
		
			setTargetMainWndFromRectDlg();
			/*
			if(((JhonnyMain*)main)->pTargetMainWindow == ((JhonnyMain*)main))
				return ;
				*/
		}


		
		POSITION listItemPos = listPlaylist.GetFirstSelectedItemPosition();
		int listItemIndex = listPlaylist.GetNextSelectedItem(listItemPos);
		if(listItemIndex == -1)
			startIndex = 0;
		else
			startIndex = listItemIndex;

		for(int i=0; i<actions.size(); i++)
			actions.at(i)->doReset();

		btnStart.EnableWindow(false);
		threadExit = false;
		
		listEvents.EnableWindow(false);
		listPlaylist.EnableWindow(false);
		btnStart.SetWindowText(_T("중지하기"));
		
		strLine.Format(_T("프로그램이 시작되었습니다.\r\n"));
		logData.Append(getLogTime());
		logData.Append(strLine);
		int nLength = editLogbox.GetWindowTextLength();
		editLogbox.SetSel(nLength, nLength);
		editLogbox.ReplaceSel(strLine);
		editLogbox.SetScrollPos(SB_HORZ, 0);

		
		
		
		doPlay();
		//Sleep(1000);
		btnStart.EnableWindow(true);
	}
	else
	{
		threadExit = true;
		rectDlg->setMoveable(true);
		btnStart.EnableWindow(false);

		
		
		listEvents.EnableWindow(true);
		listPlaylist.EnableWindow(true);
		btnStart.SetWindowText(_T("시작하기"));
		
		strLine.Format(_T("프로그램이 중지되었습니다.\r\n"));
		logData.Append(getLogTime());
		logData.Append(strLine);
		int nLength = editLogbox.GetWindowTextLength();
		editLogbox.SetSel(nLength, nLength);
		editLogbox.ReplaceSel(strLine);
		editLogbox.SetScrollPos(SB_HORZ, 0);

		setListItemFocus(0);

		Sleep(1000);
		btnStart.EnableWindow(true);
	}
	
	
}

void JhonnyMain::OnBnClickedButtonPlay()
{
	// TODO: Add your control notification handler code here
	playAndStop();
	//EventSend::SendMouseEvent(targetWindow, 35, 100, MOUSE_LCLICK);
	//EventSend::SendMouseEvent(targetWindow, 10, 10, MOUSE_LCLICK);
	
}

BOOL JhonnyMain::refreshGoToLink()
{
	LVITEM lvItem;
	TCHAR szBuffer[256]=_T("");
	TCHAR itemInfo[MAX_PATH]=_T("");

	for(int i=0; i<actions.size(); i++)
	{
		if(actions.at(i)->getEventID() == 1)
		{
			EventImageWait* eiw = (EventImageWait*)actions.at(i);

			// find go to 링크
			int index = eiw->findGotoIndex - 1;
			if( index > actions.size()-1)			// 리스트 사이즈를 넘어갈 경우
			{
				//OutputDebugString("find 사이즈 범위 밖\n");
				eiw->findGotoIndex = i+1;
				index = eiw->findGotoIndex-1;
			}

			if(StrCmpW(eiw->findGotoID, actions.at(index)->getID())==0)			// 인덱스와 액션링크가 바르게 연결 되있을 경우
				;//OutputDebugString("find 올바른 링크\n");
			else if(StrCmpW(eiw->findGotoID, _T("")) == 0)		// 설정된 액션링크가 없을 경우
			{
				;//OutputDebugString("find 링크 생성\n");
				StrCpyW(eiw->findGotoID, actions.at(index)->getID());
			}
			else      // 액션링크가 바뀌었을 경우
			{
				int j;

				// 플레이리스트에서 찾을 경우 
				for(j=0; j<actions.size(); j++)
				{
					if(StrCmpW(eiw->findGotoID, actions.at(j)->getID()) == 0)
					{
						;//OutputDebugString("find 링크 다시 찾음\n");
						eiw->findGotoIndex = j+1;
						break;
					}
				}

				// 플레이리스트에서 찾지 못한 경우 자기 자신을 가르킴
				if(j == actions.size())
				{
					;//OutputDebugString("find 못 찾아 자기자신 연결\n");
					eiw->findGotoIndex = i+1;
					StrCpyW(eiw->findGotoID, eiw->getID());
				}
			}

			// no find go to 링크
			index = eiw->nofindGotoIndex - 1;
			if( index > actions.size()-1)			// 리스트 사이즈를 넘어갈 경우
			{
				;//OutputDebugString("find 사이즈 범위 밖\n");
				eiw->nofindGotoIndex = i+1;
				index = eiw->nofindGotoIndex - 1;
			}

			if(StrCmpW(eiw->nofindGotoID, actions.at(index)->getID())==0)			// 인덱스와 액션링크가 바르게 연결 되있을 경우
				;//OutputDebugString("No find 올바른 링크\n");
			else if(StrCmpW(eiw->nofindGotoID, _T("")) == 0)		// 설정된 액션링크가 없을 경우
			{
				;//OutputDebugString("No Find 링크 생성\n");
				StrCpyW(eiw->nofindGotoID, actions.at(index)->getID());
			}
			else      // 액션링크가 바뀌었을 경우
			{
				int j;

				// 플레이리스트에서 찾을 경우 
				for(j=0; j<actions.size(); j++)
				{
					if(StrCmpW(eiw->nofindGotoID, actions.at(j)->getID()) == 0)
					{
						;//OutputDebugString("No find 링크 다시 찾음\n");
						eiw->nofindGotoIndex = j+1;
						break;
					}
				}

				// 플레이리스트에서 찾지 못한 경우 자기 자신을 가르킴
				if(j == actions.size())
				{
					;//OutputDebugString("No Find 못 찾아 자기자신 연결\n");
					eiw->nofindGotoIndex= i+1;
					StrCpyW(eiw->nofindGotoID, eiw->getID());
				}
			}
		}
		else if(actions.at(i)->getEventID() == 2)
		{
			EventLoop* eiw = (EventLoop*)actions.at(i);
			int index = eiw->goToIndex - 1;
			if( index > actions.size()-1)			// 리스트 사이즈를 넘어갈 경우
			{
				;//OutputDebugString("find 사이즈 범위 밖\n");
				eiw->goToIndex = i+1;
				index = eiw->goToIndex-1;
			}

			if(StrCmpW(eiw->gotoID, actions.at(index)->getID())==0)			// 인덱스와 액션링크가 바르게 연결 되있을 경우
				;//OutputDebugString("find 올바른 링크\n");
			else if(StrCmpW(eiw->gotoID, _T("")) == 0)		// 설정된 액션링크가 없을 경우
			{
				;//OutputDebugString("find 링크 생성\n");
				StrCpyW(eiw->gotoID, actions.at(index)->getID());

				
				// 리스트 아이템 내용 수정
				StrCpyW(itemInfo, _T(""));
				::ZeroMemory(&lvItem, sizeof(lvItem));
				::ZeroMemory(szBuffer, sizeof(szBuffer));
				lvItem.mask = LVIF_TEXT;
				lvItem.iItem = i;
				lvItem.pszText = szBuffer;
				lvItem.cchTextMax = 256;
				listPlaylist.GetItem(&lvItem);
				wsprintf(itemInfo, _T("%d 이동, %d번 %s"), eiw->goToIndex, eiw->loopCount, _T("반복하기"));
				lvItem.pszText = itemInfo;
				listPlaylist.SetItem(&lvItem);
			}
			else      // 액션링크가 바뀌었을 경우
			{
				int j;

				// 플레이리스트에서 찾을 경우 
				for(j=0; j<actions.size(); j++)
				{
					if(StrCmpW(eiw->gotoID, actions.at(j)->getID()) == 0)
					{
						;//OutputDebugString("find 링크 다시 찾음\n");
						eiw->goToIndex = j+1;

						// 리스트 아이템 내용 수정
						StrCpyW(itemInfo, _T(""));
						::ZeroMemory(&lvItem, sizeof(lvItem));
						::ZeroMemory(szBuffer, sizeof(szBuffer));
						lvItem.mask = LVIF_TEXT;
						lvItem.iItem = i;
						lvItem.pszText = szBuffer;
						lvItem.cchTextMax = 256;
						listPlaylist.GetItem(&lvItem);
						wsprintf(itemInfo, _T("%d 이동, %d번 %s"), eiw->goToIndex, eiw->loopCount, _T("반복하기"));
						lvItem.pszText = itemInfo;
						listPlaylist.SetItem(&lvItem);

						break;
					}
				}

				// 플레이리스트에서 찾지 못한 경우 자기 자신을 가르킴
				if(j == actions.size())
				{
					;//OutputDebugString("find 못 찾아 자기자신 연결\n");
					eiw->goToIndex = i+1;
					StrCpyW(eiw->gotoID, eiw->getID());
					// 리스트 아이템 내용 수정
					StrCpyW(itemInfo, _T(""));
					::ZeroMemory(&lvItem, sizeof(lvItem));
					::ZeroMemory(szBuffer, sizeof(szBuffer));
					lvItem.mask = LVIF_TEXT;
					lvItem.iItem = i;
					lvItem.pszText = szBuffer;
					lvItem.cchTextMax = 256;
					listPlaylist.GetItem(&lvItem);
					wsprintf(itemInfo, _T("%d 이동, %d번 %s"), eiw->goToIndex, eiw->loopCount, _T("반복하기"));
					lvItem.pszText = itemInfo;
					listPlaylist.SetItem(&lvItem);
				}
			}
		}
		else if(actions.at(i)->getEventID() == 7)
		{
			EventGoto* eiw = (EventGoto*)actions.at(i);
			int index = eiw->goToIndex - 1;
			if( index > actions.size()-1)			// 리스트 사이즈를 넘어갈 경우
			{
				;//OutputDebugString("find 사이즈 범위 밖\n");
				eiw->goToIndex = i+1;
				index = eiw->goToIndex-1;
			}

			if(StrCmpW(eiw->gotoID, actions.at(index)->getID())==0)			// 인덱스와 액션링크가 바르게 연결 되있을 경우
				;//OutputDebugString("find 올바른 링크\n");
			else if(StrCmpW(eiw->gotoID, _T("")) == 0)		// 설정된 액션링크가 없을 경우
			{
				;//OutputDebugString("find 링크 생성\n");
				StrCpyW(eiw->gotoID, actions.at(index)->getID());

				// 리스트 아이템 내용 수정
				StrCpyW(itemInfo, _T(""));
				::ZeroMemory(&lvItem, sizeof(lvItem));
				::ZeroMemory(szBuffer, sizeof(szBuffer));
				lvItem.mask = LVIF_TEXT;
				lvItem.iItem = i;
				lvItem.pszText = szBuffer;
				lvItem.cchTextMax = 256;
				listPlaylist.GetItem(&lvItem);
				wsprintf(itemInfo, _T("%d로 %s"),  eiw->goToIndex, _T("이동하기"));
				lvItem.pszText = itemInfo;
				listPlaylist.SetItem(&lvItem);
			}
			else      // 액션링크가 바뀌었을 경우
			{
				int j;

				// 플레이리스트에서 찾을 경우 
				for(j=0; j<actions.size(); j++)
				{
					if(StrCmpW(eiw->gotoID, actions.at(j)->getID()) == 0)
					{
						;//OutputDebugString("find 링크 다시 찾음\n");
						eiw->goToIndex = j+1;

						// 리스트 아이템 내용 수정
						StrCpyW(itemInfo, _T(""));
						::ZeroMemory(&lvItem, sizeof(lvItem));
						::ZeroMemory(szBuffer, sizeof(szBuffer));
						lvItem.mask = LVIF_TEXT;
						lvItem.iItem = i;
						lvItem.pszText = szBuffer;
						lvItem.cchTextMax = 256;
						listPlaylist.GetItem(&lvItem);
						wsprintf(itemInfo, _T("%d로 %s"),  eiw->goToIndex, _T("이동하기"));
						lvItem.pszText = itemInfo;
						listPlaylist.SetItem(&lvItem);


						break;
					}
				}

				// 플레이리스트에서 찾지 못한 경우 자기 자신을 가르킴
				if(j == actions.size())
				{
					;//OutputDebugString("find 못 찾아 자기자신 연결\n");
					eiw->goToIndex = i+1;
					StrCpyW(eiw->gotoID, eiw->getID());
					// 리스트 아이템 내용 수정
					StrCpyW(itemInfo, _T(""));
					::ZeroMemory(&lvItem, sizeof(lvItem));
					::ZeroMemory(szBuffer, sizeof(szBuffer));
					lvItem.mask = LVIF_TEXT;
					lvItem.iItem = i;
					lvItem.pszText = szBuffer;
					lvItem.cchTextMax = 256;
					listPlaylist.GetItem(&lvItem);
					wsprintf(itemInfo, _T("%d로 %s"),  eiw->goToIndex, _T("이동하기"));
					lvItem.pszText = itemInfo;
					listPlaylist.SetItem(&lvItem);
				}
			}
		}
	}

	listPlaylist.RedrawWindow();
	return TRUE;
}

void JhonnyMain::selectItem(CListCtrl* plist, int index)
{
	plist->SetItemState(index, LVIS_SELECTED, LVIS_SELECTED); 
	plist->EnsureVisible(index, false); 
	plist->SetFocus();
}


void JhonnyMain::doPlay()
{
	setListItemFocus(0);
	DWORD lpExitCode = 0;
	GetExitCodeThread(threadHandle, &lpExitCode); 
	//Output("result : %d \n", lpExitCode);

	if ( lpExitCode == STILL_ACTIVE )
		;//Output("now Running.. \n", lpExitCode);
	else
	{
		if( threadExit == false)
			threadHandle = (HANDLE) _beginthreadex(0, 0, (unsigned int (__stdcall *)(void *))threadDoPlay, (JhonnyMain*)AfxGetMainWnd(), 0, (unsigned int *)&threadID);
	}
}
HWND JhonnyMain::getTargetHandleFromPoint(int inputX, int inputY, int *transCoordX, int *transCoordY)
{
	
	if(pTargetMainWindow == NULL || IsWindow(pTargetMainWindow->GetSafeHwnd()) == false)
		return NULL;

	int outCoord = 0;
	if(IsIconic())
		outCoord = (int)GetSystemMetrics(SM_CYSCREEN);


	RECT rectRT;
	rectDlg->GetClientRect(&rectRT);
	rectDlg->ClientToScreen(&rectRT);
	
	RECT rectParent;
	pTargetMainWindow->GetClientRect(&rectParent);
	pTargetMainWindow->ClientToScreen(&rectParent);

				
				
				
	POINT handlePt;
	handlePt.x = rectRT.left + inputX - rectParent.left;
	handlePt.y = rectRT.top + inputY - rectParent.top + outCoord;	

	//CWnd* pWndMainTmp = pTargetMainWindow;
				
	CWnd* pWndMainTmp = pTargetMainWindow;
				
	// CWP_SKIPDISABLED | CWP_SKIPINVISIBLE | CWP_SKIPTRANSPARENT
	HWND hTempHandle = ::ChildWindowFromPointEx(pWndMainTmp->GetSafeHwnd(), handlePt,  CWP_SKIPDISABLED | CWP_SKIPINVISIBLE );
	
	// NULL 이면 Minimized 안된 곳에서 찾음
	//bool isMinimizedTemp = isMainWindowMinimized;
	if(hTempHandle == NULL)
	{
		handlePt.y -= outCoord;
		hTempHandle = ::ChildWindowFromPointEx(pWndMainTmp->GetSafeHwnd(), handlePt,  CWP_SKIPDISABLED | CWP_SKIPINVISIBLE );
		//isMainWindowMinimized = false;
	}
	// 그래도 NULL 이면 오류로 인정하고 실패리턴
	if(hTempHandle == NULL)
	{
		//isMainWindowMinimized = isMinimizedTemp;
		return NULL;
	}

	HWND hTargetHandle = hTempHandle;
	CWnd* pWndTarget;

	// 최하위 핸들을 구함
	while(hTempHandle != NULL)
	{
		CRect r;
		CWnd* pWndChildTmp = CWnd::FromHandle(hTempHandle);

		pWndChildTmp->GetWindowRect( r ); 
		pWndMainTmp->ScreenToClient( r ); 
		handlePt.x = handlePt.x - r.left;
		handlePt.y = handlePt.y - r.top;
					
		hTempHandle = ::ChildWindowFromPointEx(pWndChildTmp->GetSafeHwnd(), handlePt, CWP_SKIPDISABLED | CWP_SKIPINVISIBLE );	
			
		if(hTargetHandle == hTempHandle || hTempHandle == NULL)
			break;

		hTargetHandle = hTempHandle;
					
	}

	// 최하위 핸들이 마우스 이벤트를 받지 않는 핸들이면 상위 핸들로 교체
	while(hTargetHandle != NULL)
	{
		if(::IsWindowEnabled(hTargetHandle)==true)
			break;
		hTargetHandle = ::GetParent(hTargetHandle);
	}
	
	/*
	
	*/
	//core->setTargetWindow(hTargetHandle);

	RECT WndTargetRT;

	pWndTarget = CWnd::FromHandle (hTargetHandle);
	//CWnd* pWndTarget = CWnd::FromHandle (hTargetHandle);
	//pWndTarget->GetWindowRect(&WndTargetRT);

	pWndTarget->GetClientRect(&WndTargetRT);
	pWndTarget->ClientToScreen(&WndTargetRT);

	rectDlg->GetClientRect(&rectDlgRect);
	rectDlg->ClientToScreen(&rectDlgRect);

	
	*transCoordX = rectDlgRect.left - WndTargetRT.left;
	*transCoordY = rectDlgRect.top - WndTargetRT.top + outCoord;
	
	return hTargetHandle;
}

void JhonnyMain::playCore()
{
	
	
	CString temp;
	CString strLine;
	int i = startIndex;
	startIndex = 0;
	for(; i<actions.size(); i++)
	{
		temp.Format(_T("%d"), i+1);
		if(IsIconic())
			viewer->setNowNum(temp);
		while(isPause == true)
		{
			Sleep(1000);
			if(threadExit==true)
			{
				isPause = false;
				break;
			}
		}

		if(threadExit==true)
			break;
		setListItemFocus(0);
		selectItem(&listPlaylist, i);
		switch (actions.at(i)->getEventID())
		{
			/*
		case ID_IMAGE_TOUCH:
		case ID_TOUCH:
			{
				int outCoord = 0;
				if(isMainWindowMinimized)
					outCoord = (int)GetSystemMetrics(SM_CYSCREEN);


				RECT rectRT;
				rectDlg->GetClientRect(&rectRT);
				rectDlg->ClientToScreen(&rectRT);
	
				RECT rectParent;
				pTargetMainWindow->GetClientRect(&rectParent);
				pTargetMainWindow->ClientToScreen(&rectParent);
				
				
				
				POINT handlePt;
				handlePt.x = rectRT.left + ((EventTouch*)actions.at(i))->x - rectParent.left;
				handlePt.y = rectRT.top + ((EventTouch*)actions.at(i))->y - rectParent.top + outCoord;	

				//CWnd* pWndMainTmp = pTargetMainWindow;
				
					CWnd* pWndMainTmp = pTargetMainWindow;
				
				HWND hTempHandle = ::ChildWindowFromPointEx(pWndMainTmp->GetSafeHwnd(), handlePt, CWP_SKIPDISABLED | CWP_SKIPINVISIBLE | CWP_SKIPTRANSPARENT);	
				HWND hTargetHandle = hTempHandle;
				CWnd* pWndTarget;
				while(hTempHandle != NULL)
				{
					CRect r;
					CWnd* pWndChildTmp = CWnd::FromHandle(hTempHandle);

					pWndChildTmp->GetWindowRect( r ); 
					pWndMainTmp->ScreenToClient( r ); 
					handlePt.x = handlePt.x - r.left;
					handlePt.y = handlePt.y - r.top;
					
					hTempHandle = ::ChildWindowFromPointEx(pWndChildTmp->GetSafeHwnd(), handlePt, CWP_SKIPDISABLED | CWP_SKIPINVISIBLE | CWP_SKIPTRANSPARENT);	
			
					if(hTargetHandle == hTempHandle || hTempHandle == NULL)
						break;

					hTargetHandle = hTempHandle;
					
				}

				core->setTargetWindow(hTargetHandle);

				RECT WndTargetRT;
				POINT transCoord;
				pWndTarget = CWnd::FromHandle (hTargetHandle);
				//CWnd* pWndTarget = CWnd::FromHandle (hTargetHandle);
				pWndTarget->GetWindowRect(&WndTargetRT);
				rectDlg->GetClientRect(&rectDlgRect);
				rectDlg->ClientToScreen(&rectDlgRect);

		

				transCoord.x = rectDlgRect.left - WndTargetRT.left;
				transCoord.y = rectDlgRect.top - WndTargetRT.top + outCoord;
					

				core->setTransCoord(transCoord);
				
				

			}
			break;
			*/
		case ID_GOTO:
			returnIndexMain = i+1;
			break;
		default:
			break;
		}
			
		int result = actions.at(i)->doAction(this);
		
		
		
		
		strLine=_T("");
		strLine.Format(_T("%d. "), i+1);
		strLine.Append(actions.at(i)->eventLog);
		strLine.Append(_T("\r\n"));
		logData.Append(getLogTime());
		logData.Append(strLine);


		int nLength = editLogbox.GetWindowTextLength();
		if( nLength > 29900 ) 
		{
			editLogbox.SetWindowText(_T(""));
			editLogbox.Clear();
			nLength = editLogbox.GetWindowTextLength();
		}
		editLogbox.SetSel(nLength, nLength);
		editLogbox.ReplaceSel(strLine);
		editLogbox.SetScrollRange(SB_HORZ, 0, 0);
		editLogbox.SetScrollPos(SB_HORZ, 0);
		

		CString tmp;
		tmp.Format( _T("editLogbox : %d\n"), nLength);
		OutputDebugString(tmp);

		if( result == -1) 
		{
			break;
		}
		else if( result >= 1)
		{
			i = result-2; // i++과 index가 1부터 시작하기 때문에 보정
			int size = actions.size();
			if( i > size-1  )
			{
				CString tmp;
				tmp.Format( _T("%d %d\n"), i, actions.size());
				OutputDebugString(tmp);
				break;
			}
		}
		
		if(++pcounter%500 == 0)
		{
			pcounter = 0;

			

			char loginID[256]={0,};
			char loginPass[256]={0,};
			WideCharToMultiByte(CP_ACP, 0,  LPCTSTR(userID), 256, loginID, userID.GetLength(), NULL, NULL);
			WideCharToMultiByte(CP_ACP, 0,  LPCTSTR(userPass), 256, loginPass, userPass.GetLength(), NULL, NULL);

			int result = parse->signIn(loginID, loginPass);
			
			if(userID.Compare(_T(GUEST_MODE_ID)) == 0)
				setGeustMode(true);
			else if(result != 0)
			{
				AfxMessageBox(_T("Network Error(Parse):\n인증되지 않은 아이디 또는 비밀번호입니다.\n프로그램을 종료합니다."));
				EndDialog(-1);
			}
			

		}
		Sleep(optDelay*1000);
		

	}

	if( i == actions.size() && !listPlaylist.IsWindowEnabled())
	{
		// error
		if(IsIconic())
			viewer->doStop();
		else
			playAndStop();
	}
	startIndex = 0;
	
}

void JhonnyMain::setListItemFocus(int status)
{
	POSITION pos = listEvents.GetFirstSelectedItemPosition();
	while( pos ) {
		int index = listEvents.GetNextSelectedItem( pos );
		listEvents.SetItemState(index, status, LVIS_SELECTED);
	}

	pos = listPlaylist.GetFirstSelectedItemPosition();
	while( pos ) {
		int index = listPlaylist.GetNextSelectedItem( pos );
		listPlaylist.SetItemState(index, status, LVIS_SELECTED);
	}
}

int JhonnyMain::registerImage(JhonnyItem* item, int index)
{
	
	/*
	JhonnyAutoCore core;
	core.hBitmap2Ipl(&hBitSelect, &item->ipl);
	*/
	
	//pPrentWnd->items.push_back(new JhonnyItem(item));

	HBITMAP hBitSelect = NULL;
	// 이미지 리스트에 추가
	hBitSelect = (HBITMAP)::LoadImage(NULL, item->getBitmapPath(), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR | LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
	if( hBitSelect == NULL )
		return -1;
			
	BITMAP bm = {0};
	GetObject(hBitSelect, sizeof(BITMAP), &bm);

	int cx = ICON_SIZE;
	int cy = ICON_SIZE;


	CPaintDC dc(this);
	//HDC dc;
	HDC memdc = CreateCompatibleDC(dc);
	HDC memdc2 = CreateCompatibleDC(dc);
	HBITMAP hbm = CreateCompatibleBitmap(dc, cx, cy);
	HGDIOBJ hOld = SelectObject(memdc, hbm);
	SelectObject(memdc2, hBitSelect);
	StretchBlt(memdc, 0, 0, cx, cy, memdc2, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

	SelectObject(memdc, hOld);
	DeleteDC(dc);
	DeleteDC(memdc);
	DeleteDC(memdc2);


	CBitmap lamp_image ;
	lamp_image.Attach(hbm);

	

	if(index== -1)
	{
		
		imglist.Add(&lamp_image, RGB(255, 255, 255));
		if(hBitSelect != NULL) ::DeleteObject(hBitSelect);
		return imglist.GetImageCount()-1;
	}
	else 
	{
		imglist.Replace(index, &lamp_image, NULL);
		if(hBitSelect != NULL) ::DeleteObject(hBitSelect);
		return index;
	}


	// 오브젝트 삭제
	
}

int JhonnyMain::modifyEvent(int index, TCHAR* eventItemInfo)
{
	//TCHAR eventItemInfo[MAX_PATH];
	EventAction* action = actions.at(index);
	switch (action->getEventID())
	{
	case 0:			// 이미지 터치하기
		{
			
			rectDlg->ShowWindow(SW_HIDE);
			ShowWindow(SW_HIDE);
			JhonnyRegister dlgReg;
			EventImageTouch* evImageTouch = (EventImageTouch*)action;
			OutputDebugString(evImageTouch->id);
			OutputDebugString(_T("\n"));
			dlgReg.setEventImageTouch(evImageTouch);
			
			INT_PTR response = dlgReg.DoModal();
			if(response == IDOK)
			{
				rectDlg->ShowWindow(SW_SHOW);
				ShowWindow(SW_SHOW);
				StrCpyW(evImageTouch->name, dlgReg.itemName);
				wsprintf(eventItemInfo, _T("\"%s\" %s"), evImageTouch->name, _T("터치하기"));

				if(dlgReg.imgIndex == -1)		
				{
					if(dlgReg.isIfImageSetted == true)
					{
						if(evImageTouch->ifItems != NULL)
						{
							cvReleaseImage(&evImageTouch->ifItems->ipl);
							delete evImageTouch->ifItems;
						}
						evImageTouch->ifItems = new JhonnyItem(dlgReg.ifItem.getID(), dlgReg.ifItem.getBitmapPath());
					}

					if(dlgReg.ifImageDeleted == true)
					{
						if(evImageTouch->ifItems != NULL)
						{
							cvReleaseImage(&evImageTouch->ifItems->ipl);
							delete evImageTouch->ifItems;
						}
						evImageTouch->ifItems = NULL;
					}
					needToSave = true;
					return 0;
					//evImageTouch->item = &(*items.at(sel));
					//sel = items.size()-1;

				}
				else     // 기존 이미지로 변경
				{
					if(dlgReg.isIfImageSetted == true)
					{
						if(evImageTouch->ifItems != NULL)
						{
							cvReleaseImage(&evImageTouch->ifItems->ipl);
							delete evImageTouch->ifItems;
						}
						evImageTouch->ifItems = new JhonnyItem(dlgReg.ifItem.getID(), dlgReg.ifItem.getBitmapPath());
					}
					
					if(dlgReg.ifImageDeleted == true)
					{
						if(evImageTouch->ifItems != NULL)
						{
							cvReleaseImage(&evImageTouch->ifItems->ipl);
							delete evImageTouch->ifItems;
						}
						evImageTouch->ifItems = NULL;
					}
						


					if(evImageTouch->item->ipl != NULL)
						cvReleaseImage(&evImageTouch->item->ipl);
					delete evImageTouch->item;
					evImageTouch->item = new JhonnyItem(dlgReg.item.getID(), dlgReg.item.getBitmapPath());

					LVITEM lvItem;
					::ZeroMemory(&lvItem, sizeof(lvItem));
					lvItem.iItem = index;
					lvItem.mask = LVIF_IMAGE;
					listPlaylist.GetItem(&lvItem);
					int imageListIndex = lvItem.iImage;

					needToSave = true;
					return registerImage(((EventImageTouch*)(actions.at(index)))->item, imageListIndex);
					/*
					if(evImageTouch->item->ipl != NULL)
						cvReleaseImage(&evImageTouch->item->ipl);
					delete evImageTouch->item;


					// 이미지 당기기 지우기
					LVITEM lvItem;
					::ZeroMemory(&lvItem, sizeof(lvItem));
					lvItem.iItem = index;
					lvItem.mask = LVIF_IMAGE;
					listPlaylist.GetItem(&lvItem);
					int imageListIndex = lvItem.iImage;

					for(int i=0; i<listPlaylist.GetItemCount(); i++)
					{
						::ZeroMemory(&lvItem, sizeof(lvItem));
						lvItem.iItem = i;
						lvItem.mask = LVIF_IMAGE;
						listPlaylist.GetItem(&lvItem);
						if(lvItem.iImage >=  imageListIndex)
							lvItem.iImage = lvItem.iImage-1;
						listPlaylist.SetItem(&lvItem);
					}
					
					
					imglist.Remove(index);
					evImageTouch->item = new JhonnyItem(dlgReg.item.getID(), dlgReg.item.getBitmapPath());
					return registerImage(((EventImageTouch*)(actions.at(index)))->item, -1);
					*/
				}

				
				
				

				
			}
			else if(response == IDCANCEL)
			{
				rectDlg->ShowWindow(SW_SHOW);
				ShowWindow(SW_SHOW);
				return -1;
			}



		}
		break;
	case 1:			// 이미지 찾기
		{
			rectDlg->ShowWindow(SW_HIDE);
			ShowWindow(SW_HIDE);
			JhonnyRegister dlgReg;
			dlgReg.setDlgMode(1);		// 조건 추가
			
			EventImageWait* evImageWait = (EventImageWait*)action;
			dlgReg.setEventImageWait(evImageWait);

			INT_PTR response = dlgReg.DoModal();
			if(response == IDOK)
			{
				rectDlg->ShowWindow(SW_SHOW);
				ShowWindow(SW_SHOW);

				int before_findGotoIndex = evImageWait->findGotoIndex;
				int before_nofindGotoIndex = evImageWait->nofindGotoIndex;
				StrCpyW(evImageWait->name, dlgReg.itemName);
				evImageWait->findGotoIndex =  _ttoi(dlgReg.editFindGoto);
				evImageWait->nofindGotoIndex =  _ttoi(dlgReg.editNofindGoto);

				if( before_findGotoIndex != evImageWait->findGotoIndex )
					StrCpyW(evImageWait->findGotoID, _T(""));
				if( before_nofindGotoIndex != evImageWait->nofindGotoIndex )
					StrCpyW(evImageWait->nofindGotoID, _T(""));

				wsprintf(eventItemInfo, _T(" [#%s] 찾기"), evImageWait->name);

				if(dlgReg.imgIndex == -1)		
				{
					if(dlgReg.isIfImageSetted == true)
					{
						if(evImageWait->ifItems != NULL)
						{
							cvReleaseImage(&evImageWait->ifItems->ipl);
							delete evImageWait->ifItems;
						}
						evImageWait->ifItems = new JhonnyItem(dlgReg.ifItem.getID(), dlgReg.ifItem.getBitmapPath());
					}

					if(dlgReg.ifImageDeleted == true)
					{
						if(evImageWait->ifItems != NULL)
						{
							cvReleaseImage(&evImageWait->ifItems->ipl);
							delete evImageWait->ifItems;
						}
						evImageWait->ifItems = NULL;
					}
					needToSave = true;
					return 0;

				}
				else     // 기존 이미지로 변경
				{
					if(dlgReg.isIfImageSetted == true)
					{
						if(evImageWait->ifItems != NULL)
						{
							cvReleaseImage(&evImageWait->ifItems->ipl);
							delete evImageWait->ifItems;
						}
						evImageWait->ifItems = new JhonnyItem(dlgReg.ifItem.getID(), dlgReg.ifItem.getBitmapPath());
					}
					
					if(dlgReg.ifImageDeleted == true)
					{
						if(evImageWait->ifItems != NULL)
						{
							cvReleaseImage(&evImageWait->ifItems->ipl);
							delete evImageWait->ifItems;
						}
						evImageWait->ifItems = NULL;
					}
						


					if(evImageWait->item->ipl != NULL)
						cvReleaseImage(&evImageWait->item->ipl);
					delete evImageWait->item;
					evImageWait->item = new JhonnyItem(dlgReg.item.getID(), dlgReg.item.getBitmapPath());

					LVITEM lvItem;
					::ZeroMemory(&lvItem, sizeof(lvItem));
					lvItem.iItem = index;
					lvItem.mask = LVIF_IMAGE;
					listPlaylist.GetItem(&lvItem);
					int imageListIndex = lvItem.iImage;

					needToSave = true;
					return registerImage(((EventImageTouch*)(actions.at(index)))->item, imageListIndex);
				}
			}
			else if(response == IDCANCEL)
			{
				rectDlg->ShowWindow(SW_SHOW);
				ShowWindow(SW_SHOW);
				return -1;
			}
		}
		break;
	case 2:			// 반복하기
		{
			EventDlgLoop dlgLoop;
			EventLoop* evLoop = (EventLoop*)action;
			dlgLoop.setEventLoop(evLoop);
			INT_PTR response = dlgLoop.DoModal();
			if(response == IDOK)
			{
				


				int gotoIndex =  _ttoi(dlgLoop.editGotoIndex);
				int loopNum =  _ttoi(dlgLoop.editLoopNum);
				if( gotoIndex != evLoop->goToIndex )
					StrCpyW(evLoop->gotoID, _T(""));
				evLoop->goToIndex = gotoIndex;
				evLoop->loopCount = loopNum;
				evLoop->nowLoop = loopNum;
				//actions.push_back(new EventLoop(loopNum, gotoIndex));
				wsprintf(eventItemInfo, _T("%d 이동, %d번 %s"), gotoIndex, loopNum, _T("반복하기"));
			}
			else if(response == IDCANCEL)
			{
				return -1;
			}


		}

		break;
	case 3:			// 기다리기
		{
			EventDlgWait dlgWait;
			EventWait* evWait = (EventWait*)action;
			dlgWait.setEventWait(evWait);
			INT_PTR response = dlgWait.DoModal();
			if(response == IDOK)
			{
				int waitTime =  _ttoi(dlgWait.editWaitTime);
				evWait->millisec = (int)(waitTime*1000);
				wsprintf(eventItemInfo, _T("%d초 %s"), waitTime, _T("기다리기"));
			}
			else if(response == IDCANCEL)
			{
				return -1;
			}

		}
		break;
	case 4:			// 터치하기
		{
			EventDlgTouch dlgTouch;
			EventTouch* evTouch = (EventTouch*)action;
			dlgTouch.setEventTouch(evTouch);
			INT_PTR response = dlgTouch.DoModal();
			if(response == IDOK)
			{
				int coordX =  _ttoi(dlgTouch.editCoordX);
				int coordY =  _ttoi(dlgTouch.editCoordY);
				int coordDragX =  _ttoi(dlgTouch.editDragCoordX);
				int coordDragY =  _ttoi(dlgTouch.editDragCoordY);
				
				BOOL isAbsolute = dlgTouch.checkIsAbsolute;
				BOOL isDrag = dlgTouch.checkIsDrag;
				BOOL isRight = dlgTouch.checkIsRight;
				
				StrCpyW(evTouch->name, dlgTouch.editTouchName);
				evTouch->x = coordX;
				evTouch->y = coordY;
				evTouch->dragX = coordDragX;
				evTouch->dragY = coordDragY;
				evTouch->isAbsolute = isAbsolute;
				evTouch->isDrag = isDrag;
				evTouch->isRightClick = isRight;

				TCHAR coords[64];
				if(isAbsolute)
				{
					if(isDrag)
					{
						wsprintf(coords, _T("(%d,%d)"), coordDragX, coordDragY);	
						wsprintf(eventItemInfo, _T("%s %s"), (StrCmpW(evTouch->name, _T("")) != 0 ? evTouch->name : coords), _T("드래그하기"));
					}
					else
					{
						wsprintf(coords, _T("(%d,%d)"), coordX, coordY);	
						wsprintf(eventItemInfo, _T("%s %s"), (StrCmpW(evTouch->name, _T("")) != 0 ? evTouch->name : coords), _T("터치하기"));
					}
					
				}
				else
				{
					if(isDrag)
					{
						wsprintf(coords, _T("[%s%d,%s%d]"), (coordX >= 0 ? _T("+") : _T("")),  coordX,
									 (coordY >= 0 ? _T("+") : _T("")),  coordY);
						wsprintf(eventItemInfo, _T("%s %s"), (StrCmpW(evTouch->name, _T("")) != 0 ? evTouch->name : coords),  _T("드래그하기"));
					}
					else
					{
						wsprintf(coords, _T("[%s%d,%s%d]"), (coordX >= 0 ? _T("+") : _T("")),  coordX,
									 (coordY >= 0 ? _T("+") : _T("")),  coordY);
						wsprintf(eventItemInfo, _T("%s %s"), (StrCmpW(evTouch->name, _T("")) != 0 ? evTouch->name : coords),  _T("터치하기"));
					}
					

				}
			}
			else if(response == IDCANCEL)
			{
				return -1;
			}
		}
		break;
	case 5:			// 키누르기
		{
			EventDlgPressKey dlgPressKey;
			EventPressKey* evKey = (EventPressKey*)action;
			dlgPressKey.setEventPressKey(evKey);
			INT_PTR response = dlgPressKey.DoModal();
			if(response == IDOK)
			{
				evKey->vk = dlgPressKey.vk;
				evKey->mod = dlgPressKey.mod;
				CString key = dlgPressKey.getHotkeyName(dlgPressKey.vk, dlgPressKey.mod);
				wsprintf(eventItemInfo, _T("%s %s"), key, _T("키누르기"));
			}
			else if(response == IDCANCEL)
			{
				return -1;
			}
		}
		break;
	case 6:			// 구분선
		{
			EventDlgSeparator dlgSeparator;
			EventSeparator* evSeprator = (EventSeparator*)action;
			dlgSeparator.setEventSeparator(evSeprator);
			INT_PTR response = dlgSeparator.DoModal();
			if(response == IDOK)
			{
				StrCpyW(evSeprator->name, dlgSeparator.editSeparatorName);
				wsprintf(eventItemInfo, _T("------%s------"), evSeprator->name);
			}
			else if(response == IDCANCEL)
			{
				return -1;
			}

		}
		break;
	case 7:			// 이동하기
		{
			EventDlgMove dlgGoto;
			EventGoto* evGoto = (EventGoto*)action;
			dlgGoto.setEventGoto(evGoto);
			INT_PTR response = dlgGoto.DoModal();
			if(response == IDOK)
			{
				int gotoIndex =  _ttoi(dlgGoto.editGotoIndex);
				if( gotoIndex != evGoto->goToIndex )
					StrCpyW(evGoto->gotoID, _T(""));
				evGoto->goToIndex = gotoIndex;
				//actions.push_back(new EventLoop(loopNum, gotoIndex));
				wsprintf(eventItemInfo, _T("%d로 %s"), gotoIndex, _T("이동하기"));
			}
			else if(response == IDCANCEL)
			{
				return -1;
			}
		}
		break;
	case 8:			// 돌아가기
		wsprintf(eventItemInfo, _T("%s"), _T("돌아가기"));
		break;
	default:
		break;
	}
	needToSave = true;
	return 0;
}

int JhonnyMain::insertEvent(TCHAR* eventItem, TCHAR* eventItemInfo, int index)
{
	//{"이미지 터치하기", "이미지 찾기", "반복하기", "기다리기", "터치하기", "키누르기"};
	//EventAction ea;
	for(int i=0; i<EVENT_SIZE; i++)
	{
		if(StrCmpW(events[i], eventItem) == 0)
		{
			switch (i)
			{
			case 0:			// 이미지 터치하기
				{
					rectDlg->ShowWindow(SW_HIDE);
					ShowWindow(SW_HIDE);
					JhonnyRegister dlgReg;
					INT_PTR response = dlgReg.DoModal();
					if(response == IDOK)
					{
						rectDlg->ShowWindow(SW_SHOW);
						ShowWindow(SW_SHOW);
						
						TCHAR name[MAX_PATH];
						TCHAR note[MAX_PATH];
						StrCpyW(name, dlgReg.itemName);
						if(dlgReg.isIfImageSetted == true)
						{
							actions.insert(actions.begin()+index, new EventImageTouch(name, 
													new JhonnyItem(dlgReg.item.getID(), dlgReg.item.getBitmapPath()), 
													new JhonnyItem(dlgReg.ifItem.getID(), dlgReg.ifItem.getBitmapPath())));
						}
						else
							actions.insert(actions.begin()+index, new EventImageTouch(name, new JhonnyItem(dlgReg.item.getID(), dlgReg.item.getBitmapPath())));
						wsprintf(eventItemInfo, _T("\"%s\" %s"), name, _T("터치하기"));
						return registerImage(((EventImageTouch*)(actions.at(index)))->item, -1);

					}
					else if(response == IDCANCEL)
					{
						rectDlg->ShowWindow(SW_SHOW);
						ShowWindow(SW_SHOW);
						return -1;
					}

					
					
				}
				break;
			case 1:			// 이미지 찾기
				{
					rectDlg->ShowWindow(SW_HIDE);
					ShowWindow(SW_HIDE);
					JhonnyRegister dlgReg;
					dlgReg.setDlgMode(1);		// 조건 추가
					//dlgReg.setGoto(index+1, index);
					INT_PTR response = dlgReg.DoModal();
				
					

					if(response == IDOK)
					{
						rectDlg->ShowWindow(SW_SHOW);
						ShowWindow(SW_SHOW);

						int findIndex =  _ttoi(dlgReg.editFindGoto);
						int nofindIndex =  _ttoi(dlgReg.editNofindGoto);
						TCHAR name[MAX_PATH];

						StrCpyW(name, dlgReg.itemName);

						
						if(dlgReg.isIfImageSetted == true)
						{
							actions.insert(actions.begin()+index, new EventImageWait(name, 
												new JhonnyItem(dlgReg.item.getID(), dlgReg.item.getBitmapPath()), 
												new JhonnyItem(dlgReg.ifItem.getID(), dlgReg.ifItem.getBitmapPath()),
												findIndex, nofindIndex));
						}
						else
						actions.insert(actions.begin()+index, new EventImageWait(name, new JhonnyItem(dlgReg.item.getID(), dlgReg.item.getBitmapPath()), NULL, findIndex, nofindIndex));
						//actions.push_back();
						wsprintf(eventItemInfo, _T(" [#%s] 찾기"), name);
						return registerImage(((EventImageWait*)(actions.at(index)))->item, -1);
					}
					else if(response == IDCANCEL)
					{
						rectDlg->ShowWindow(SW_SHOW);
						ShowWindow(SW_SHOW);
						return -1;
					}
				}
				break;
			case 2:			// 반복하기
				{
					EventDlgLoop dlgLoop;
					INT_PTR response = dlgLoop.DoModal();
					if(response == IDOK)
					{
						int gotoIndex =  _ttoi(dlgLoop.editGotoIndex);
						int loopNum =  _ttoi(dlgLoop.editLoopNum);
						actions.insert(actions.begin()+index, new EventLoop(loopNum, gotoIndex));
						//actions.push_back(new EventLoop(loopNum, gotoIndex));
						wsprintf(eventItemInfo, _T("%d 이동, %d번 %s"), gotoIndex, loopNum, _T("반복하기"));
					}
					else if(response == IDCANCEL)
					{
						return -1;
					}

					
				}
				
				break;
			case 3:			// 기다리기
				{
					EventDlgWait dlgWait;
					INT_PTR response = dlgWait.DoModal();
					if(response == IDOK)
					{
						int waitTime =  _ttoi(dlgWait.editWaitTime);
						actions.insert(actions.begin()+index, new EventWait(int(waitTime*1000)));
						//actions.push_back(new EventWait(int(waitTime*1000)));
						wsprintf(eventItemInfo, _T("%d초 %s"), waitTime, _T("기다리기"));
					}
					else if(response == IDCANCEL)
					{
						return -1;
					}
					
				}
				break;
			case 4:			// 터치하기
				{
					EventDlgTouch dlgTouch;
					INT_PTR response = dlgTouch.DoModal();
					if(response == IDOK)
					{
						int coordX =  _ttoi(dlgTouch.editCoordX);
						int coordY =  _ttoi(dlgTouch.editCoordY);
						int coordDragX =  _ttoi(dlgTouch.editDragCoordX);
						int coordDragY =  _ttoi(dlgTouch.editDragCoordY);

						BOOL isAbsolute = dlgTouch.checkIsAbsolute;
						BOOL isDrag = dlgTouch.checkIsDrag;
						BOOL isRight = dlgTouch.checkIsRight;
						
						
						actions.insert(actions.begin()+index, new EventTouch((TCHAR*)(LPCTSTR)dlgTouch.editTouchName, coordX, coordY, coordDragX, coordDragY, isAbsolute, isDrag, isRight));
						//actions.push_back(new EventTouch(coordX, coordY, isRight));

						TCHAR coords[64];
						if(isAbsolute)
						{
							wsprintf(coords, _T("(%d,%d)"), coordX, coordY);	
							wsprintf(eventItemInfo, _T("%s %s"), (StrCmpW(dlgTouch.editTouchName, _T("")) != 0 ? dlgTouch.editTouchName : coords), _T("터치하기"));
						}
						else
						{
							wsprintf(coords, _T("[%s%d,%s%d]"), (coordX >= 0 ? _T("+") : _T("")),  coordX,
												(coordY >= 0 ? _T("+") : _T("")),  coordY);
							wsprintf(eventItemInfo, _T("%s %s"), (StrCmpW(dlgTouch.editTouchName, _T("")) != 0 ? dlgTouch.editTouchName : coords),  _T("터치하기"));

						}
						
					}
					else if(response == IDCANCEL)
					{
						return -1;
					}
				}
				break;
			case 5:			// 키누르기
				{
					EventDlgPressKey dlgPressKey;
					INT_PTR response = dlgPressKey.DoModal();
					if(response == IDOK)
					{
						actions.insert(actions.begin()+index, new EventPressKey(dlgPressKey.vk, dlgPressKey.mod));
						//actions.push_back(new EventPressKey(dlgPressKey.vk, dlgPressKey.mod));
						CString key = dlgPressKey.getHotkeyName(dlgPressKey.vk, dlgPressKey.mod);
						wsprintf(eventItemInfo, _T("%s %s"), key, _T("키누르기"));
					}
					else if(response == IDCANCEL)
					{
						return -1;
					}
				}
				break;
			case 6:			// 구분선
				{
					EventDlgSeparator dlgSeparator;
					INT_PTR response = dlgSeparator.DoModal();
					if(response == IDOK)
					{
						actions.insert(actions.begin()+index, new EventSeparator( (TCHAR*)(LPCTSTR)dlgSeparator.editSeparatorName));
						wsprintf(eventItemInfo, _T("------%s------"), dlgSeparator.editSeparatorName);
					}
					else if(response == IDCANCEL)
					{
						return -1;
					}
				}
				break;
			case 7:			// 이동하기
				{
					EventDlgMove dlgMove;
					INT_PTR response = dlgMove.DoModal();
					if(response == IDOK)
					{
						int gotoIndex =  _ttoi(dlgMove.editGotoIndex);
						actions.insert(actions.begin()+index, new EventGoto(gotoIndex));
						//actions.push_back(new EventLoop(loopNum, gotoIndex));
						wsprintf(eventItemInfo, _T("%d로 %s"), gotoIndex, _T("이동하기"));
					}
					else if(response == IDCANCEL)
					{
						return -1;
					}
				}
				break;
			case 8:			// 돌아가기
				{
					actions.insert(actions.begin()+index, new EventReturn(&returnIndexMain));
					wsprintf(eventItemInfo, _T("%s"), _T("돌아가기"));
				}
				break;

			default:
				break;
			}
		}
	}
	needToSave = true;
	return 0;
}




BOOL JhonnyMain::deleteDir(CString dir)
{
    if(dir == _T(""))
    {
        return FALSE;
    }
 
    BOOL bRval = FALSE;
    int nRval = 0;
    CString szNextDirPath = _T("");
    CString szRoot = _T("");
    CFileFind find;
 
    // Directory가 존재 하는지 확인 검사
    bRval = find.FindFile(dir);
 
    if(bRval == FALSE)
    {
        return bRval;
    }
 
    while(bRval)
    {
        bRval = find.FindNextFile();
 
        // . or .. 인 경우 무시한다.
        if(find.IsDots() == TRUE)
        {
            continue;
        }
 
        // Directory 일 경우
        if(find.IsDirectory())
        {
            szNextDirPath.Format(_T("%s\\*.*"), find.GetFilePath());
 
            // Recursion function 호출
            deleteDir(szNextDirPath);
        }
 
        // file일 경우
        else
        {
            //파일 삭제d
            ::DeleteFile(find.GetFilePath());
        }
    }
 
    szRoot = find.GetRoot();
    find.Close();
 
    
    bRval = RemoveDirectory(szRoot);
 
    return bRval;
}

void JhonnyMain::OnNMDblclkListPlaylist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here

	int index = pNMItemActivate->iItem;
	if(index != -1)
	{
		TCHAR itemInfo[MAX_PATH]=_T("");

		LVITEM lvItem;
		TCHAR szBuffer[256];
		::ZeroMemory(&lvItem, sizeof(lvItem));
		::ZeroMemory(szBuffer, sizeof(szBuffer));

		lvItem.mask = LVIF_TEXT | LVIF_IMAGE;
		lvItem.iItem = index;
		lvItem.pszText = szBuffer;
		lvItem.cchTextMax = 256;
		listPlaylist.GetItem(&lvItem);

		int result = modifyEvent(index, itemInfo);
		int index = listPlaylist.GetItemCount();
		CString strIndex;
		strIndex.Format(_T("%d"),index+1);
		//strIndex.Format("%d",999);
		
		if( result == 0)
		{
			lvItem.pszText = itemInfo;
			listPlaylist.SetItem(&lvItem);
			//listPlaylist.SetItemText(index, 0, itemInfo);
			//listPlaylist.InsertItem(index, itemInfo, lvItem.iImage);
			//listPlaylist.SetItemText(index, 1, strIndex);
		}
		else if( result > 0 )
		{
			lvItem.pszText = itemInfo;
			lvItem.iImage = result;
			listPlaylist.SetItem(&lvItem);

			//listPlaylist.InsertItem(index, itemInfo, result);
			//listPlaylist.SetItemText(index, 1, strIndex);
		}
		
		// 링크 재설정
		refreshGoToLink();

		CString tmp;
		tmp.Format(_T("list:%d\n image:%d\n\n"), listPlaylist.GetItemCount(), imglist.GetImageCount() - EVENT_SIZE);
		OutputDebugString(tmp);
			
		/*
		JhonnyRegisterRun ciDlg;
		ciDlg.setRunItem(&runs.at(index));
		//ciDlg.setListItems(&items);
		//ciDlg.setIndex(index);
		CString strTmp;
		strTmp.Format("index : %d\n", index);
		OutputDebugString(strTmp);
		OutputDebugString(runItems.GetItemText(index,0));
				

		INT_PTR response = ciDlg.DoModal();
		if(response == IDOK)
			runItems.SetItemText(index, 0, runs.at(index).getName());
			*/
	
	}


	*pResult = 0;
}


BOOL JhonnyMain::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	BOOL bControl = ((GetKeyState(VK_CONTROL) & 0x8000) != 0); // Control 키가 눌렸는지의 여부 저장

	if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
	{
		if (GetDlgItem(IDC_EDIT_THRESHOLD) == GetFocus())
		{
			GetDlgItem(IDC_EDIT_THRESHOLD)->SendMessage(WM_KILLFOCUS, NULL); 
			return FALSE;
		}
		return TRUE;
	}

	if(editLogbox.m_hWnd == pMsg ->hwnd && pMsg->message == WM_RBUTTONUP) 
	{
		CMenu popUpLogSave;
		popUpLogSave.LoadMenu(IDR_MENU_LOG_SAVE);
		CRect rectBtn;
		editLogbox.GetWindowRect(&rectBtn);
		CMenu* popUpLogSaveSub = popUpLogSave.GetSubMenu(0);
		popUpLogSaveSub->TrackPopupMenu(TPM_RIGHTBUTTON|TPM_LEFTALIGN, pMsg->pt.x, pMsg->pt.y, this);
		return TRUE;
	}
	 if(bControl == true  && isCtrlPress == false)
     {
		 mh_prev_cursor = SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));
		 isCtrlPress = true;
	 }
	 else if(bControl == false  && isCtrlPress == true)
	 {
		 SetCursor(mh_prev_cursor);
		 isCtrlPress = false;
	 }
	 else if(bControl == true  && isCtrlPress == true)
	 {
		 SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));
	 }





	return CDialogEx::PreTranslateMessage(pMsg);
}


void JhonnyMain::OnMenuLoad()
{
	
	CString strLine;


	if(doLoad() == 0)
	{
		CString txt = appSaveFilePath;
		int idx = txt.ReverseFind('\\');
		txt = _T("\"") + txt.Mid(idx+1) + _T("\"") + _T("을 불러왔습니다.") ;
		strLine.Append(txt);
		strLine.Append(_T("\r\n"));
		logData.Append(getLogTime());
		logData.Append(strLine);
	}
	int nLength = editLogbox.GetWindowTextLength();
	if( nLength > 29900 ) 
	{
		editLogbox.SetWindowText(_T(""));
		editLogbox.Clear();
		nLength = editLogbox.GetWindowTextLength();
	}
	editLogbox.SetSel(nLength, nLength);
	editLogbox.ReplaceSel(strLine);
	editLogbox.SetScrollRange(SB_HORZ, 0, 0);
	editLogbox.SetScrollPos(SB_HORZ, 0);

	// 링크 재설정
	refreshGoToLink();

	CString tmp;
	tmp.Format(_T("list:%d\n image:%d\n\n"), listPlaylist.GetItemCount(), imglist.GetImageCount() - EVENT_SIZE);
	OutputDebugString(tmp);
}

void JhonnyMain::OnMenuSave()
{
	// TODO: Add your command handler code here

	CString strLine;
	


	if(doSave(appSaveFilePath) == 0)
	{
		CString txt = appSaveFilePath;
		int idx = txt.ReverseFind('\\');
		txt = _T("\"") + txt.Mid(idx+1) + _T("\"") + _T("을 저장하였습니다.") ;
		strLine.Append(txt);
		strLine.Append(_T("\r\n"));

		logData.Append(getLogTime());
		logData.Append(strLine);
	}
	int nLength = editLogbox.GetWindowTextLength();
	if( nLength > 29900 ) 
	{
		editLogbox.SetWindowText(_T(""));
		editLogbox.Clear();
		nLength = editLogbox.GetWindowTextLength();
	}
	editLogbox.SetSel(nLength, nLength);
	editLogbox.ReplaceSel(strLine);
	editLogbox.SetScrollRange(SB_HORZ, 0, 0);
	editLogbox.SetScrollPos(SB_HORZ, 0);
	
}

void JhonnyMain::OnMenuOtherSave()
{
	// TODO: Add your command handler code here
	CString strLine;


	if(doSave(_T("")) == 0)
	{
		CString txt = appSaveFilePath;
		int idx = txt.ReverseFind('\\');
		txt = _T("\"") + txt.Mid(idx+1) + _T("\"") + _T("을 저장하였습니다.") ;
		strLine.Append(txt);
		strLine.Append(_T("\r\n"));
		logData.Append(getLogTime());
		logData.Append(strLine);
	}
	int nLength = editLogbox.GetWindowTextLength();
	if( nLength > 29900 ) 
	{
		editLogbox.SetWindowText(_T(""));
		editLogbox.Clear();
		nLength = editLogbox.GetWindowTextLength();
	}
	editLogbox.SetSel(nLength, nLength);
	editLogbox.ReplaceSel(strLine);
	editLogbox.SetScrollRange(SB_HORZ, 0, 0);
	editLogbox.SetScrollPos(SB_HORZ, 0);
	
}


int JhonnyMain::doLoad()
{
	TCHAR loadPath[MAX_PATH];

	TCHAR name_filter[] = _T("Load File (*.jam)|*.jam|");
	CFileDialog ins_dlg(TRUE, _T("jam"), _T("*.jam"), OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, name_filter, NULL);

	if(ins_dlg.DoModal() == IDOK){

		loadingDlg->ShowWindow(SW_SHOW);
		loadingDlg->RedrawWindow();


		CFileFind pFind;
		BOOL bRet; 

		
		while (imglist.GetImageCount()) imglist.Remove(0);
		listPlaylist.DeleteAllItems();
		actions.clear();
		returnIndexMain = 0;

		imglist.Add(AfxGetApp()->LoadIcon(IDI_ICON_IMAGE_TOUCH));
		imglist.Add(AfxGetApp()->LoadIcon(IDI_ICON_IMAGE_WAIT));
		imglist.Add(AfxGetApp()->LoadIcon(IDI_ICON_LOOP));
		imglist.Add(AfxGetApp()->LoadIcon(IDI_ICON_WAIT));
		imglist.Add(AfxGetApp()->LoadIcon(IDI_ICON_TOUCH));
		imglist.Add(AfxGetApp()->LoadIcon(IDI_ICON_PRESS_KEY));
		imglist.Add(AfxGetApp()->LoadIcon(IDI_ICON_SEPARATOR));
		imglist.Add(AfxGetApp()->LoadIcon(IDI_ICON_GOTO));
		imglist.Add(AfxGetApp()->LoadIcon(IDI_ICON_RETURN));
		//imglist.Add(AfxGetApp()->LoadIcon(IDI_ICON_DRAG));

		TCHAR filePath[MAX_PATH];
		SetCurrentDirectory(rootPath);
		StrCpyW(filePath, rootPath);
		StrCatW(filePath, _T("\\data"));
		bRet = pFind.FindFile(filePath); // 파일 존재여부 파악
		if(bRet == true)
			deleteDir(filePath);

		StrCpyW(loadPath, ins_dlg.GetPathName());
		
		
		pFind.Close();

		if(uncompressFiles(loadPath) != 0)
		{
			AfxMessageBox(_T("로드하는 동안 문제가 발생하였습니다."));
			return -1;
		}
	}
	else
		return -1;

	//JhonnyRegister* jr = new JhonnyRegister();
	//jr->Create(IDD_DIALOG_REGISTER);
	//jr->ShowWindow(SW_SHOW);
	//jr->RedrawWindow();
	
	std::vector<JhonnyItem*> items;
	if(loadListItems(&items) != 0)
	{
		AfxMessageBox(_T("리스트아이템을 로드하는 동안 문제가 발생하였습니다."));
		loadingDlg->ShowWindow(SW_HIDE);
		return -1;
	}
	
	if(loadRunItems(&items) != 0)
	{
		AfxMessageBox(_T("실행아이템을 로드하는 동안 문제가 발생하였습니다."));
		loadingDlg->ShowWindow(SW_HIDE);
		return -1;
	}

	SetWindowText(ins_dlg.GetFileName());
	StrCpyW(appSaveFilePath, loadPath);
	
	loadingDlg->ShowWindow(SW_HIDE);

	return 0;
}

int JhonnyMain::loadListItems(std::vector<JhonnyItem*>* items)
{
	std::vector<TCHAR*> listItemFiles;
	TCHAR itemPath[MAX_PATH];
	StrCpyW(itemPath, rootPath);
	StrCatW(itemPath, _T("\\"));
	StrCatW(itemPath, listItemDirName);
	if ( getFiles(itemPath, _T("\\*.txt"), &listItemFiles) == 0)
	{
		for(int i=0; i<listItemFiles.size(); i++)
		{
			OutputDebugString(listItemFiles.at(i));
			OutputDebugString(_T("\n"));
			//int fileExist = _access(listItemFiles.at(i), 0);

			basic_ifstream<TCHAR> input(listItemFiles.at(i), std::ios::in); 
			std::locale mylocale("");   // get global locale
			input.imbue(mylocale);
			//input.imbue(std::locale(TCHARToString(countryISO)));
			if (!input)
			{ 
				std::cerr<<"Error reading to ..."<<std::endl; 
				input.close();
				return -1;
			} 
			
			TCHAR id[MAX_PATH];
			TCHAR name[MAX_PATH];
			TCHAR bitmapPath[MAX_PATH];


			int j=0;
			for(  TCHAR line[256]; input.getline(line, 256, '\n'); j++)
			{
				switch (j)
				{
				case 0:
					StrCpyW(id, line);
					break;
				case 1:
					StrCpyW(name, line);
					break;
				default:
					break;
				}
			}
			wsprintf(bitmapPath, _T("%s\\%s.bmp"), itemPath, id);
			items->push_back(new JhonnyItem(id, bitmapPath));


			/*
			HBITMAP hBitSelect = NULL;
			hBitSelect = (HBITMAP)::LoadImage(NULL, bitmapPath, IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR | LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
			if( hBitSelect == NULL )
				return -1;
			
			BITMAP bm = {0};
			GetObject(hBitSelect, sizeof(BITMAP), &bm);

			int cx = ICON_SIZE;
			int cy = ICON_SIZE;


			CPaintDC dc(this);
			//HDC dc;
			HDC memdc = CreateCompatibleDC(dc);
			HDC memdc2 = CreateCompatibleDC(dc);
			HBITMAP hbm = CreateCompatibleBitmap(dc, cx, cy);
			HGDIOBJ hOld = SelectObject(memdc, hbm);
			SelectObject(memdc2, hBitSelect);
			StretchBlt(memdc, 0, 0, cx, cy, memdc2, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

			SelectObject(memdc, hOld);
			DeleteDC(dc);
			DeleteDC(memdc);
			DeleteDC(memdc2);


			CBitmap lamp_image ;
			lamp_image.Attach(hbm);


			int result = imglist.Add(&lamp_image, RGB(255, 255, 255));
			if(result == -1)
				return -1;

			// 오브젝트 삭제
			if(hBitSelect != NULL) ::DeleteObject(hBitSelect);
			*/

			
			input.close();
		}
	}


	/*
	for(int i=0; i<items.size(); i++)
	{
		insertJhonnyItem(&listItems , items.at(i), i);
	}
	*/

	for(int i=0; i<listItemFiles.size(); i++)
		delete[] listItemFiles.at(i);
	return 0;
}

int JhonnyMain::loadRunItems(std::vector<JhonnyItem*>* items)
{
	std::vector<TCHAR*> runItemFiles;
	
	TCHAR itemPath[MAX_PATH];
	StrCpyW(itemPath, rootPath);
	StrCatW(itemPath, _T("\\"));
	StrCatW(itemPath, runItemDirName);
	if ( getFiles(itemPath, _T("\\*.txt"), &runItemFiles) == 0)
	{
		for(int i=0; i<runItemFiles.size(); i++)
		{
			OutputDebugString(runItemFiles.at(i));
			OutputDebugString(_T("\n"));
			//int fileExist = _access(listItemFiles.at(i), 0);

			
			basic_ifstream<TCHAR> input(runItemFiles.at(i), std::ios::in); 
			std::locale mylocale("");   // get global locale
			input.imbue(mylocale);
			//input.imbue(std::locale(TCHARToString(countryISO)));
			if (!input)
			{ 
				std::cerr<<"Error reading to ..."<<std::endl; 
				input.close();
				return -1;
			} 
			
			
			std::vector<CString> data;
			for(  TCHAR line[256]; input.getline(line, 256, '\n');)
			{
				data.push_back(line);
			}
			parsingRunItems(&data, items);

			//data.at(0);
			

			//runs.push_back(JhonnyRunItem(runID, runName, runNote, priority, repeatTime, virtualKeyCode, modifiers, moveX, moveY, leftClick, rightClick, sortNum, item, _ifItems));
			input.close();
		}
	}
	else 
		return -1;
	

	
	std::sort( actions.begin(), actions.end(), compareObj);
	for(int i=0; i<actions.size(); i++)
	{
		int eventID = actions.at(i)->getEventID();
		TCHAR itemInfo[MAX_PATH]=_T("");
		int index = -1;
		if( eventID >= 0 && eventID < 2)
		{
			for(int j=0; j<items->size(); j++)
			{
				if(StrCmpW(items->at(j)->getID(), ((EventImageTouch*)actions.at(i))->item->getID()) == 0)
				{
					index = j;
				}

			}
			if(index == -1)
				return -1;


			// 이미지 리스트에 아이템 이미지 등록
			HBITMAP hBitSelect = NULL;
			hBitSelect = (HBITMAP)::LoadImage(NULL, items->at(index)->getBitmapPath(), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR | LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
			if( hBitSelect == NULL )
				return -1;
			BITMAP bm = {0};
			GetObject(hBitSelect, sizeof(BITMAP), &bm);
			int cx = ICON_SIZE;
			int cy = ICON_SIZE;
			CPaintDC dc(this);
			//HDC dc;
			HDC memdc = CreateCompatibleDC(dc);
			HDC memdc2 = CreateCompatibleDC(dc);
			HBITMAP hbm = CreateCompatibleBitmap(dc, cx, cy);
			HGDIOBJ hOld = SelectObject(memdc, hbm);
			SelectObject(memdc2, hBitSelect);
			StretchBlt(memdc, 0, 0, cx, cy, memdc2, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

			SelectObject(memdc, hOld);
			DeleteDC(dc);
			DeleteDC(memdc);
			DeleteDC(memdc2);


			CBitmap lamp_image ;
			lamp_image.Attach(hbm);


			int result = imglist.Add(&lamp_image, RGB(255, 255, 255));
			if(result == -1)
				return -1;

			// 오브젝트 삭제
			if(hBitSelect != NULL) ::DeleteObject(hBitSelect);


			switch (eventID)
			{
			case 0:			// 이미지 터치하기
				wsprintf(itemInfo, _T("\"%s\" %s"), ((EventImageTouch*)actions.at(i))->name, _T("터치하기"));
				break;
			case 1:			// 이미지 찾기
				wsprintf(itemInfo, _T(" [#%s] 찾기"), ((EventImageWait*)actions.at(i))->name);
				break;
			default:
				break;
			}
			CString strIndex;
			strIndex.Format(_T("%d"), i+1);
			CString temp;
			temp.Format(_T("%d"), imglist.GetImageCount());
			OutputDebugString(temp);
			
			listPlaylist.InsertItem(i, itemInfo, imglist.GetImageCount()-1);
			listPlaylist.SetItemText(i, 1, strIndex);


			

			continue;
		}
		else if(eventID < EVENT_SIZE)
		{
			
			
			switch (eventID)
			{
			case 2:			// 반복하기
				wsprintf(itemInfo, _T("%d 이동, %d번 %s"), ((EventLoop*)actions.at(i))->goToIndex, ((EventLoop*)actions.at(i))->loopCount, _T("반복하기"));
				break;
			case 3:			// 찾기
				wsprintf(itemInfo, _T("%d초 %s"), (int)(((EventWait*)actions.at(i))->millisec / 1000), _T("기다리기"));
				break;
			case 4:			// 터치하기
				{
					TCHAR coords[64];
					int coordX = ((EventTouch*)actions.at(i))->x;
					int coordY = ((EventTouch*)actions.at(i))->y;

					if(((EventTouch*)actions.at(i))->isAbsolute)
					{
						wsprintf(coords, _T("(%d,%d)"), coordX, coordY);
						if(((EventTouch*)actions.at(i))->isDrag)
							wsprintf(itemInfo, _T("%s %s"), (StrCmpW(((EventTouch*)actions.at(i))->name, _T("")) != 0 ? ((EventTouch*)actions.at(i))->name : coords), _T("드래그하기"));
						else
							wsprintf(itemInfo, _T("%s %s"), (StrCmpW(((EventTouch*)actions.at(i))->name, _T("")) != 0 ? ((EventTouch*)actions.at(i))->name : coords), _T("터치하기"));
						
					}
					else
					{
						
						wsprintf(coords, _T("[%s%d,%s%d]"), (coordX >= 0 ? _T("+") : _T("")),  coordX,
										 (coordY >= 0 ? _T("+") : _T("")),  coordY);
						if(((EventTouch*)actions.at(i))->isDrag)
							wsprintf(itemInfo, _T("%s %s"), (StrCmpW(((EventTouch*)actions.at(i))->name, _T("")) != 0 ? ((EventTouch*)actions.at(i))->name : coords),  _T("드래그하기"));
						else
							wsprintf(itemInfo, _T("%s %s"), (StrCmpW(((EventTouch*)actions.at(i))->name, _T("")) != 0 ? ((EventTouch*)actions.at(i))->name : coords),  _T("터치하기"));
						

					}

				}
				break;
			case 5:			// 키누르기
				{
					EventDlgPressKey dlgPressKey;
					wsprintf(itemInfo, _T("%s %s"), dlgPressKey.getHotkeyName(((EventPressKey*)actions.at(i))->vk, ((EventPressKey*)actions.at(i))->mod), _T("키누르기"));
				}
				break;
			case 6:
				wsprintf(itemInfo, _T("------%s------"), ((EventSeparator*)actions.at(i))->name);
				break;
			case 7:
				wsprintf(itemInfo, _T("%d로 %s"),  ((EventGoto*)actions.at(i))->goToIndex, _T("이동하기"));
				break;
			case 8:
				wsprintf(itemInfo, _T("%s"),  _T("돌아가기"));
				break;
			default:
				break;
			}
			CString strIndex;
			strIndex.Format(_T("%d"), i+1);
			listPlaylist.InsertItem(i, itemInfo, actions.at(i)->getEventID());
			//if(actions.at(i)->getEventID() == 6)		// 구분선은 그림 삭제
			//	listPlaylist.InsertItem(i, itemInfo, -1);
			//else
			//	listPlaylist.InsertItem(i, itemInfo, actions.at(i)->getEventID());
			listPlaylist.SetItemText(i, 1, strIndex);
			continue;
		}
	}
	
	


	for(int i=0; i<runItemFiles.size(); i++)
		delete[] runItemFiles.at(i);
	return 0;
}

int JhonnyMain::parsingRunItems(std::vector<CString>* data, std::vector<JhonnyItem*>* items)
{
	// 더미노드 추가, 기존 세이브 파일에서 out of index  방지
	for(int dummy = 0; dummy < 10; dummy++)
		data->push_back(_T(""));


	int eventID =  _ttoi(data->at(1));
	if(eventID < 0 || eventID > EVENT_SIZE)
		return -1;
	switch (eventID)
	{
	case 0:			// 이미지 터치하기
		{
			EventImageTouch* evt = new EventImageTouch();
			evt->sortNum =  _ttoi(data->at(0));
			StrCpyW(evt->id, data->at(2));
			StrCpyW(evt->name, data->at(3));
			evt->item = NULL;
			for(int i=0; i<items->size(); i++)
			{
				if(StrCmpW(items->at(i)->getID(), data->at(4)) == 0)
				{
							
					evt->item = items->at(i);
				}
			}
			if( evt->item == NULL)
				return -1;

			if( data->size() > 5)
			{
				for(int j=0; j<items->size(); j++)
				{
					if(StrCmpW(items->at(j)->getID(), data->at(5)) == 0)
					{
						evt->ifItems = items->at(j);
					}
				}
			}
			
			actions.push_back(evt);
		}
		break;
		
	case 1:			// 이미지 찾기
		{
			EventImageWait* evt = new EventImageWait();
			evt->sortNum =  _ttoi(data->at(0));
			StrCpyW(evt->id, data->at(2));
			StrCpyW(evt->name, data->at(3));
			evt->findGotoIndex =  _ttoi(data->at(4));
			evt->nofindGotoIndex =  _ttoi(data->at(5));
			StrCpyW(evt->findGotoID, _T(""));
			StrCpyW(evt->nofindGotoID, _T(""));
			evt->item = NULL;
			for(int i=0; i<items->size(); i++)
			{
				if(StrCmpW(items->at(i)->getID(), data->at(6)) == 0)
				{
							
					evt->item = items->at(i);
				}
			}
			if( evt->item == NULL)
				return -1;

			if( data->size() > 7)
			{
				for(int j=0; j<items->size(); j++)
				{
					if(StrCmpW(items->at(j)->getID(), data->at(7)) == 0)
					{
						evt->ifItems = items->at(j);
					}
				}
			}
			actions.push_back(evt);
			/*
			EventImageWait* action = (EventImageWait*)actions.at(i);
			output << i << std::endl;
			output << action->eventID << std::endl;
			output << action->id << std::endl;
			output << action->name << std::endl;
			output << action->note << std::endl;
			output << action->findGotoIndex << std::endl;
			output << action->nofindGotoIndex << std::endl;
			output << action->item->getID() << std::endl;
			for(int j=0; j<action->ifItems.size(); j++)
				output << action->ifItems.at(j)->getID() << std::endl;
			*/
		}
		break;
	case 2:			// 반복하기
		{
			EventLoop* evt = new EventLoop();
			evt->sortNum =  _ttoi(data->at(0));
			StrCpyW(evt->id, data->at(2));
			evt->loopCount =  _ttoi(data->at(3));
			evt->goToIndex =  _ttoi(data->at(4));
			StrCpyW(evt->gotoID, _T(""));
			actions.push_back(evt);
			/*
			EventLoop* action = (EventLoop*)actions.at(i);
			output << i << std::endl;
			output << action->eventID << std::endl;
			output << action->id << std::endl;
			output << action->loopCount << std::endl;
			output << action->goToIndex << std::endl;
			*/
		}
		break;
	case 3:			// 기다리기
		{
			EventWait* evt = new EventWait();
			evt->sortNum =  _ttoi(data->at(0));
			StrCpyW(evt->id, data->at(2));
			evt->millisec =   _ttoi(data->at(3)) * 1000;
			actions.push_back(evt);
			/*
			EventWait* action = (EventWait*)actions.at(i);
			output << i << std::endl;
			output << action->eventID << std::endl;
			output << action->id << std::endl;
			output << action->millisec << std::endl;
			*/
		}
		break;
	case 4:			// 터치하기
		{
			EventTouch* evt = new EventTouch();
			evt->sortNum =  _ttoi(data->at(0));
			StrCpyW(evt->id, data->at(2));
			evt->x =   _ttoi(data->at(3));
			evt->y =   _ttoi(data->at(4));
			evt->isAbsolute =   _ttoi(data->at(5));
			evt->isRightClick =   _ttoi(data->at(6));
			StrCpyW(evt->name, data->at(7));
			evt->dragX =   _ttoi(data->at(8));
			evt->dragY =   _ttoi(data->at(9));
			evt->isDrag =   _ttoi(data->at(10));
			actions.push_back(evt);
			/*
			EventTouch* action = (EventTouch*)actions.at(i);
			output << i << std::endl;
			output << action->eventID << std::endl;
			output << action->id << std::endl;
			output << action->x << std::endl;
			output << action->y << std::endl;
			output << action->isRightClick << std::endl;
			*/
		}
		break;
	case 5:			// 키누르기
		{
			EventPressKey* evt = new EventPressKey();
			evt->sortNum =  _ttoi(data->at(0));
			StrCpyW(evt->id, data->at(2));
			evt->vk = (WORD) _ttoi(data->at(3));
			evt->mod = (WORD) _ttoi(data->at(4)); 
			actions.push_back(evt);
			/*
			EventPressKey* action = (EventPressKey*)actions.at(i);
			output << i << std::endl;
			output << action->eventID << std::endl;
			output << action->id << std::endl;
			output << action->vk << std::endl;
			output << action->mod << std::endl;
			*/
		}
		break;
	case 6:			// 구분선
		{
			EventSeparator* evt = new EventSeparator();
			evt->sortNum =  _ttoi(data->at(0));
			StrCpyW(evt->id, data->at(2));
			StrCpyW(evt->name, data->at(3));
			actions.push_back(evt);
		}
		break;
	case 7:			// 이동하기
		{
			EventGoto* evt = new EventGoto();
			evt->sortNum =  _ttoi(data->at(0));
			StrCpyW(evt->id, data->at(2));
			evt->goToIndex =  _ttoi(data->at(3));
			StrCpyW(evt->gotoID, _T(""));
			actions.push_back(evt);
		}
		break;
	case 8:			// 돌아가기
		{
			EventReturn* evt = new EventReturn();
			evt->sortNum =  _ttoi(data->at(0));
			StrCpyW(evt->id, data->at(2));
			evt->returnIndex = &returnIndexMain;
			actions.push_back(evt);
		}
		break;	
	default:
		break;
	}
	return 0;
}

int JhonnyMain::uncompressFiles(TCHAR* loadPath)
{
	HZIP hz = OpenZip(loadPath, 0);  
	ZIPENTRY ze; 
	GetZipItem(hz,-1,&ze);
	int numitems=ze.index;
	for (int i=0; i<numitems; i++)
	{ 
		GetZipItem(hz,i,&ze);               
		TCHAR dest[MAX_PATH];
		wsprintf(dest, _T("%s\\%s"), rootPath,ze.name);     
		TCHAR error[MAX_PATH];
		FormatZipMessageU(UnzipItem(hz,i,dest), error, MAX_PATH);
		//CString a;
		//a.Format(_T("result = %d\n"), result);
		OutputDebugString(error);
		OutputDebugString(_T("\n"));
	}   
	CloseZip(hz);
	return 0;
}

int JhonnyMain::doSave(TCHAR* path)
{
	bool hasError = false;
	CString saveLog = _T("");
	TCHAR savePath[MAX_PATH];
	TCHAR name_filter[] = _T("Save File (*.jam)|*.jam|");
		CFileDialog ins_dlg(FALSE, _T("jam"), _T("*.jam"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT |
													OFN_NOCHANGEDIR, name_filter, NULL);

	if(listPlaylist.GetItemCount() > 0)
	{
		// TODO: Add your command handler code here
		if(StrCmpW(path, _T("")) != 0)
		{
			loadingDlg->ShowWindow(SW_SHOW);
			loadingDlg->RedrawWindow();
			StrCpyW(savePath, path);
		}
		else 
		{
			if(ins_dlg.DoModal() == IDOK){
				loadingDlg->ShowWindow(SW_SHOW);
				loadingDlg->RedrawWindow();
				SetCurrentDirectory(rootPath);
				StrCpyW(savePath, ins_dlg.GetPathName());
				StrCpyW(appSaveFilePath, savePath);
			}
			else
				return -1;
		}
	}
	else
	{
		AfxMessageBox(_T("저장 할 아이템이 없습니다."));
		return -1;
	}
	


	
	if(deleteTempData() != 0)
	{
		loadingDlg->ShowWindow(SW_HIDE);
		saveLog += _T("임시 데이터를 삭제하는 동안 문제가 발생하였습니다.\n");
		hasError = true;
	}

	
	if(saveRunItems(&saveLog) != 0)
	{
		saveLog += _T("실행아이템을 저장하는 동안 문제가 발생하였습니다.\n");
		hasError = true;
	}
	
	if(saveListItems(&saveLog) != 0)
	{
		saveLog += _T("이미지를 저장하는 동안 문제가 발생하였습니다.\n");
		hasError = true;
	}
	

	if(compressFiles(savePath) != 0)
	{
		loadingDlg->ShowWindow(SW_HIDE);
		AfxMessageBox(_T("저장하는 동안 문제가 발생하였습니다."));
		return -1;
	}
	CString tmp = appSaveFilePath;
	int idx = tmp.ReverseFind('\\');
	tmp = tmp.Mid(idx+1);
	SetWindowText(tmp);
	//if(StrCmpW(path, "") == 0)
	needToSave = false;
	loadingDlg->ShowWindow(SW_HIDE);
	if(hasError == false)
		saveLog.Format(_T("\"%s\"이 저장되었습니다."), tmp);
	else
	{
		CString temp;
		temp.Format(_T("\n\n일부 저장이 되지 않은 데이터가 있습니다.\n\"%s\"이 저장되었습니다."), tmp);
		saveLog += temp;
	}
	AfxMessageBox(saveLog);

	return 0;
}

int JhonnyMain::deleteTempData()
{
	// list items
	TCHAR filePath[MAX_PATH]=_T("");
	TCHAR runFilePath[MAX_PATH]=_T("");
	TCHAR listFilePath[MAX_PATH]=_T("");
	TCHAR imageDest[MAX_PATH]=_T("");
	TCHAR infoDest[MAX_PATH]=_T("");
	TCHAR ifImageDest[MAX_PATH]=_T("");
	TCHAR ifInfoDest[MAX_PATH]=_T("");
	CFileFind pFind;
	BOOL bRet; 

	StrCpyW(filePath, rootPath);
	StrCpyW(runFilePath, filePath);
	StrCpyW(listFilePath, filePath);
	StrCatW(listFilePath, _T("\\"));
	StrCatW(listFilePath, listItemDirName);

	

	
	std::vector<TCHAR*> fileList;
	getFiles(listFilePath, _T("\\*.*"), &fileList);
	for(int i=0; i<fileList.size(); i++)
	{
		int j;
		for(j=0; j<actions.size(); j++)
		{
			if (actions.at(j)->getEventID() == 0 || actions.at(j)->getEventID() == 1 )
			{
				EventImageTouch* Event = (EventImageTouch*)actions.at(j);
				StrCpyW(imageDest, listFilePath);
				StrCatW(imageDest, _T("\\"));
				StrCatW(imageDest, Event->item->getID());
				StrCpyW(infoDest, imageDest);
				StrCatW(infoDest, _T(".txt"));
				StrCatW(imageDest, _T(".bmp"));

				if(Event->ifItems != NULL)
				{
					StrCpyW(ifImageDest, listFilePath);
					StrCatW(ifImageDest, _T("\\"));
					StrCatW(ifImageDest, Event->ifItems->getID());
					StrCpyW(ifInfoDest, ifImageDest);
					StrCatW(ifInfoDest, _T(".txt"));
					StrCatW(ifImageDest, _T(".bmp"));
				}

				if(StrCmpW(imageDest, fileList.at(i)) == 0 || StrCmpW(infoDest, fileList.at(i)) == 0 ||  StrCmpW(ifImageDest, fileList.at(i)) == 0 ||  StrCmpW(ifInfoDest, fileList.at(i)) == 0 )
					break;
			}
			
			
		}
		if(j==actions.size())
		{
			bRet = pFind.FindFile(fileList.at(i)); // 파일 존재여부 파악
			if(bRet == true)
				DeleteFile(fileList.at(i)); // 파일삭제 
			else
			{
				pFind.Close();
				return -1;
			}
		}
		
	}


	
	StrCatW(runFilePath, _T("\\"));
	StrCatW(runFilePath, runItemDirName);
	getFiles(runFilePath, _T("\\*.*"), &fileList);
	for(int i=0; i<fileList.size(); i++)
	{
		int j;
		for(j=0; j<actions.size(); j++)
		{
			StrCpyW(infoDest, runFilePath);
			StrCatW(infoDest,_T( "\\"));
			StrCatW(infoDest, actions.at(j)->getID());
			StrCatW(infoDest, _T(".txt"));

			if(StrCmpW(infoDest, fileList.at(i)) == 0 )
				break;
			
		}

		if(j==actions.size())
		{
			bRet = pFind.FindFile(fileList.at(i)); // 파일 존재여부 파악
			if(bRet == true)
				DeleteFile(fileList.at(i)); // 파일삭제 
			else
			{
				pFind.Close();
				return -1;
			}
		}
		
	}
	
	pFind.Close();
	return 0;
}

int JhonnyMain::compressFiles(TCHAR* savePath)
{
	TCHAR filePath[MAX_PATH];
	TCHAR dstZipPath[MAX_PATH];
	TCHAR  szDirPath[MAX_PATH] = _T("");
	TCHAR  listItemDirSearch[MAX_PATH] = _T("");
	TCHAR  runItemDirSearch[MAX_PATH] = _T("");
	HANDLE hSrch;
	WIN32_FIND_DATA stWinFindData;
	BOOL   bResult = TRUE;

	HZIP hz;
	StrCpyW(filePath, rootPath);
	StrCpyW(dstZipPath, filePath);
	StrCatW(dstZipPath, _T("\\data"));
	
	hz = CreateZip ( savePath, 0 );
	ZipAddFolder(hz, listItemDirName);
	ZipAddFolder(hz, runItemDirName);
	
	
	
	
	StrCpyW(listItemDirSearch, listItemDirName);
	StrCatW(listItemDirSearch, _T("\\*.*"));
	hSrch = FindFirstFile ( listItemDirSearch, &stWinFindData );
	if ( hSrch == INVALID_HANDLE_VALUE )
		return FALSE;
	while ( bResult )
	{
		if ( !(stWinFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
		{
			OutputDebugString(stWinFindData.cFileName);
			OutputDebugString(_T("\n"));
			wsprintf ( szDirPath, _T("%s\\%s"), listItemDirName, stWinFindData.cFileName );
			ZipAdd ( hz, szDirPath, szDirPath );
		}
		bResult = FindNextFile ( hSrch, &stWinFindData );
	}
	FindClose(hSrch);

	SetCurrentDirectory(rootPath);
	bResult = TRUE;
	StrCpyW(runItemDirSearch, runItemDirName);
	StrCatW(runItemDirSearch, _T("\\*.*"));
	hSrch = FindFirstFile ( runItemDirSearch, &stWinFindData );
	if ( hSrch == INVALID_HANDLE_VALUE )
		return FALSE;
	while ( bResult )
	{
		if ( !(stWinFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
		{
			OutputDebugString(stWinFindData.cFileName);
			OutputDebugString(_T("\n"));
			wsprintf ( szDirPath, _T("%s\\%s"), runItemDirName, stWinFindData.cFileName );
			ZipAdd ( hz, szDirPath, szDirPath );
		}
		bResult = FindNextFile ( hSrch, &stWinFindData );
	}
	FindClose(hSrch);
	CloseZip ( hz );
	SetCurrentDirectory(rootPath);
	return 0;	
}


int JhonnyMain::saveRunItems(CString* log)
{
	CFileFind pFind;
	BOOL bRet; 
	TCHAR filePath[MAX_PATH];
	TCHAR infoDest[MAX_PATH];
	StrCpyW(filePath, rootPath);
	StrCatW(filePath, _T("\\"));
	StrCatW(filePath, runItemDirName);

	for(int i=0; i<actions.size(); i++)
	{
		StrCpyW(infoDest, filePath);
		StrCatW(infoDest, _T("\\"));
		StrCatW(infoDest, actions.at(i)->getID());
		StrCatW(infoDest, _T(".txt"));


		
	
		bRet = pFind.FindFile(infoDest); // 파일 존재여부 파악
		if(bRet == true)
			DeleteFile(infoDest); // 파일삭제 
		
		//MoveFile(imageSrc,  imageDest);
		//StrCpyW(item.bitmapPath, imageDest);

		/*
		bRet = pFind.FindFile(infoDest); // 파일 존재여부 파악
		if(bRet == true)
			DeleteFile(infoDest); // 파일삭제 
		StrCpyW(item.bitmapPath, infoDest);
			*/

		
		int fileExist = _waccess(infoDest, 0);
		basic_ofstream<TCHAR> output(infoDest,  std::ios::out | std::ios::trunc );  
		//output.imbue(std::locale(TCHARToString(countryISO)));
		std::locale mylocale("");   // get global locale
		output.imbue(mylocale);
		if (!output)
		{ 
			std::cerr<<"Error writing to ..."<<std::endl; 
			pFind.Close();
			output.close();
			return -1;
		} 

		
		CString temp;
		switch (actions.at(i)->getEventID())
		{

			case 0:			// 이미지 터치하기
				{
					EventImageTouch* action = (EventImageTouch*)actions.at(i);
				
					temp.Format(_T("%d"), i);	
					output << temp.GetBuffer() << std::endl;
					output << action->eventID << std::endl;
					output << action->id << std::endl;
					output << action->name << std::endl;
					output << action->item->getID() << std::endl;
					if( action->ifItems->getID() != NULL)
						output << action->ifItems->getID() << std::endl;
				}
				break;
			case 1:			// 이미지 찾기
				{
					EventImageWait* action = (EventImageWait*)actions.at(i);
					temp.Format(_T("%d"), i);	
					output << temp.GetBuffer() << std::endl;
					output << action->eventID << std::endl;
					output << action->id << std::endl;
					output << action->name << std::endl;
					temp.Format(_T("%d"), action->findGotoIndex);
					output << temp.GetBuffer() << std::endl;
					temp.Format(_T("%d"), action->nofindGotoIndex);
					output << temp.GetBuffer() << std::endl;
					output << action->item->getID() << std::endl;
					if( action->ifItems->getID() != NULL)
						output << action->ifItems->getID() << std::endl;
				}
				break;
			case 2:			// 반복하기
				{
					
					EventLoop* action = (EventLoop*)actions.at(i);
					temp.Format(_T("%d"), i);	
					output << temp.GetBuffer() << std::endl;
					output << action->eventID << std::endl;
					output << action->id << std::endl;
					temp.Format(_T("%d"), action->loopCount);
					output << temp.GetBuffer() << std::endl;
					output << action->goToIndex << std::endl;
				}
				break;
			case 3:			// 기다리기
				{
					EventWait* action = (EventWait*)actions.at(i);
					temp.Format(_T("%d"), i);	
					output << temp.GetBuffer() << std::endl;
					output << action->eventID << std::endl;
					output << action->id << std::endl;
					temp.Format(_T("%d"), (int)(action->millisec / 1000.0));
					output << temp.GetBuffer() << std::endl;
				}
				break;
			case 4:			// 터치하기
				{
					EventTouch* action = (EventTouch*)actions.at(i);
					temp.Format(_T("%d"), i);	
					output << temp.GetBuffer() << std::endl;
					output << action->eventID << std::endl;
					output << action->id << std::endl;
					temp.Format(_T("%d"), action->x);
					output << temp.GetBuffer() << std::endl;
					temp.Format(_T("%d"), action->y);
					output << temp.GetBuffer() << std::endl;
					output << action->isAbsolute << std::endl;
					output << action->isRightClick << std::endl;
					output << action->name << std::endl;
					temp.Format(_T("%d"), action->dragX);
					output <<temp.GetBuffer() << std::endl;
					temp.Format(_T("%d"), action->dragY);
					output << temp.GetBuffer() << std::endl;
					output << action->isDrag << std::endl;
				}
				break;
			case 5:			// 키누르기
				{
					EventPressKey* action = (EventPressKey*)actions.at(i);
					temp.Format(_T("%d"), i);	
					output << temp.GetBuffer() << std::endl;
					output << action->eventID << std::endl;
					output << action->id << std::endl;
					output << action->vk << std::endl;
					output << action->mod << std::endl;
				}
			case 6:	
				{
					EventSeparator* action = (EventSeparator*)actions.at(i);
					temp.Format(_T("%d"), i);	
					output << temp.GetBuffer() << std::endl;
					output << action->eventID << std::endl;
					output << action->id << std::endl;
					output << action->name << std::endl;
				}
				break;
			case 7:
				{
					EventGoto* action = (EventGoto*)actions.at(i);
					temp.Format(_T("%d"), i);	
					output << temp.GetBuffer() << std::endl;
					output << action->eventID << std::endl;
					output << action->id << std::endl;
					temp.Format(_T("%d"), action->goToIndex);
					output << temp.GetBuffer() << std::endl;
				}
				break;
			case 8:
				{
					EventReturn* action = (EventReturn*)actions.at(i);
					temp.Format(_T("%d"), i);	
					output << temp.GetBuffer() << std::endl;
					output << action->eventID << std::endl;
					output << action->id << std::endl;
					temp.Format(_T("%d"), action->returnIndex);		//????
					output << temp.GetBuffer() << std::endl;
				}
				break;
			default:
				break;
		}


		output.close();
		pFind.Close();
	}
	return 0;
}

void JhonnyMain::saveTextFile(TCHAR* infoDest, TCHAR* id)
{


	CFileFind pFind;
	BOOL bRet; 
	
	bRet = pFind.FindFile(infoDest); // 파일 존재여부 파악
	if(bRet == true)
	DeleteFile(infoDest); // 파일삭제 
	
	int fileExist = _waccess(infoDest, 0);
	basic_ofstream<TCHAR> output(infoDest,  std::ios::out | std::ios::trunc );  
	//output.imbue(std::locale(((JhonnyMain*)GetParent())->TCHARToString(((JhonnyMain*)GetParent())->countryISO)));
	std::locale mylocale("");   // get global locale
	output.imbue(mylocale);
	if (!output)
	{ 
		std::cerr<<"Error writing to ..."<<std::endl; 
		output.close();
		return ;
	} 
	output << id << endl;
	output.close();
}

int JhonnyMain::saveListItems(CString* log)
{
	int result = 0;
	CFileFind pFind;
	BOOL bRet; 
	TCHAR filePath[MAX_PATH];
	TCHAR infoDest[MAX_PATH];
	TCHAR imageDest[MAX_PATH];
	TCHAR ifImageDest[MAX_PATH];
	StrCpyW(filePath, rootPath);
	StrCatW(filePath, _T("\\"));
	StrCatW(filePath, listItemDirName);

	for(int i=0; i<actions.size(); i++)
	{
		
		if (actions.at(i)->getEventID() == 0 || actions.at(i)->getEventID() == 1 )
		{
			CString indexString;
			indexString.Format(_T("%4d"), i);

			EventImageTouch* Event = (EventImageTouch*)actions.at(i);
			StrCpyW(imageDest, filePath);
			StrCatW(imageDest, _T("\\"));
			StrCatW(imageDest, Event->item->getID());
			StrCatW(imageDest, _T(".bmp"));
			bRet = pFind.FindFile(imageDest); // 파일 존재여부 파악
			if(bRet != true)
			{
				CBitmap bitmap;
				bitmap.LoadBitmap(IDB_BITMAP_NO_IMAGE);  
				CImage image;
				image.Attach(bitmap);
				image.Save(imageDest, Gdiplus::ImageFormatBMP);
				*log += _T("  ") + indexString + _T(", ") + Event->name + _T("\n");
				result = -1;
				//return -1;
			}

			StrCpyW(infoDest, filePath);
			StrCatW(infoDest, _T("\\"));
			StrCatW(infoDest, Event->item->getID());
			StrCatW(infoDest, _T(".txt"));
			bRet = pFind.FindFile(infoDest); // 파일 존재여부 파악
			if(bRet != true)
			{
				saveTextFile(infoDest, Event->item->getID());
				//return -1;
			}

			if(Event->ifItems != NULL)
			{
				StrCpyW(imageDest, filePath);
				StrCatW(imageDest, _T( "\\"));
				StrCatW(imageDest, Event->ifItems->getID());
				StrCatW(imageDest, _T(".bmp"));
				bRet = pFind.FindFile(imageDest); // 파일 존재여부 파악
				if(bRet != true)
				{
					CBitmap bitmap;
					bitmap.LoadBitmap(IDB_BITMAP_NO_IMAGE);  
					CImage image;
					image.Attach(bitmap);
					image.Save(imageDest, Gdiplus::ImageFormatBMP);
					*log += _T("  ") + indexString + _T(", ") + Event->name + _T("(조건)\n");
					result = -1;
					//return -1;
				}

				StrCpyW(infoDest, filePath);
				StrCatW(infoDest, _T("\\"));
				StrCatW(infoDest, Event->ifItems->getID());
				StrCatW(infoDest, _T(".txt"));
				bRet = pFind.FindFile(infoDest); // 파일 존재여부 파악
				if(bRet != true)
				{
					saveTextFile(infoDest, Event->item->getID());
					//return -1;
				}
			}
			
		}

	}
	pFind.Close();
	return result;
}






void JhonnyMain::OnMenuNew()
{
	// TODO: Add your command handler code here
	while (imglist.GetImageCount()) imglist.Remove(0);
	listPlaylist.DeleteAllItems();
	actions.clear();

	imglist.Add(AfxGetApp()->LoadIcon(IDI_ICON_IMAGE_TOUCH));
	imglist.Add(AfxGetApp()->LoadIcon(IDI_ICON_IMAGE_WAIT));
	imglist.Add(AfxGetApp()->LoadIcon(IDI_ICON_LOOP));
	imglist.Add(AfxGetApp()->LoadIcon(IDI_ICON_WAIT));
	imglist.Add(AfxGetApp()->LoadIcon(IDI_ICON_TOUCH));
	imglist.Add(AfxGetApp()->LoadIcon(IDI_ICON_PRESS_KEY));
	imglist.Add(AfxGetApp()->LoadIcon(IDI_ICON_SEPARATOR));
	imglist.Add(AfxGetApp()->LoadIcon(IDI_ICON_GOTO));
	imglist.Add(AfxGetApp()->LoadIcon(IDI_ICON_RETURN));
	//imglist.Add(AfxGetApp()->LoadIcon(IDI_ICON_DRAG));

	pImageItemsDrag = NULL;
	nIndexLeftSel = -1;
	nIndexRightSel = -1;
	returnIndexMain = 0;
	needToSave = false;

	StrCpyW(appSaveFilePath, _T(""));
	SetWindowText(_T("죠니"));
	
}


void JhonnyMain::OnMenuExit()
{
	// TODO: Add your command handler code here
	
	OnOK();
}


void JhonnyMain::OnMenuHelp()
{
	// TODO: Add your command handler code here
	const TCHAR* helpUrl = _T("http://cafe.naver.com/jhonnymacro/26");
	::ShellExecute(NULL, _T("open"), helpUrl, NULL, NULL, SW_SHOW);

}


void JhonnyMain::OnMenuInfo()
{
	// TODO: Add your command handler code here
	JhonnyAbout dlgAbout;
	dlgAbout.DoModal();
}



void JhonnyMain::OnMenuOption()
{
	// TODO: Add your command handler code here
	JhonnyOptionDlg dlgOption;
	dlgOption.setOptDelay(optDelay);
	dlgOption.setOptPlay(optVk, optMod);

	if(dlgOption.DoModal()==IDOK)
	{

		optDelay = dlgOption.modOptDelay;
		optVk = dlgOption.vk;
		optMod = dlgOption.mod;

		UnregisterHotKey(GetSafeHwnd(), hotKeyID);
		if(!::RegisterHotKey(GetSafeHwnd(), hotKeyID, optMod, optVk))   // 추가
			AfxMessageBox(_T("핫키 등록 실패"));                                  // 추가


		saveOption();
	}
}






//BOOL JhonnyMain::DestroyWindow()
//{
//	// TODO: Add your specialized code here and/or call the base class
//	if(needToSave == true)
//		AfxMessageBox("변경 내용을 저장하시겠습니까?", MB_YESNOCANCEL | MB_ICONEXCLAMATION);
//	return CDialogEx::DestroyWindow();
//}


void JhonnyMain::OnClose()
{
	// TODO: Add your message handler code here and/or call default
 
    if(needToSave == true && listPlaylist.GetItemCount() > 0)
	{
		int result = AfxMessageBox(_T("변경 내용을 저장하시겠습니까?"), MB_YESNOCANCEL | MB_ICONEXCLAMATION);
		if( result == IDYES )
			doSave(appSaveFilePath);
		else if( result == IDNO )
			;
		else
			return ;
	}
 
	CDialogEx::OnClose();
}


void JhonnyMain::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: Add your message handler code here and/or call default

	lpMMI->ptMinTrackSize.x = MIN_WINDOW_WIDTH;   // 최소 Width  800
	lpMMI->ptMinTrackSize.y = MIN_WINDOW_HEIGHT;   // 최소 Height 600
	
	lpMMI->ptMaxTrackSize.x = MAX_WINDOW_WIDTH;  // 최대 Width  3000  
	lpMMI->ptMaxTrackSize.y = MAX_WINDOW_HEIGHT;  // 최소 Height 3000  

	CDialogEx::OnGetMinMaxInfo(lpMMI);

	// icon 34
}


void JhonnyMain::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if (listEvents.GetSafeHwnd())  // m_List: 다이얼로그에 그려진 리스트박스의 멤버변수
    {
		
        // m_List의 크기조절
        CRect rect;
		listEvents.GetWindowRect(&rect);
		ScreenToClient(&rect);
		rect.bottom = cy - 120;
	    //GetClientRect(&rect);
		
        //rect.bottom += 5;
        listEvents.MoveWindow(rect);
    } 	

	if (listPlaylist.GetSafeHwnd())  // m_List: 다이얼로그에 그려진 리스트박스의 멤버변수
    {
		
        // m_List의 크기조절
        CRect rect;
		listPlaylist.GetWindowRect(&rect);
		ScreenToClient(&rect);
		rect.bottom = cy - 120;
	    //GetClientRect(&rect);
		
        //rect.bottom += 5;
        listPlaylist.MoveWindow(rect);
    } 	

	if(btnStart.GetSafeHwnd())
	{
		CRect rect;
		btnStart.GetWindowRect(&rect);
		ScreenToClient(&rect);

		rect.top = cy - 105; 
		rect.bottom = cy - 45;
	    //GetClientRect(&rect);
		
        //rect.bottom += 5;
        btnStart.MoveWindow(rect);
	}

	if(editLogbox.GetSafeHwnd())
	{
		CRect rect;
		editLogbox.GetWindowRect(&rect);
		ScreenToClient(&rect);
		rect.top = cy - 105; 
		rect.bottom = cy - 45;
	    //GetClientRect(&rect);
		
        //rect.bottom += 5;
        editLogbox.MoveWindow(rect);
	}

	
	if(sliderThreshold.GetSafeHwnd())
	{
		CRect rect;
		sliderThreshold.GetWindowRect(&rect);
		ScreenToClient(&rect);
		rect.top = cy - 35; 
		rect.bottom = cy - 5;
	    //GetClientRect(&rect);
		
        //rect.bottom += 5;
       sliderThreshold.MoveWindow(rect);
	   sliderThreshold.RedrawWindow();
	}
	

	
	if(GetDlgItem(IDC_EDIT_THRESHOLD)->GetSafeHwnd())
	{
		CRect rect;
		GetDlgItem(IDC_EDIT_THRESHOLD)->GetWindowRect(&rect);
		ScreenToClient(&rect);
		rect.top = cy - 32; 
		rect.bottom = cy - 10;
	    //GetClientRect(&rect);
		
        //rect.bottom += 5;
       GetDlgItem(IDC_EDIT_THRESHOLD)->MoveWindow(rect);
	    GetDlgItem(IDC_EDIT_THRESHOLD)->RedrawWindow();
	}
	

	// TODO: Add your message handler code here
}


void JhonnyMain::OnMenuMacroHome()
{
	const TCHAR* helpUrl = _T("http://cafe.naver.com/jhonnymacro");
	::ShellExecute(NULL, _T("open"), helpUrl, NULL, NULL, SW_SHOW);
	// TODO: Add your command handler code here
}


void JhonnyMain::OnNMReleasedcaptureSliderThreshold(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here


	int nPos = sliderThreshold.GetPos();
	core->setThreshold((float)nPos / 100.0f);
	editThreshold = nPos;
	UpdateData(false);

	*pResult = 0;
}


void JhonnyMain::OnEnKillfocusEditThreshold()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	core->setThreshold((float)editThreshold / 100.0f);
	sliderThreshold.SetPos(editThreshold);
	//UpdateData(false);
}


void JhonnyMain::OnNMCustomdrawListPlaylist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD;  
	pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	*pResult = 0;  

	LPNMLVCUSTOMDRAW  lplvcd = (LPNMLVCUSTOMDRAW)pNMHDR;  
	int iRow = (int)lplvcd->nmcd.dwItemSpec;  

	switch( lplvcd->nmcd.dwDrawStage )  
	{  
	case CDDS_PREPAINT :
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;

		// Modify item text and or background  
	case CDDS_ITEMPREPAINT :
		*pResult = CDRF_NOTIFYSUBITEMDRAW;  
		break;

		// Modify sub item text and/or background  
	case CDDS_SUBITEM | CDDS_PREPAINT | CDDS_ITEM:
		{
		
			//SetBkMode(lplvcd->nmcd.hdc, TRANSPARENT);

			POSITION pos = listPlaylist.GetFirstSelectedItemPosition();
			int nSel = listPlaylist.GetNextSelectedItem(pos);
 
			int row = (int)lplvcd->nmcd.dwItemSpec; 
            if( row == nSel )
            {
               lplvcd->clrText = GetSysColor(COLOR_HIGHLIGHTTEXT);
                lplvcd->clrTextBk = GetSysColor(COLOR_HIGHLIGHT);
				break;
            }	
			
			CString name = listPlaylist.GetItemText(lplvcd->nmcd.dwItemSpec, 0);
			
			if(name.Find(_T("------")) != -1)
			{
				lplvcd->clrTextBk = RGB(45, 45, 46);
				lplvcd->clrText = RGB(255, 255, 255);
			}
			else if(name.Find(_T("@")) != -1)
			{
				lplvcd->clrTextBk = RGB(232,76,61);
				lplvcd->clrText = RGB(255, 255, 255);
			}
			else if(name.Find(_T("#")) != -1)
			{
				lplvcd->clrTextBk = RGB(231,126,35);
				lplvcd->clrText = RGB(255, 255, 255);
			}
			else if(name.Find(_T("$")) != -1)
			{
				lplvcd->clrTextBk = RGB(47,204,113);
				lplvcd->clrText = RGB(255, 255, 255);
			}
			
			
		}
	

		*pResult = CDRF_DODEFAULT;
		break;
	}
}

void JhonnyMain::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
	// TODO: Add your message handler code here and/or call default
	

	if(nHotKeyId == hotKeyID)
    {
		playAndStop();
		
    }

	CDialogEx::OnHotKey(nHotKeyId, nKey1, nKey2);
}



void JhonnyMain::OnMenuLogSave()
{
	// TODO: Add your command handler code here
	TCHAR loadPath[MAX_PATH];

	TCHAR name_filter[] = _T("Save File (*.txt)|*.txt|");
	CFileDialog ins_dlg(FALSE, _T("txt"), _T("*.txt"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT |
													OFN_NOCHANGEDIR, name_filter, NULL);

	if(ins_dlg.DoModal() == IDOK){
		CStdioFile file;

		if(file.Open(ins_dlg.GetPathName(), CFile::modeCreate|CFile::modeWrite))
		{
			file.WriteString(logData);
			MessageBox(_T("저장하였습니다.\n"));
		}
		else
			MessageBox(_T("로그 저장을 실패하였습니다.\n"));
	}
	
}

CString JhonnyMain::getLogTime()
{
	CString val;
	CTime time = CTime::GetCurrentTime();
	val.Format(_T("<%02d:%02d:%02d> "), time.GetHour(), time.GetMinute(), time.GetSecond());
	return val;
}

void JhonnyMain::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: Add your message handler code here and/or call default
	switch (nID)
	{
	case SC_CLOSE:
		/*
		if(isMainWindowMinimized)
		{
			if(pTargetMainWindow != NULL )
			{
				pTargetMainWindow->ShowWindow(SW_RESTORE);
				//pTargetMainWindow->SetWindowPos(NULL, returnTargetWindowRect.left, returnTargetWindowRect.top, 0, 0, SWP_NOSIZE);
			}
		}
		isMainWindowMinimized = false;
		*/
		break;
	case SC_MINIMIZE:
		if(pTargetMainWindow != NULL )
		{
			

			
			// 이벤트가 실행 중 일때만 실행
			//if(listPlaylist.IsWindowEnabled() == false)
			{
				RECT rectPos;
				rectDlg->GetClientRect(&rectPos);
				rectDlg->ClientToScreen(&rectPos);

				RECT returnTargetWindowRect;
				pTargetMainWindow->ShowWindow(SW_RESTORE);
				pTargetMainWindow->GetWindowRect(&returnTargetWindowRect); 

				RECT distance;
				distance.left = rectPos.left - returnTargetWindowRect.left;
				distance.top = rectPos.top - returnTargetWindowRect.top;
				distance.right = rectPos.right - returnTargetWindowRect.right;
				distance.bottom = rectPos.bottom - returnTargetWindowRect.bottom;

				CString temp, temp2;
				temp.Format(_T("%d"), actions.size());
				temp2.Format(_T("%d"), startIndex+1);
				

				viewer = new ViewerDlg();
				viewer->Create(IDD_DIALOG_VIEWER, GetDesktopWindow());
				viewer->setMainWindow(this->GetSafeHwnd());
				viewer->setTargetWindow(pTargetMainWindow->GetSafeHwnd());
				viewer->setNowNum(temp2);
				viewer->setNowTotal(temp);
				viewer->CenterWindow();
				viewer->ShowWindow(SW_SHOW);
				viewer->doDwmCapture(distance);
				MfcUtil::HideTaskbarIcon(this->GetSafeHwnd());
				MfcUtil::HideTaskbarIcon(pTargetMainWindow->GetSafeHwnd());
				pTargetMainWindow->SetWindowPos(NULL, returnTargetWindowRect.left, returnTargetWindowRect.top + screenY, 0, 0, SWP_NOSIZE);
				viewer->setReturnTargetWindow(returnTargetWindowRect);
			}
				
			
			// 이벤트가 실행 중 일때만 실행
			//if(listPlaylist.IsWindowEnabled() == false)
			//	pTargetMainWindow->SetWindowPos(NULL, returnTargetWindowRect.left, returnTargetWindowRect.top + screenY, 0, 0, SWP_NOSIZE);

			
			//rectDlg->GetClientRect(&returnRectWindowRect);
			//rectDlg->ClientToScreen(&returnRectWindowRect);
			//pTargetMainWindow->GetWin
			//rectDlg->SetWindowPos(NULL, returnRectWindowRect.left, returnRectWindowRect.top + screenY, 0, 0, SWP_NOSIZE);
			
		}
		//isMainWindowMinimized = true;
		break;
	case SC_RESTORE:
		/*
		if(isMainWindowMinimized)
		{
			if(pTargetMainWindow != NULL)
			{
				//pTargetMainWindow->ShowWindow(SW_RESTORE);
				

				
				//rectDlg->ShowWindow(SW_RESTORE);
				//rectDlg->SetWindowPos(NULL, returnRectWindowRect.left, returnRectWindowRect.top, 0, 0, SWP_NOSIZE);
				

			}
			
			//AfxMessageBox(_T("re"));
		}
		isMainWindowMinimized = false;
		*/
		break;
	case SC_MAXIMIZE:
		break;
	default:
		break;
	}
	CDialogEx::OnSysCommand(nID, lParam);
}


BOOL Capture(HWND hTargetWnd, LPCTSTR lpszFilePath)
{
    CRect rct;
    if(hTargetWnd)
        ::GetWindowRect(hTargetWnd, &rct);
    else
        return FALSE;

    HBITMAP hBitmap = NULL;
    HBITMAP hOldBitmap = NULL;
    BOOL bSuccess = FALSE;

    HDC hDC = ::GetDC(hTargetWnd);
    HDC hMemDC = ::CreateCompatibleDC(hDC);
    hBitmap = ::CreateCompatibleBitmap(hDC, rct.Width(), rct.Height());

    if(!hBitmap)
        return FALSE;

    hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);

    if(!::PrintWindow(hTargetWnd, hMemDC, PW_CLIENTONLY))
        bSuccess = FALSE;
    else
        bSuccess = TRUE;

	/*
    //Save 함수 : http://blog.daum.net/odega/25
    if(bSuccess && !Save(hBitmap, lpszFilePath))
        bSuccess = FALSE;
    else
        bSuccess = TRUE;
	*/

    SelectObject(hMemDC, hOldBitmap);
    DeleteObject(hBitmap);
    ::DeleteDC(hMemDC);
    ::ReleaseDC(hTargetWnd, hDC);

    return bSuccess;
}



bool compareObj( EventAction* first, EventAction* second )
{
 // vector 내에서 앞에 놓이는 녀석은 뒤에 놓이는 녀석보다 다음과 같은 조건을 만족한다
 return first->getSortNum() < second->getSortNum();
 
 // 만약 vector의 index 0부터 끝까지 getInt()의 값에 따라 오름차순이 되게 하려면
 // return first.getInt() < second.getInt(); 가 되야 할 것이다.
}




int JhonnyMain::HttpGetJhonnyVersionBaaS(TCHAR* noticeInput, TCHAR* versionInput)
 {
	 HINTERNET hInternetRoot = InternetOpen( _T("TestWinINet"),
                                            INTERNET_OPEN_TYPE_PRECONFIG,
                                            NULL,
                                            NULL,
                                            0);
 
    HINTERNET hInternetConnect = InternetConnect(   hInternetRoot,
                                                    _T("api.baas.io"),
                                                    INTERNET_DEFAULT_HTTPS_PORT,
                                                    _T(""),
                                                    _T(""),
                                                    INTERNET_SERVICE_HTTP,
                                                    0,
                                                    0);
 
    HINTERNET hOpenRequest = HttpOpenRequest(   hInternetConnect,
                                                _T("GET"),
                                                _T("/14c45929-47a7-11e4-9866-06f4fe0000b5/1dd8c207-47a8-11e4-9866-06f4fe0000b5/menifests/243b3c47-6dfe-11e4-9e22-06a6fa0000b9/"),
                                                HTTP_VERSION,
                                                _T(""),
                                                NULL,
                                                INTERNET_FLAG_SECURE|INTERNET_FLAG_IGNORE_CERT_CN_INVALID|INTERNET_FLAG_IGNORE_CERT_DATE_INVALID,
                                                0);
 
	
	


    BOOL bSendRequest = HttpSendRequestA(hOpenRequest,
                                        NULL,
                                        0,
                                        NULL, 0);
	char returnData[2048]={0,};
	DWORD dwSize = 0;
    BOOL bRead = InternetReadFile(  hOpenRequest,
                                    returnData,
                                    2048,
                                    &dwSize);

	TCHAR szUniCode[2048]={0,};


	int nLen = MultiByteToWideChar(CP_UTF8, 0, returnData, strlen(returnData), NULL, NULL);
	MultiByteToWideChar(CP_UTF8, 0, returnData, strlen(returnData), szUniCode, 2048);

	CString version = szUniCode;
	CString tokenizerVersion = _T("\"version\" : ");
	int idx = version.Find(tokenizerVersion);
	version = version.Mid(idx + tokenizerVersion.GetLength() + 1);
	idx = version.Find(_T("\""));
	version = version.Left(idx);
	StrCpyW(versionInput, version);
	OutputDebugString(version);
	OutputDebugString(_T("\n"));


	CString notice = szUniCode;
	CString tokenizerNotice = _T("\"notice\" : ");
	idx = notice.Find(tokenizerNotice);
	notice = notice.Mid(idx + tokenizerNotice.GetLength() + 1);
	idx = notice.Find(_T("\""));
	notice = notice.Left(idx);
	notice.Replace( _T("\\n"), _T("\n") );
	StrCpyW(noticeInput, notice);
	OutputDebugString(notice);
	OutputDebugString(_T("\n"));

	


	BOOL bRet = InternetCloseHandle(hOpenRequest);
	bRet = InternetCloseHandle(hInternetConnect);
	bRet = InternetCloseHandle(hInternetRoot);
	/*
	CString clientVersion = GetFileVersion();
	OutputDebugString(clientVersion);
	OutputDebugString(_T("\n"));
	if ( version.Compare(clientVersion) == 0 ) 
	{
		OutputDebugString( _T("최신버전 입니다.\n"));
		return 0;
	}
	else
	{
		OutputDebugString( _T("구버전 입니다.\n"));
		return -1;
	}
	*/

	return 0;
 }





CString JhonnyMain::HttpPostUserTokenBaaS(char* returnData, int length, CString id, CString pass, bool* activated)
 {
	 HINTERNET hInternetRoot = InternetOpen( _T("TestWinINet"),
                                            INTERNET_OPEN_TYPE_PRECONFIG,
                                            NULL,
                                            NULL,
                                            0);
 
    HINTERNET hInternetConnect = InternetConnect(   hInternetRoot,
                                                    _T("api.baas.io"),
                                                    INTERNET_DEFAULT_HTTPS_PORT,
                                                    _T(""),
                                                    _T(""),
                                                    INTERNET_SERVICE_HTTP,
                                                    0,
                                                    0);
 
    HINTERNET hOpenRequest = HttpOpenRequest(   hInternetConnect,
                                                _T("POST"),
                                                _T("/14c45929-47a7-11e4-9866-06f4fe0000b5/1dd8c207-47a8-11e4-9866-06f4fe0000b5/token"),
                                                HTTP_VERSION,
                                                _T(""),
                                                NULL,
                                                INTERNET_FLAG_SECURE|INTERNET_FLAG_IGNORE_CERT_CN_INVALID|INTERNET_FLAG_IGNORE_CERT_DATE_INVALID,
                                                0);
 
	
	
    TCHAR szPostData[2048] = {0};
	wsprintf(szPostData, _T("grant_type=password&username=%s&password=%s"), id, pass);
    //lstrcpy(szPostData, "grant_type=password&username=jhonny&password=jhonny");
 
    // post header
    TCHAR szLen[MAX_PATH] = {0};
    TCHAR szHeader[2048] = {0};


 
    wsprintf(szLen, _T("%d"), lstrlen(szPostData));
    lstrcpy(szHeader, _T("Accept: text/*\r\n"));
    lstrcat(szHeader, _T("User-Agent: Mozilla/4.0 (compatible; MSIE 5.0;* Windows NT)\r\n"));
    lstrcat(szHeader, _T("Content-type: application/x-www-form-urlencoded\r\n"));
    lstrcat(szHeader, _T("Content-length: "));
    lstrcat(szHeader, szLen);
    lstrcat(szHeader, _T("\r\n\n"));
	
 
	int dwLength = _tcslen(szHeader) + 1; 
	char *lpFormData = new char[dwLength]; 
	WideCharToMultiByte(CP_ACP, 0, szHeader, dwLength, lpFormData, dwLength, NULL, NULL);
	HttpAddRequestHeadersA(hOpenRequest, lpFormData, -1L, HTTP_ADDREQ_FLAG_ADD);
	delete [] lpFormData; 


	dwLength = _tcslen(szPostData) + 1; 
	lpFormData = new char[dwLength]; 
	WideCharToMultiByte(CP_ACP, 0, szPostData, dwLength, lpFormData, dwLength, NULL, NULL);
    BOOL bSendRequest = HttpSendRequestA(hOpenRequest,
                                        NULL,
                                        0,
                                        lpFormData,
                                        strlen(lpFormData));
	delete [] lpFormData; 

	char data[2048]={0,};
	DWORD dwSize = 0;
    BOOL bRead = InternetReadFile(  hOpenRequest,
                                    data,
                                    2048,
                                    &dwSize);
	CString token;
	CString access = _T("access_token");
	CString inactived = _T("user not activated");
	token = CString( data );
	
	int idx = token.Find(inactived);
	if(activated != NULL)
	{
		if(idx == -1)
			*activated = true;
		else 
			*activated = false;
	}
	

	idx = token.Find(access);
	if(idx == -1)
		return _T("fail");



	token = token.Mid(idx + access.GetLength()+2);
	OutputDebugString(token);
	OutputDebugString(_T("\n"));
	idx = token.Find(_T("\""));
	token = token.Mid(idx+1);

	idx = token.Find(_T(","));
	token = token.Left(idx-1);
	
	OutputDebugString(token);
	OutputDebugString(_T("\n"));

	BOOL bRet = InternetCloseHandle(hOpenRequest);
	bRet = InternetCloseHandle(hInternetConnect);
	bRet = InternetCloseHandle(hInternetRoot);
	return token;
 }

int JhonnyMain::getFiles(TCHAR* path, TCHAR* extensionType, std::vector<TCHAR*>* fileList)
{
	TCHAR  filePathSearch[MAX_PATH]= _T("");
	HANDLE hSrch;
	WIN32_FIND_DATA stWinFindData;
	BOOL   bResult = TRUE;
	
	StrCpyW(filePathSearch, path);
	StrCatW(filePathSearch, extensionType);
	hSrch = FindFirstFile ( filePathSearch, &stWinFindData );
	if ( hSrch == INVALID_HANDLE_VALUE )
		return -1;
	fileList->clear();
	

	while ( bResult )
	{
		if ( !(stWinFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
		{
			TCHAR* inputPath = new TCHAR[MAX_PATH];
			wsprintf ( inputPath, _T("%s\\%s"), path, stWinFindData.cFileName );
			fileList->push_back(inputPath);
		}
		bResult = FindNextFile ( hSrch, &stWinFindData );
	}
	FindClose(hSrch);
	return 0;
}

std::string JhonnyMain::TCHARToString(const TCHAR* ptsz)
{
     int len = wcslen((wchar_t*)ptsz);
     char* psz = new char[2*len + 1];
     wcstombs(psz, (wchar_t*)ptsz, 2*len + 1);
     std::string s = psz;
     delete [] psz;
     return s;
}

RECT JhonnyMain::getDlgRectRect()
{
	RECT rectDlgRT={0,};
	
	if(IsIconic())
	{
		viewer->GetClientRect(&rectDlgRT);
		viewer->ClientToScreen(&rectDlgRT);
		/*
		rectDlgRT = returnRectWindowRect;
		rectDlgRT.top += screenY;
		rectDlgRT.bottom += screenY;
		*/
	}
	else
	{
		rectDlg->GetClientRect(&rectDlgRT);
		rectDlg->ClientToScreen(&rectDlgRT);
	}
	return rectDlgRT;
}


int JhonnyMain::loadOption()
{
	CString loadOptPath = appEnvironmentPath + _T("jhonnyOpt");
	basic_ifstream<TCHAR> input(loadOptPath, std::ios::in); 
	std::locale mylocale("");   // get global locale
	input.imbue(mylocale);
	//input.imbue(std::locale(TCHARToString(countryISO)));
	if (!input)
	{ 
		optDelay = 1.0;
		optVk = VK_F4;
		optMod = NULL;

		saveOption();
		std::cerr<<"Error reading to ..."<<std::endl; 
		input.close();
		return -1;
	} 
			
			
	std::vector<CString> data;
	for(  TCHAR line[256]; input.getline(line, 256, '\n');)
	{
		data.push_back(line);
	}

	optDelay = _ttof(data.at(0));
	optVk = _ttoi(data.at(1));
	optMod = _ttoi(data.at(2));
	//data.at(0);
			

	//runs.push_back(JhonnyRunItem(runID, runName, runNote, priority, repeatTime, virtualKeyCode, modifiers, moveX, moveY, leftClick, rightClick, sortNum, item, _ifItems));
	input.close();
}

int JhonnyMain::saveOption()
{
	CString saveOptPath = appEnvironmentPath + _T("jhonnyOpt");
	basic_ofstream<TCHAR> output(saveOptPath,  std::ios::out | std::ios::trunc );  
	//output.imbue(std::locale(TCHARToString(countryISO)));
	std::locale mylocale("");   // get global locale
	output.imbue(mylocale);
	if (!output)
	{ 
		std::cerr<<"Error writing to ..."<<std::endl; 
		output.close();
		return -1;
	} 

		
	CString temp;
	temp.Format(_T("%f"), optDelay);	
	output << temp.GetBuffer() << std::endl;
	temp.Format(_T("%d"), optVk);	
	output << temp.GetBuffer() << std::endl;
	temp.Format(_T("%d"), optMod);	
	output << temp.GetBuffer() << std::endl;
	output.close();
}

/*
RECT JhonnyMain::getTargetMainWndGetWndRect()
{
	RECT targetMainWndRT={0,};
	
	if(isMainWindowMinimized == false)
	{
		pTargetMainWindow->GetWindowRect(&targetMainWndRT);
	}
	else
	{
		targetMainWndRT = returnTargetMainWindowRect;
		targetMainWndRT.top += screenY;
		targetMainWndRT.bottom += screenY;
	}
	return targetMainWndRT;

}
*/

