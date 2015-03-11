
// JhonnyAutoDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "JhonnyAuto.h"
#include "JhonnyAutoDlg.h"
#include "afxdialogex.h"
#include "JhonnySignUp.h"

#include <fstream>  

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;


CString GetFileVersion();
// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CJhonnyAutoDlg 대화 상자



CJhonnyAutoDlg::CJhonnyAutoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CJhonnyAutoDlg::IDD, pParent)
	, m_strID(_T(""))
	, editLogin(_T("아이디 (기능 제한모드는 로그인 클릭!)"))
	, editPass(_T("1234"))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	isStarted = false;
	// TODO: Add your message handler code here and/or call default
	//TCHAR notice[2048]=_T("");
	//TCHAR version[64] =_T("");
	
	
	const CString tokenizerVersion = L"\"updateVersion\":";
	const CString tokenizerContent = L"\"updateContent\":";
	CString data;
	CString version;
	CString content;

	if(dlg.parse->getData(&data, VERSION) == 0)
	{
		int idx = data.Find(tokenizerVersion);
		version = data.Mid(idx + tokenizerVersion.GetLength() + 1);
		idx = version.Find(_T("\""));
		version = version.Left(idx);
		OutputDebugString(version);
		OutputDebugString(_T("\n"));
		
		idx = data.Find(tokenizerContent);
		content = data.Mid(idx + tokenizerContent.GetLength() + 1);
		idx = content.Find(_T("\""));
		content = content.Left(idx);
		content.Replace(L"\\n", L"\n"); 
		OutputDebugString(content);
		OutputDebugString(_T("\n"));

		CString clientVersion = GetFileVersion();
		OutputDebugString(clientVersion);
		OutputDebugString(_T("\n"));
		if ( version.Compare(clientVersion) == 0 ) 
		{
			OutputDebugString( _T("최신버전 입니다.\n"));
			
		}
		else
		{
			OutputDebugString( _T("구버전 입니다.\n"));

			CString message = _T("현재 사용하시는 버전은 구버전 입니다.\n최신 버전을 다운 받으시겠습니까?\n\n\n");

			message += _T("현재 버전 : ");
			message += clientVersion;
			message += _T("\n업데이트 버전 : ");
			message += version;
			message += _T("\n\n[업데이트 내용]\n");
			message += content;
			/*
			StrCatW(message, _T("Version : "));
			StrCatW(message, version);
			StrCatW(message, _T("\n\n[업데이트 내용]\n"));
			StrCatW(message, notice);
			StrCatW(message, _T("\n\0"));
			*/
			if(IDYES == AfxMessageBox(message, MB_YESNO )) 
			{
				const TCHAR* helpUrl = _T("http://cafe.naver.com/jhonnymacro/75");
				::ShellExecute(NULL, _T("open"), helpUrl, NULL, NULL, SW_SHOW);
				EndDialog(-1);
				return ;
			}
			
		}
	}
	
	

	/*
	if(dlg.HttpGetJhonnyVersionBaaS(notice, version) != 0)
	{
		TCHAR message[2048] = _T("현재 사용하시는 버전은 구버전 입니다.\n최신 버전을 다운 받으시겠습니까?\n\n\n");
		StrCatW(message, _T("Version : "));
		StrCatW(message, version);
		StrCatW(message, _T("\n\n[업데이트 내용]\n"));
		StrCatW(message, notice);
		StrCatW(message, _T("\n\0"));
		
		
		if(IDYES == AfxMessageBox(message, MB_YESNO )) 
		{
			const TCHAR* helpUrl = _T("http://cafe.naver.com/jhonnymacro/75");
			::ShellExecute(NULL, _T("open"), helpUrl, NULL, NULL, SW_SHOW);
			EndDialog(-1);
			return ;
		}
	}
	*/
}

void CJhonnyAutoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strID);
	DDX_Text(pDX, IDC_EDIT_LOG_IN, editLogin);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, editPass);
	DDX_Control(pDX, IDC_EDIT_LOG_IN, editctrlLogin);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, editctrlPas);
}

BEGIN_MESSAGE_MAP(CJhonnyAutoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CJhonnyAutoDlg::OnBnClickedOk)
//	ON_WM_SHOWWINDOW()
//ON_WM_NCHITTEST()
ON_WM_LBUTTONUP()
ON_EN_SETFOCUS(IDC_EDIT_LOG_IN, &CJhonnyAutoDlg::OnEnSetfocusEditLogIn)
ON_EN_SETFOCUS(IDC_EDIT_PASSWORD, &CJhonnyAutoDlg::OnEnSetfocusEditPassword)
ON_STN_CLICKED(IDC_STATIC_SIGNUP, &CJhonnyAutoDlg::OnStnClickedStaticSignup)
END_MESSAGE_MAP()


// CJhonnyAutoDlg 메시지 처리기



