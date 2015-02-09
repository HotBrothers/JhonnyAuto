#pragma once
#include "afxcmn.h"
#include "afxext.h"


// CBitmapButtonTrans

class CBitmapButtonTrans : public CBitmapButton
{
	DECLARE_DYNAMIC(CBitmapButtonTrans)

public:
	CBitmapButtonTrans();
	virtual ~CBitmapButtonTrans();
	virtual VOID SetTransBitmap(IN COLORREF clrTrans) {m_clrTrans = clrTrans;}
	virtual VOID SetHoverBitmapID(IN UINT nIDBitmapResourceHover);

protected:
	COLORREF			m_clrTrans;
	BOOL				m_bMouseHover;
	TRACKMOUSEEVENT		m_stTrackMouse;
	UINT				m_nIDBitmapResourceHover;
	CBitmap				m_cBitmapHover;

protected:
	DECLARE_MESSAGE_MAP()
public:
	VOID DrawBitmapTrans(IN OUT CDC* pDC, IN CBitmap* pcBitmap, IN int x, IN int y, IN COLORREF crMask) ;
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
