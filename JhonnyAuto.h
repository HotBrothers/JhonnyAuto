
// JhonnyAuto.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CJhonnyAutoApp:
// �� Ŭ������ ������ ���ؼ��� JhonnyAuto.cpp�� �����Ͻʽÿ�.
//

class CJhonnyAutoApp : public CWinApp
{
public:
	CJhonnyAutoApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CJhonnyAutoApp theApp;