BOOL CJhonnyAutoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	adware.Create(IDD_ADware, this);
	adware.SetWindowPos(NULL, 10, 370, 230, 60, SWP_SHOWWINDOW | SWP_NOZORDER);

	 LOGFONT log;
    GetFont()->GetLogFont(&log);
    log.lfUnderline = TRUE;
    m_font.CreateFontIndirect(&log);

    // 만들어진 폰트를 스태틱 컨트롤에 적용한다.
    GetDlgItem(IDC_STATIC_SIGNUP)->SetFont(&m_font);
	
	
	
	
	
	//Create the ToolTip control
	if( !m_ToolTip.Create(this))
	{
	   TRACE0("Unable to create the ToolTip!");
	}
	else
	{
	 // Add tool tips to the controls, either by hard coded string 
	  // or using the string table resource
		
	  m_ToolTip.AddTool( GetDlgItem(IDC_EDIT_LOG_IN), _T("아이디를 입력해주세요."));
	   m_ToolTip.AddTool( GetDlgItem(IDC_EDIT_PASSWORD), _T("비밀번호를 입력해주세요."));
	  //m_ToolTip.AddTool( &m_myEdit, IDS_MY_EDIT);

	  m_ToolTip.Activate(TRUE);
	}
	
	/*
	JhonnyMain dlg;
	dlg.setUserID(_T("betatest"));
	dlg.setUserPass(_T("betatest"));
	//dlg.setUserID(_T(GUEST_MODE_ID));
	//dlg.setUserPass(_T(GUEST_MODE_ID));
	AfxGetApp()->m_pMainWnd = &dlg;
	EndDialog(-1);
	
	INT_PTR nResponse = dlg.DoModal();
	*/

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CJhonnyAutoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CJhonnyAutoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CJhonnyAutoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CJhonnyAutoDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here


	UpdateData();
	
	 
	

	char loginID[256]={0,};
	char loginPass[256]={0,};
	WideCharToMultiByte(CP_ACP, 0,  LPCTSTR(editLogin), 256, loginID, editLogin.GetLength(), NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0,  LPCTSTR(editPass), 256, loginPass, editPass.GetLength(), NULL, NULL);
	

	int result = dlg.parse->signIn(loginID, loginPass);
	if(result == 0)
	{
		dlg.setUserID(editLogin);
		dlg.setUserPass(editPass);
		//dlg.HttpGetBaaS(
		AfxGetApp()->m_pMainWnd = &dlg;
		EndDialog(-1);
		
		INT_PTR nResponse = dlg.DoModal();

		if (nResponse == IDOK)
		{
			// TODO: 여기에 [확인]을 클릭하여 대화 상자가 없어질 때 처리할
			//  코드를 배치합니다.
		}
		else if (nResponse == IDCANCEL)
		{
			// TODO: 여기에 [취소]를 클릭하여 대화 상자가 없어질 때 처리할
			//  코드를 배치합니다.
		}
		else if (nResponse == -1)
		{
			TRACE(traceAppMsg, 0, "경고: 대화 상자를 만들지 못했으므로 응용 프로그램이 예기치 않게 종료됩니다.\n");
			TRACE(traceAppMsg, 0, "경고: 대화 상자에서 MFC 컨트롤을 사용하는 경우 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS를 수행할 수 없습니다.\n");
		}
	}
	else
	{
		
		///////////////////////////////////
		// 베타테스트기간 강제 인증 
		dlg.setUserID(_T("betatest"));
		dlg.setUserPass(_T("betatest"));
		AfxGetApp()->m_pMainWnd = &dlg;
		EndDialog(-1);
		INT_PTR nResponse = dlg.DoModal();
		///////////////////////////////////
		
		
		
		if(IDYES == AfxMessageBox(_T("로그인에 실패하였습니다.\n기능 제한모드로 실행 하시겠습니까?"),MB_YESNO )) 
		{
			dlg.setUserID(_T(GUEST_MODE_ID));
			dlg.setUserPass(_T(GUEST_MODE_ID));
			AfxGetApp()->m_pMainWnd = &dlg;
			EndDialog(-1);
			INT_PTR nResponse = dlg.DoModal();
		}
	}

	/*
	CString result = dlg.HttpPostUserTokenBaaS(szBuf, 512, editLogin, editPass, &isActvated);
	if( isActvated == false)
	{
		AfxMessageBox(_T("활성화 되지 않은 아이디 입니다."));
	}
	int com = result.Compare(_T("fail"));
	if(com == 0)
	{

		dlg.setUserID(_T("betatest"));
		dlg.setUserPass(_T("betatest"));
		AfxGetApp()->m_pMainWnd = &dlg;
		EndDialog(-1);
		INT_PTR nResponse = dlg.DoModal();
		
		if(IDYES == AfxMessageBox(_T("로그인에 실패하였습니다.\n기능 제한모드로 실행 하시겠습니까?"),MB_YESNO )) 
		{
			dlg.setUserID(_T(GUEST_MODE_ID));
			dlg.setUserPass(_T(GUEST_MODE_ID));
			AfxGetApp()->m_pMainWnd = &dlg;
			EndDialog(-1);
			INT_PTR nResponse = dlg.DoModal();
		}
		
	}
	else
	{
		dlg.setUserID(editLogin);
		dlg.setUserPass(editPass);
		//dlg.HttpGetBaaS(
		AfxGetApp()->m_pMainWnd = &dlg;
		EndDialog(-1);
		
		INT_PTR nResponse = dlg.DoModal();

		if (nResponse == IDOK)
		{
			// TODO: 여기에 [확인]을 클릭하여 대화 상자가 없어질 때 처리할
			//  코드를 배치합니다.
		}
		else if (nResponse == IDCANCEL)
		{
			// TODO: 여기에 [취소]를 클릭하여 대화 상자가 없어질 때 처리할
			//  코드를 배치합니다.
		}
		else if (nResponse == -1)
		{
			TRACE(traceAppMsg, 0, "경고: 대화 상자를 만들지 못했으므로 응용 프로그램이 예기치 않게 종료됩니다.\n");
			TRACE(traceAppMsg, 0, "경고: 대화 상자에서 MFC 컨트롤을 사용하는 경우 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS를 수행할 수 없습니다.\n");
		}
	}
	*/
}




