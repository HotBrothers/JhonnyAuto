// JhonnyRegister.cpp : implementation file
//



#include "stdafx.h"

#include <fstream>  
#include <iostream>  
#include <string>  
#include <io.h>


#include "JhonnyAuto.h"
#include "JhonnyMain.h"
#include "JhonnyRegister.h"
#include "CropImageDlg.h"
#include "afxdialogex.h"
#include "JhonnyAutoCore.h"

using namespace std;


// JhonnyRegister dialog

IMPLEMENT_DYNAMIC(JhonnyRegister, CDialog)

JhonnyRegister::JhonnyRegister(CWnd* pParent /*=NULL*/)
	: CDialog(JhonnyRegister::IDD, pParent)
	, itemName(_T(""))
	, editFindGoto(_T(""))
	, editNofindGoto(_T(""))
{
	modImageTouch = NULL; 
	modImageWait = NULL; 
	dlgMode = 0;
	isImageChanged = false;
	imgIndex = -1;
	isIfImageSetted = false;
	ifImageDeleted = false;

	redFont = new CFont(); 
	redFont->CreateFont(12, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET
	, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, _T("MS Shell Dlg")); 
	
}

JhonnyRegister::~JhonnyRegister()
{
	if(redFont != NULL)
		delete redFont;
}

void JhonnyRegister::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_REG_NAME, itemName);
	DDX_Text(pDX, IDC_EDIT_REG_FIND_GOTO, editFindGoto);
	DDX_Text(pDX, IDC_EDIT_REG_NOFIND_GOTO, editNofindGoto);
}


BEGIN_MESSAGE_MAP(JhonnyRegister, CDialog)
	//ON_STN_CLICKED(IDC_STATIC_IF_ITEM, &JhonnyRegister::OnStnClickedStaticIfItem)
	ON_STN_CLICKED(IDC_STATIC_ITEM_IMAGE, &JhonnyRegister::OnStnClickedStaticItemImage)
	ON_BN_CLICKED(IDOK, &JhonnyRegister::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &JhonnyRegister::OnBnClickedCancel)
//	ON_WM_DESTROY()
//ON_NOTIFY(HDN_ITEMCLICK, 0, &JhonnyRegister::OnHdnItemclickListRegItemlist)
//ON_WM_LBUTTONUP()

ON_BN_CLICKED(IDC_BUTTON_IF_DELETE, &JhonnyRegister::OnBnClickedButtonIfDelete)
END_MESSAGE_MAP()




