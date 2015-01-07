#pragma once
#include "JhonnyRecpAuth.h"

// JhonnySignUp dialog

class JhonnySignUp : public CDialogEx
{
	DECLARE_DYNAMIC(JhonnySignUp)

public:
	JhonnySignUp(CWnd* pParent = NULL);   // standard constructor
	virtual ~JhonnySignUp();
	JhonnyRecpAuth recpAuth;

// Dialog Data
	enum { IDD = IDD_DIALOG_SIGN_UP };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	CString stringSignID;
	CString stringSignPass;
	CString stringSignPassCon;

	bool checkEmailFormat(CString strMailAddress);
	bool checkPasswordFormat(CString strPassword);

	int HttpGetJhonnyCheckIDBaaS(CString id);
	CString HttpPostUserRegisterBaaS(CString id,  CString email, CString pass);

};