//void CJhonnyAutoDlg::OnShowWindow(BOOL bShow, UINT nStatus)
//{
//	CDialogEx::OnShowWindow(bShow, nStatus);
//
//	// TODO: Add your message handler code here
//}


//LRESULT CJhonnyAutoDlg::OnNcHitTest(CPoint point)
//{
//	
//	return CDialogEx::OnNcHitTest(point);
//}


//BOOL CJhonnyAutoDlg::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult)
//{
//	// TODO: Add your specialized code here and/or call the base class
//
//	
//	return CDialogEx::OnChildNotify(message, wParam, lParam, pLResult);
//}


BOOL CJhonnyAutoDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	m_ToolTip.RelayEvent(pMsg);
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CJhonnyAutoDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CDialogEx::OnLButtonUp(nFlags, point);
}


void CJhonnyAutoDlg::OnEnSetfocusEditLogIn()
{
	// TODO: Add your control notification handler code here
	if(isStarted == false)
	{
		editLogin = _T("");
		editPass = _T("");
		UpdateData(false);
		isStarted = true;
	}
	
}


void CJhonnyAutoDlg::OnEnSetfocusEditPassword()
{

	// TODO: Add your control notification handler code here
	if(isStarted == false)
	{
		editLogin = _T("");
		editPass = _T("");
		UpdateData(false);
		isStarted = true;
	}
}


void CJhonnyAutoDlg::OnStnClickedStaticSignup()
{
	MessageBox(_T("현재는 베타테스트 기간입니다.\n아이디:betatest\n비밀번호:betatest"));
	/*
	JhonnySignUp dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 여기에 [확인]을 클릭하여 대화 상자가 없어질 때 처리할
		//  코드를 배치합니다.
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 여기에 [취소]를 클릭하여 대화 상자가 없어질 때 처리할
		//  코드를 배치합니다.
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "경고: 대화 상자를 만들지 못했으므로 응용 프로그램이 예기치 않게 종료됩니다.\n");
		TRACE(traceAppMsg, 0, "경고: 대화 상자에서 MFC 컨트롤을 사용하는 경우 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS를 수행할 수 없습니다.\n");
	}
	*/
}





CString GetFileVersion()
{
    CString strVersion = _T("");
    HRSRC hRsrc = FindResource(NULL, MAKEINTRESOURCE(VS_VERSION_INFO), RT_VERSION);
    if (hRsrc != NULL)
    {
        HGLOBAL hGlobalMemory = LoadResource(NULL, hRsrc);
        if (hGlobalMemory != NULL)
        {
            CString rVersion;
            LPVOID pVersionResouece = LockResource(hGlobalMemory);
            LPVOID pVersion = NULL;
            DWORD uLength,langD;
            BOOL retVal;
            retVal = VerQueryValue(pVersionResouece, _T("\\VarFileInfo\\Translation"), (LPVOID*)&pVersion, (UINT*)&uLength);
            if (retVal && uLength == 4) 
            {
                memcpy(&langD,pVersion,4); 
                rVersion.Format(_T("\\StringFileInfo\\%02X%02X%02X%02X\\FileVersion"),
                    (langD & 0xff00)>>8,langD & 0xff,(langD & 0xff000000)>>24, 
                    (langD & 0xff0000)>>16);
            }
            else
            {
                rVersion.Format(_T("\\StringFileInfo\\%04X04B0\\FileVersion"), GetUserDefaultLangID());
            }
            if( VerQueryValue(pVersionResouece, rVersion.GetBuffer(0), (LPVOID*)&pVersion, (UINT *)&uLength) != 0 )
            {
                strVersion.Format(_T("%s"), pVersion);
            }
        }
        FreeResource(hGlobalMemory);
    }
    return strVersion;
}