void JhonnyRegister::OnStnClickedStaticItemImage()
{
	// TODO: Add your control notification handler code here
	//ShowWindow(SW_HIDE);
	//SendMessage( WM_SYSCOMMAND, SC_MINIMIZE );
	this->ShowWindow(false);
	Sleep(300);
	CropImageDlg ciDlg;
	ciDlg.setSaveFileName(_T("temp"));
	INT_PTR response = ciDlg.DoModal();
	if(response == IDOK)
	{
		
		

		TCHAR filePath[MAX_PATH];
		StrCpyW(filePath, ((JhonnyMain*)GetParent())->rootPath);
		StrCatW(filePath, _T("\\"));
		StrCatW(filePath, ((JhonnyMain*)GetParent())->listItemDirName);
		StrCatW(filePath, _T("\\temp.bmp"));


		hBitSelect = (HBITMAP)::LoadImage(NULL, filePath, IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR | LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		CStatic *p_lamp_image = (CStatic *)GetDlgItem(IDC_STATIC_ITEM_IMAGE);
		CBitmap lamp_image;
		lamp_image.Attach(hBitSelect);
		HBITMAP h_old_bitmap = p_lamp_image->SetBitmap(lamp_image);
		if(h_old_bitmap != NULL) ::DeleteObject(h_old_bitmap);
		lamp_image.Detach();
		//item.hBitmap = (HBITMAP)::LoadImage(NULL, src, IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR | LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		isImageChanged = true;
		imgIndex = 0;

	}
	this->ShowWindow(true);
	//SendMessage( WM_SYSCOMMAND, SC_RESTORE );
	//ShowWindow(SW_SHOW);
}


void JhonnyRegister::OnBnClickedOk()
{
	JhonnyMain* pPrentWnd = (JhonnyMain*)GetParent();

	
	
	//item.hBitmap = NULL;
	

	TCHAR filePath[MAX_PATH];
	TCHAR imageSrc[MAX_PATH];
	TCHAR imageDest[MAX_PATH];
	TCHAR infoDest[MAX_PATH];
	StrCpyW(filePath, ((JhonnyMain*)GetParent())->rootPath);


	// TODO: Add your control notification handler code here
	bool isModifyMode = false;
	UpdateData();

	if(hBitSelect == NULL )
	{
		AfxMessageBox(_T("이미지가 설정되지 않았습니다."));
		return;
	}
	
	if( StrCmpW(itemName.GetString(), _T("")) == 0)
	{
		AfxMessageBox(_T("이름이 설정되지 않았습니다."));
		return;
	}

	if( isIfImageSetted == true)
	{
		StrCpyW(ifItem.id, ifItem.generateItemID());

		StrCatW(filePath, _T("\\"));

		StrCatW(filePath, ((JhonnyMain*)GetParent())->listItemDirName);

		StrCpyW(imageSrc, filePath);
		StrCatW(imageSrc, _T("\\temp2.bmp"));

		StrCpyW(imageDest, filePath);
		StrCatW(imageDest, _T("\\"));
		StrCatW(imageDest, ifItem.id);
		StrCatW(imageDest, _T(".bmp"));

		StrCpyW(infoDest, filePath);
		StrCatW(infoDest, _T("\\"));
		StrCatW(infoDest, ifItem.id);
		StrCatW(infoDest, _T(".txt"));



		CFileFind pFind;
		BOOL bRet; 


		bRet = pFind.FindFile(imageDest); // 파일 존재여부 파악
		if(bRet == true)
			DeleteFile(imageDest); // 파일삭제 
		MoveFile(imageSrc,  imageDest);
		StrCpyW(ifItem.bitmapPath, imageDest);

		/*
		bRet = pFind.FindFile(infoDest); // 파일 존재여부 파악
		if(bRet == true)
		DeleteFile(infoDest); // 파일삭제 
		StrCpyW(item.bitmapPath, infoDest);
		*/

		int fileExist = _waccess(infoDest, 0);
		basic_ofstream<TCHAR> output(infoDest,  std::ios::out | std::ios::trunc );  
		//output.imbue(std::locale(((JhonnyMain*)GetParent())->TCHARToString(((JhonnyMain*)GetParent())->countryISO)));
		std::locale mylocale("");   // get global locale
		output.imbue(mylocale);
		if (!output)
		{ 
			std::cerr<<"Error writing to ..."<<std::endl; 
			output.close();
			return ;
		} 
		output << ifItem.id << endl;
		output.close();





		JhonnyAutoCore core;
		core.hBitmap2Ipl(&hBitSelect, &ifItem.ipl);
	}

	// 변경이 없는 경우
	if( isImageChanged == false)
	{
		imgIndex = -1;
		CDialog::OnOK();
		return ;
	}

	if( modImageTouch != NULL || modImageWait != NULL)
		isModifyMode = true;

	

	StrCpyW(filePath, ((JhonnyMain*)GetParent())->rootPath);
	StrCpyW(item.id, item.generateItemID());
	
	StrCatW(filePath, _T("\\"));
	
	StrCatW(filePath, ((JhonnyMain*)GetParent())->listItemDirName);
	
	StrCpyW(imageSrc, filePath);
	StrCatW(imageSrc, _T("\\temp.bmp"));

	StrCpyW(imageDest, filePath);
	StrCatW(imageDest, _T("\\"));
	StrCatW(imageDest, item.id);
	StrCatW(imageDest, _T(".bmp"));

	StrCpyW(infoDest, filePath);
	StrCatW(infoDest, _T("\\"));
	StrCatW(infoDest, item.id);
	StrCatW(infoDest, _T(".txt"));

	

	CFileFind pFind;
	BOOL bRet; 
    
	
	bRet = pFind.FindFile(imageDest); // 파일 존재여부 파악
	if(bRet == true)
		DeleteFile(imageDest); // 파일삭제 
	MoveFile(imageSrc,  imageDest);
	StrCpyW(item.bitmapPath, imageDest);

	/*
	bRet = pFind.FindFile(infoDest); // 파일 존재여부 파악
	if(bRet == true)
		DeleteFile(infoDest); // 파일삭제 
	StrCpyW(item.bitmapPath, infoDest);
		*/

	
	int fileExist = _waccess(infoDest, 0);
	basic_ofstream<TCHAR> output(infoDest,  std::ios::out | std::ios::trunc );  
	//output.imbue(std::locale(((JhonnyMain*)GetParent())->TCHARToString(((JhonnyMain*)GetParent())->countryISO)));
	std::locale mylocale("");   // get global locale
	output.imbue(mylocale);
	
	if (!output)
	{ 
		std::cerr<<"Error writing to ..."<<std::endl; 
		output.close();
		return ;
	} 
	output << item.getID() << endl;
	OutputDebugString(item.getID());
	output.close();


	


	JhonnyAutoCore core;
	core.hBitmap2Ipl(&hBitSelect, &item.ipl);
	
	//pPrentWnd->items.push_back(new JhonnyItem(item));

	imgIndex = 0;

	CDialog::OnOK();

	


}


void JhonnyRegister::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here


	if(hBitSelect != NULL) ::DeleteObject(hBitSelect);
	CDialog::OnCancel();
}



