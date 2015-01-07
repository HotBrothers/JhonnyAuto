#pragma once


// LoadingDlg dialog

class LoadingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(LoadingDlg)

public:
	LoadingDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~LoadingDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_LOADING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
