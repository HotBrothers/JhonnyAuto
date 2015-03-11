
// JhonnyAutoDlg.cpp : ���� ����
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
// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
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


// CJhonnyAutoDlg ��ȭ ����



CJhonnyAutoDlg::CJhonnyAutoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CJhonnyAutoDlg::IDD, pParent)
	, m_strID(_T(""))
	, editLogin(_T("���̵� (��� ���Ѹ��� �α��� Ŭ��!)"))
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
			OutputDebugString( _T("�ֽŹ��� �Դϴ�.\n"));
			
		}
		else
		{
			OutputDebugString( _T("������ �Դϴ�.\n"));

			CString message = _T("���� ����Ͻô� ������ ������ �Դϴ�.\n�ֽ� ������ �ٿ� �����ðڽ��ϱ�?\n\n\n");

			message += _T("���� ���� : ");
			message += clientVersion;
			message += _T("\n������Ʈ ���� : ");
			message += version;
			message += _T("\n\n[������Ʈ ����]\n");
			message += content;
			/*
			StrCatW(message, _T("Version : "));
			StrCatW(message, version);
			StrCatW(message, _T("\n\n[������Ʈ ����]\n"));
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
		TCHAR message[2048] = _T("���� ����Ͻô� ������ ������ �Դϴ�.\n�ֽ� ������ �ٿ� �����ðڽ��ϱ�?\n\n\n");
		StrCatW(message, _T("Version : "));
		StrCatW(message, version);
		StrCatW(message, _T("\n\n[������Ʈ ����]\n"));
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


// CJhonnyAutoDlg �޽��� ó����



BOOL CJhonnyAutoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
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

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	adware.Create(IDD_ADware, this);
	adware.SetWindowPos(NULL, 10, 370, 230, 60, SWP_SHOWWINDOW | SWP_NOZORDER);

	 LOGFONT log;
    GetFont()->GetLogFont(&log);
    log.lfUnderline = TRUE;
    m_font.CreateFontIndirect(&log);

    // ������� ��Ʈ�� ����ƽ ��Ʈ�ѿ� �����Ѵ�.
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
		
	  m_ToolTip.AddTool( GetDlgItem(IDC_EDIT_LOG_IN), _T("���̵� �Է����ּ���."));
	   m_ToolTip.AddTool( GetDlgItem(IDC_EDIT_PASSWORD), _T("��й�ȣ�� �Է����ּ���."));
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

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
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

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CJhonnyAutoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
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
			// TODO: ���⿡ [Ȯ��]�� Ŭ���Ͽ� ��ȭ ���ڰ� ������ �� ó����
			//  �ڵ带 ��ġ�մϴ�.
		}
		else if (nResponse == IDCANCEL)
		{
			// TODO: ���⿡ [���]�� Ŭ���Ͽ� ��ȭ ���ڰ� ������ �� ó����
			//  �ڵ带 ��ġ�մϴ�.
		}
		else if (nResponse == -1)
		{
			TRACE(traceAppMsg, 0, "���: ��ȭ ���ڸ� ������ �������Ƿ� ���� ���α׷��� ����ġ �ʰ� ����˴ϴ�.\n");
			TRACE(traceAppMsg, 0, "���: ��ȭ ���ڿ��� MFC ��Ʈ���� ����ϴ� ��� #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS�� ������ �� �����ϴ�.\n");
		}
	}
	else
	{
		
		///////////////////////////////////
		// ��Ÿ�׽�Ʈ�Ⱓ ���� ���� 
		dlg.setUserID(_T("betatest"));
		dlg.setUserPass(_T("betatest"));
		AfxGetApp()->m_pMainWnd = &dlg;
		EndDialog(-1);
		INT_PTR nResponse = dlg.DoModal();
		///////////////////////////////////
		
		
		
		if(IDYES == AfxMessageBox(_T("�α��ο� �����Ͽ����ϴ�.\n��� ���Ѹ��� ���� �Ͻðڽ��ϱ�?"),MB_YESNO )) 
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
		AfxMessageBox(_T("Ȱ��ȭ ���� ���� ���̵� �Դϴ�."));
	}
	int com = result.Compare(_T("fail"));
	if(com == 0)
	{

		dlg.setUserID(_T("betatest"));
		dlg.setUserPass(_T("betatest"));
		AfxGetApp()->m_pMainWnd = &dlg;
		EndDialog(-1);
		INT_PTR nResponse = dlg.DoModal();
		
		if(IDYES == AfxMessageBox(_T("�α��ο� �����Ͽ����ϴ�.\n��� ���Ѹ��� ���� �Ͻðڽ��ϱ�?"),MB_YESNO )) 
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
			// TODO: ���⿡ [Ȯ��]�� Ŭ���Ͽ� ��ȭ ���ڰ� ������ �� ó����
			//  �ڵ带 ��ġ�մϴ�.
		}
		else if (nResponse == IDCANCEL)
		{
			// TODO: ���⿡ [���]�� Ŭ���Ͽ� ��ȭ ���ڰ� ������ �� ó����
			//  �ڵ带 ��ġ�մϴ�.
		}
		else if (nResponse == -1)
		{
			TRACE(traceAppMsg, 0, "���: ��ȭ ���ڸ� ������ �������Ƿ� ���� ���α׷��� ����ġ �ʰ� ����˴ϴ�.\n");
			TRACE(traceAppMsg, 0, "���: ��ȭ ���ڿ��� MFC ��Ʈ���� ����ϴ� ��� #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS�� ������ �� �����ϴ�.\n");
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
	MessageBox(_T("����� ��Ÿ�׽�Ʈ �Ⱓ�Դϴ�.\n���̵�:betatest\n��й�ȣ:betatest"));
	/*
	JhonnySignUp dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: ���⿡ [Ȯ��]�� Ŭ���Ͽ� ��ȭ ���ڰ� ������ �� ó����
		//  �ڵ带 ��ġ�մϴ�.
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: ���⿡ [���]�� Ŭ���Ͽ� ��ȭ ���ڰ� ������ �� ó����
		//  �ڵ带 ��ġ�մϴ�.
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "���: ��ȭ ���ڸ� ������ �������Ƿ� ���� ���α׷��� ����ġ �ʰ� ����˴ϴ�.\n");
		TRACE(traceAppMsg, 0, "���: ��ȭ ���ڿ��� MFC ��Ʈ���� ����ϴ� ��� #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS�� ������ �� �����ϴ�.\n");
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