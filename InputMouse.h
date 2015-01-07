#pragma once


// InputMouse dialog

class InputMouse : public CDialogEx
{
	DECLARE_DYNAMIC(InputMouse)

public:
	InputMouse(CWnd* pParent = NULL);   // standard constructor
	virtual ~InputMouse();

// Dialog Data
	enum { IDD = IDD_DIALOG_INPUT_MOUSE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	CBitmap scBackground;
	HBITMAP hBit;
	JhonnyAutoCore* jCore;
	int x, y;
};
