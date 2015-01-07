// JhonnyRegisterRun.cpp : implementation file
//

#include "stdafx.h"
#include "JhonnyAuto.h"
#include "JhonnyRegisterRun.h"
#include "JhonnyAutoMain.h"
#include "afxdialogex.h"


// JhonnyRegisterRun dialog

IMPLEMENT_DYNAMIC(JhonnyRegisterRun, CDialog)

JhonnyRegisterRun::JhonnyRegisterRun(CWnd* pParent /*=NULL*/)
	: CDialog(JhonnyRegisterRun::IDD, pParent)
	, runName(_T(""))
	, runRepeatTime(0)
	, runPriority(0)
	, actionLeftClick(FALSE)
	, actionRightClick(FALSE)
	, editMoveX(0)
	, editMoveY(0)
{

}

JhonnyRegisterRun::~JhonnyRegisterRun()
{
}

void JhonnyRegisterRun::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_REG_NAME_RUNS, runName);
	DDX_Text(pDX, IDC_EDIT_REG_REPEAT_RUNS, runRepeatTime);
	DDX_Control(pDX, IDC_SLIDER_PRIORITY, sliderPriority);
	DDX_Text(pDX, IDC_STATIC_PRIORITY, runPriority);
	DDX_Control(pDX, IDC_EDIT_REG_NAME_RUNS, editRunsName);
	DDX_Control(pDX, IDC_STATIC_PRIORITY, staticPriority);
	DDX_Control(pDX, IDC_LIST_IFITEM, listIfItems);
	DDX_Control(pDX, IDC_HOTKEY_ACTION, hotkeyAction);
	DDX_Check(pDX, IDC_CHECK_MOUSE_LEFT, actionLeftClick);
	DDX_Check(pDX, IDC_CHECK_MOUSE_RIGHT, actionRightClick);
	DDX_Text(pDX, IDC_EDIT_REG_MOVEX, editMoveX);
	DDX_Text(pDX, IDC_EDIT_REG_MOVEY, editMoveY);
}


BEGIN_MESSAGE_MAP(JhonnyRegisterRun, CDialog)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_PRIORITY, &JhonnyRegisterRun::OnNMReleasedcaptureSliderPriority)
	ON_BN_CLICKED(IDOK, &JhonnyRegisterRun::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &JhonnyRegisterRun::OnBnClickedCancel)
END_MESSAGE_MAP()


// JhonnyRegisterRun message handlers


BOOL JhonnyRegisterRun::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  Add extra initialization here
	

	runName.SetString( run->getName() );
	runRepeatTime = run->getRepeatTime();
	editMoveX = run->getMoveX();
	editMoveY = run->getMoveY();
	runPriority = run->getPriority();
	actionLeftClick = run->getLeftClick();
	actionRightClick = run->getRightClick();
	
	
	sliderPriority.SetRange(1, 10);
	sliderPriority.SetPos(run->getPriority());
	sliderPriority.SetTicFreq(1);

	hotkeyAction.SetHotKey(run->getVirtualKeyCode(), run->getModifiers());
	
	

	hBitSelect = (HBITMAP)::LoadImage(NULL, run->item->getBitmapPath(), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR | LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	CStatic *p_lamp_image = (CStatic *)GetDlgItem(IDC_STATIC_RUNS_IMAGE);
	CBitmap lamp_image;
	lamp_image.Attach(hBitSelect);
	HBITMAP h_old_bitmap = p_lamp_image->SetBitmap(lamp_image);
	if(h_old_bitmap != NULL) ::DeleteObject(h_old_bitmap);
	lamp_image.Detach();
	UpdateData(false);

	/*
	CStatic *p_lamp_image = (CStatic *)GetDlgItem(IDC_STATIC_RUNS_IMAGE);
	p_lamp_image->SetIcon(((JhonnyAutoMain*)GetParent())->imageItems.ExtractIconA(lvItem.iImage));
	*/
	//p_lamp_image->SetBitmap(lvItem.iImage);
	
		
	listIfItems.SetImageList(&((JhonnyAutoMain*)GetParent())->imageItems, LVSIL_NORMAL);
	listIfItems.SetImageList(&((JhonnyAutoMain*)GetParent())->imageItems, LVSIL_SMALL);
	listIfItems.DeleteAllItems();
	//((JhonnyAutoMain*)GetParent())->items
	backupIfItems = run->ifItems;
	for(int i=0; i<run->ifItems.size(); i++)
	{
		int index = -1;
		for(int j=0; j<((JhonnyAutoMain*)GetParent())->items.size(); j++)
		{
			if(strcmp(((JhonnyAutoMain*)GetParent())->items.at(j)->getID(), run->ifItems.at(i)->getID()) == 0)
			{
				index = j;
			}

		}
		if(index == -1)
			return -1;

		LVITEM lvItem;
		TCHAR szBuffer[256];
		::ZeroMemory(&lvItem, sizeof(lvItem));
		::ZeroMemory(szBuffer, sizeof(szBuffer));
		lvItem.mask = LVIF_TEXT | LVIF_IMAGE;
		lvItem.iItem = index;
		lvItem.pszText = szBuffer;
		lvItem.cchTextMax = 256;
		((JhonnyAutoMain*)GetParent())->listItems.GetItem(&lvItem);
		listIfItems.InsertItem(listIfItems.GetItemCount(), lvItem.pszText, lvItem.iImage);

		

	}
	

	//runRepeatTime.Set

	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void JhonnyRegisterRun::OnNMReleasedcaptureSliderPriority(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here

	int pos = sliderPriority.GetPos();
	runPriority = pos;
	char temp[100];
	
	sprintf(temp, "%d", pos);
	staticPriority.SetWindowTextA(temp);
	//UpdateData(false);
	*pResult = 0;
}


void JhonnyRegisterRun::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
	WORD vk;
	WORD mod;
	char temp[100];	

	UpdateData();
	hotkeyAction.GetHotKey(vk, mod);
	
	strcpy(temp, runName.GetString());
	run->setName(temp);
	run->setNote(temp);
	run->setRepeatTime(runRepeatTime);
	run->setPriority(runPriority);
	run->setLeftClick(actionLeftClick);
	run->setRightClick(actionRightClick);
	run->SetMoveX(editMoveX);
	run->SetMoveY(editMoveY);
	run->setVirtualKeyCode(vk);
	run->setModifiers(mod);
	
}


BOOL JhonnyRegisterRun::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
	{
		return TRUE;
	}

	// 사용자가 원하는 키 입력 구문
	if(pMsg->wParam == VK_DELETE)
	{
		//((JhonnyAutoMain*)GetParent())->
		POSITION pos = listIfItems.GetFirstSelectedItemPosition();
		int nItem = listIfItems.GetNextSelectedItem(pos);
		if (nItem != -1)
		{
			listIfItems.DeleteItem(nItem);
			run->ifItems.erase(run->ifItems.begin()+nItem);
		}
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}


void JhonnyRegisterRun::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();

	run->ifItems = backupIfItems;
}