BOOL JhonnyRegister::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	if(hBitSelect != NULL) ::DeleteObject(hBitSelect);
	return CDialog::DestroyWindow();
}





BOOL JhonnyRegister::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	isNowLoading = true;
	// TODO:  Add extra initialization here

		//item.cBitmap->Attach(item.hBitmap);
	//imgReglist.DeleteImageList();
	//imgReglist.Create(ICON_SIZE, ICON_SIZE, ILC_COLOR32 | ILC_MASK, 0, 0);


	//GetDlgItem(IDC_STATIC_ITEM_IMAGE)->SetWindowPos(&wndBottom, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);
	// 터치하기
	if(dlgMode == 0)
	{
		GetDlgItem(IDC_EDIT_REG_FIND_GOTO)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_REG_NOFIND_GOTO)->EnableWindow(FALSE);
	}
	else
	{
		
		CString tmp;
		tmp.Format(_T("%d"), ((JhonnyMain*)GetParent())->listPlaylist.GetItemCount()+1);
		editFindGoto.SetString(tmp);
		tmp.Format(_T("%d"), ((JhonnyMain*)GetParent())->listPlaylist.GetItemCount()+1);
		editNofindGoto.SetString(tmp);
		UpdateData(false);
	}
	
	
	//((JhonnyAutoMain*)GetParent())->items



	
	if(modImageTouch != NULL)
	{
		
		// 대표 이미지 변경
		hBitSelect = (HBITMAP)::LoadImage(NULL, modImageTouch->item->getBitmapPath(), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR | LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		CStatic *p_lamp_image = (CStatic *)GetDlgItem(IDC_STATIC_ITEM_IMAGE);
		CBitmap lamp_image;
		lamp_image.Attach(hBitSelect);
		HBITMAP h_old_bitmap = p_lamp_image->SetBitmap(lamp_image);
		if(h_old_bitmap != NULL) ::DeleteObject(h_old_bitmap);
		lamp_image.Detach();

		if(modImageTouch->ifItems != NULL)
		{
			hBitSelect = (HBITMAP)::LoadImage(NULL, modImageTouch->ifItems->getBitmapPath(), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR | LR_LOADFROMFILE | LR_CREATEDIBSECTION);
			CStatic *p_lamp_image = (CStatic *)GetDlgItem(IDC_STATIC_IF_ITEM);
			CBitmap lamp_image;
			lamp_image.Attach(hBitSelect);
			HBITMAP h_old_bitmap = p_lamp_image->SetBitmap(lamp_image);
			if(h_old_bitmap != NULL) ::DeleteObject(h_old_bitmap);
			lamp_image.Detach();
		}

		// 내용 변경
		itemName.SetString(  modImageTouch->name );
		UpdateData(false);
	}
	else if(modImageWait != NULL)
	{
		// 대표 이미지 변경
		hBitSelect = (HBITMAP)::LoadImage(NULL, modImageWait->item->getBitmapPath(), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR | LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		CStatic *p_lamp_image = (CStatic *)GetDlgItem(IDC_STATIC_ITEM_IMAGE);
		CBitmap lamp_image;
		lamp_image.Attach(hBitSelect);
		HBITMAP h_old_bitmap = p_lamp_image->SetBitmap(lamp_image);
		if(h_old_bitmap != NULL) ::DeleteObject(h_old_bitmap);
		lamp_image.Detach();

		if(modImageWait->ifItems != NULL)
		{
			hBitSelect = (HBITMAP)::LoadImage(NULL, modImageWait->ifItems->getBitmapPath(), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR | LR_LOADFROMFILE | LR_CREATEDIBSECTION);
			CStatic *p_lamp_image = (CStatic *)GetDlgItem(IDC_STATIC_IF_ITEM);
			CBitmap lamp_image;
			lamp_image.Attach(hBitSelect);
			HBITMAP h_old_bitmap = p_lamp_image->SetBitmap(lamp_image);
			if(h_old_bitmap != NULL) ::DeleteObject(h_old_bitmap);
			lamp_image.Detach();
		}

		// 내용 변경
		CString tmp;
		itemName.SetString( modImageWait->name );
		tmp.Format(_T("%d"), modImageWait->findGotoIndex);
		editFindGoto.SetString(tmp);
		tmp.Format(_T("%d"), modImageWait->nofindGotoIndex);
		editNofindGoto.SetString(tmp);
		UpdateData(false);
	}
	

	//GetDlgItem(IDC_EDIT_REG_NAME)->EnableWindow(FALSE);
	//GetDlgItem(IDC_EDIT_REG_NOTE)->EnableWindow(FALSE);

	

	((CStatic *)GetDlgItem(IDC_STATIC_IF_NO_FIND))->SetFont(redFont);

	isNowLoading = false;
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}



/*
void JhonnyRegister::OnStnClickedStaticIfItem()
{
	// TODO: Add your control notification handler code here
	AfxMessageBox("azzz");
}	
*/

//void JhonnyRegister::OnLButtonUp(UINT nFlags, CPoint point)
//{
//	// TODO: Add your message handler code here and/or call default
//	
//	CWnd* pWnd = CWnd::WindowFromPoint(point);
//	CString a;
//	pWnd->GetWindowTextA(a);
//	AfxMessageBox(a);
//
//	CDialog::OnLButtonUp(nFlags, point);
//
//}



//BOOL JhonnyRegister::PreCreateWindow(CREATESTRUCT& cs)
//{
//	// TODO: Add your specialized code here and/or call the base class
//
//	return CDialog::PreCreateWindow(cs);
//}


BOOL JhonnyRegister::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class


	
	
	 // 전달된 메시지가 WM_RBUTTONDOWN이고, 전달된 컨트롤의 핸들이 IDC_EDIT라면...
    if(pMsg->message == WM_LBUTTONDOWN )
	{
        // 아무런 처리 없이 그냥 return 한다.
		RECT rect;
		POINT pt;
		GetCursorPos(&pt);
		
		//GetDlgItem(IDC_STATIC_IF_ITEM)->hit
		//this->ScreenToClient(&point);
		GetDlgItem(IDC_STATIC_IF_ITEM)->GetWindowRect(&rect);
		if(rect.left < pt.x && rect.right > pt.x && rect.top < pt.y && rect.bottom > pt.y)
		{
			//ShowWindow(SW_HIDE);
			this->ShowWindow(false);
			//SendMessage( WM_SYSCOMMAND, SC_MINIMIZE );
			Sleep(300);
			CropImageDlg ciDlg;
			ciDlg.setSaveFileName(_T("temp2"));
			INT_PTR response = ciDlg.DoModal();
			if(response == IDOK)
			{
				TCHAR filePath[MAX_PATH];
				StrCpyW(filePath, ((JhonnyMain*)GetParent())->rootPath);
				StrCatW(filePath, _T("\\"));
				StrCatW(filePath, ((JhonnyMain*)GetParent())->listItemDirName);
				StrCatW(filePath, _T("\\temp2.bmp"));


				hBitSelect = (HBITMAP)::LoadImage(NULL, filePath, IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR | LR_LOADFROMFILE | LR_CREATEDIBSECTION);
				CStatic *p_lamp_image = (CStatic *)GetDlgItem(IDC_STATIC_IF_ITEM);
				CBitmap lamp_image;
				lamp_image.Attach(hBitSelect);
				HBITMAP h_old_bitmap = p_lamp_image->SetBitmap(lamp_image);
				if(h_old_bitmap != NULL) ::DeleteObject(h_old_bitmap);
				lamp_image.Detach();
				//item.hBitmap = (HBITMAP)::LoadImage(NULL, src, IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR | LR_LOADFROMFILE | LR_CREATEDIBSECTION);
				isIfImageSetted = true;
				ifImageDeleted = false;

			}
			this->ShowWindow(true);
			//SendMessage( WM_SYSCOMMAND, SC_RESTORE );
			//ShowWindow(SW_SHOW);
			/*
			CString tmp;
			tmp.Format("%d,%d = %d,%d", pt.x, pt.y, rect.left, rect.top);
			AfxMessageBox(tmp);
			*/
			return TRUE;
		}
		
    }
	/*
	else if(pMsg->message == WM_LBUTTONDOWN && pMsg->hwnd == GetDlgItem(IDC_STATIC_ITEM_IMAGE)->m_hWnd){
        // 아무런 처리 없이 그냥 return 한다.
		AfxMessageBox("item");
		return CDialog::PreTranslateMessage(pMsg);
		
        //return TRUE;
    }
	*/



	return CDialog::PreTranslateMessage(pMsg);
}


void JhonnyRegister::OnBnClickedButtonIfDelete()
{
	// TODO: Add your control notification handler code here

	HBITMAP hBitmap=NULL;
	hBitmap = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP_IF_IMAGE));
	//hBitmap=(HBITMAP)LoadImage(theApp.m_hInstance, MAKEINTRESOURCE(IDB_BITMAP_IF_IMAGE)); 
	((CStatic *)GetDlgItem(IDC_STATIC_IF_ITEM))->SetBitmap(hBitmap);
	if(!hBitmap)::CloseHandle(hBitmap);

	isIfImageSetted = false;
	ifImageDeleted = true;
}


