#pragma once


// JhonnyAbout dialog

class JhonnyAbout : public CDialogEx
{
	DECLARE_DYNAMIC(JhonnyAbout)

public:
	JhonnyAbout(CWnd* pParent = NULL);   // standard constructor
	virtual ~JhonnyAbout();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
