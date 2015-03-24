#pragma once


// JhonnyOptionDlg dialog

class JhonnyOptionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(JhonnyOptionDlg)

public:
	JhonnyOptionDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~JhonnyOptionDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG6 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	CString editOptionDelay;
	float* modOptDelay;

	
	virtual BOOL OnInitDialog();
};
