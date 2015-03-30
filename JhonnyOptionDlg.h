#pragma once
#include "afxcmn.h"


// JhonnyOptionDlg dialog

class JhonnyOptionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(JhonnyOptionDlg)

public:
	JhonnyOptionDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~JhonnyOptionDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_OPTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	CString editOptionDelay;
	float modOptDelay;
	//CHotKeyCtrl modOptPlay;
	void setOptDelay(float _modOptDelay) { modOptDelay = _modOptDelay; }
	void setOptPlay(WORD wCode, WORD wModifiers) { vk = wCode;  mod = wModifiers; }

	WORD vk;
	WORD mod;
	
	virtual BOOL OnInitDialog();
	CHotKeyCtrl modOptPlay;
	afx_msg void OnBnClickedOk();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
