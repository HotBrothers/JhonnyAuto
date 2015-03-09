
// JhonnyAutoDlg.h : ��� ����
//

#pragma once
#include "JhonnyAdware.h"
#include "JhonnyMain.h"
#include "afxwin.h"

// CJhonnyAutoDlg ��ȭ ����
class CJhonnyAutoDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CJhonnyAutoDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_JHONNYAUTO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.
	JhonnyAdware adware;
	JhonnyMain dlg;

// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
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
