
// JhonnyAutoDlg.h : 헤더 파일
//

#pragma once
#include "JhonnyAdware.h"
#include "JhonnyMain.h"
#include "afxwin.h"

// CJhonnyAutoDlg 대화 상자
class CJhonnyAutoDlg : public CDialogEx
{
// 생성입니다.
public:
	CJhonnyAutoDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_JHONNYAUTO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.
	JhonnyAdware adware;
	JhonnyMain dlg;

// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CFont m_font;
	//CEdit editPass;
	boolean isStarted;
	CString m_strID;
	afx_msg void OnBnClickedOk();
	CString editLogin;
	CString editPass;
	 CToolTipCtrl m_ToolTip;
//	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
//	afx_msg LRESULT OnNcHitTest(CPoint point);
//	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult);
	 virtual BOOL PreTranslateMessage(MSG* pMsg);
	 afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	 CEdit editctrlLogin;
	 CEdit editctrlPas;
	 afx_msg void OnEnSetfocusEditLogIn();
	 afx_msg void OnEnSetfocusEditPassword();
	 afx_msg void OnStnClickedStaticSignup();
};
