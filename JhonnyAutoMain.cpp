// JhonnyAutoMain.cpp : implementation file
//

#include "stdafx.h"
#include "JhonnyAuto.h"
#include "JhonnyAutoCore.h"
#include "JhonnyAutoMain.h"
#include "JhonnyRegister.h"
#include "JhonnyRegisterRun.h"
#include "JhonnyAbout.h"

#include "afxdialogex.h"
#include "zip.h"
#include "unzip.h"

#include <istream>  
#include <fstream>  
#include <iostream>  
#include <string>  
#include <io.h>



#define ICON_SIZE 24
#define HOTKEY_STOP 860515


void SwapItems(CListCtrl* list, int idxFrom, int idxTo);
BOOL DeleteDir(CString dir);
int getFiles(char* path, char* extensionType, std::vector<char*>* fileList);

std::map<UINT_PTR, JhonnyAutoMain*> JhonnyAutoMain::m_JhonnyAutoMainMap; 

// JhonnyAutoMain dialog
IMPLEMENT_DYNAMIC(JhonnyAutoMain, CDialog)

JhonnyAutoMain::JhonnyAutoMain(CWnd* pParent /*=NULL*/)
	: CDialog(JhonnyAutoMain::IDD, pParent)
	, editLog(_T(""))
	, stringThreshold(_T(""))
	, radioTimeRun(0)
{

}

JhonnyAutoMain::~JhonnyAutoMain()
{

}

void JhonnyAutoMain::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ITEMS, listItems);
	DDX_Control(pDX, IDC_LIST_RUN_ITEMS, runItems);
	DDX_Control(pDX, IDADD, btnAdd);
	DDX_Control(pDX, IDSTART, btnStart);
	DDX_Text(pDX, IDC_EDIT_LOG, editLog);
	DDX_Control(pDX, IDC_SLIDER_THRESHOLD, sliderThreshold);
	DDX_Text(pDX, IDC_STATIC_THRESHOLD, stringThreshold);
	DDX_Control(pDX, IDC_STATIC_THRESHOLD, staticThreshold);
	DDX_Control(pDX, IDC_EDIT_LOG, editLogCtrl);
	DDX_Radio(pDX, IDC_RADIO_RUN1, radioTimeRun);
}


BEGIN_MESSAGE_MAP(JhonnyAutoMain, CDialog)
	ON_BN_CLICKED(IDSTART, &JhonnyAutoMain::OnBnClickedStart)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDADD, &JhonnyAutoMain::OnBnClickedAdd)
//	ON_NOTIFY(LVN_BEGINRDRAG, IDC_LIST_ITEMS, &JhonnyAutoMain::OnLvnBeginrdragListItems)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_NOTIFY(LVN_BEGINDRAG, IDC_LIST_ITEMS, &JhonnyAutoMain::OnLvnBegindragListItems)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_THRESHOLD, &JhonnyAutoMain::OnNMReleasedcaptureSliderThreshold)
//	ON_NOTIFY(HDN_ITEMDBLCLICK, 0, &JhonnyAutoMain::OnHdnItemdblclickListRunItems)
//ON_NOTIFY(NM_DBLCLK, IDC_LIST_ITEMS, &JhonnyAutoMain::OnNMDblclkListItems)
ON_NOTIFY(NM_DBLCLK, IDC_LIST_RUN_ITEMS, &JhonnyAutoMain::OnNMDblclkListRunItems)
ON_COMMAND(ID_MENU_EXIT, &JhonnyAutoMain::OnMenuExit)
ON_COMMAND(ID_MENU_SAVE, &JhonnyAutoMain::OnMenuSave)
ON_COMMAND(ID_MENU_LOAD, &JhonnyAutoMain::OnMenuLoad)
ON_NOTIFY(NM_CLICK, IDC_LIST_ITEMS, &JhonnyAutoMain::OnNMClickListItems)
ON_NOTIFY(NM_CLICK, IDC_LIST_RUN_ITEMS, &JhonnyAutoMain::OnNMClickListRunItems)
ON_COMMAND(ID_MENU_INFO, &JhonnyAutoMain::OnMenuInfo)
ON_COMMAND(ID_MENU_HELP, &JhonnyAutoMain::OnMenuHelp)
ON_COMMAND(ID_MENU_NEW, &JhonnyAutoMain::OnMenuNew)
ON_NOTIFY(LVN_BEGINDRAG, IDC_LIST_RUN_ITEMS, &JhonnyAutoMain::OnLvnBegindragListRunItems)
ON_WM_HOTKEY()
END_MESSAGE_MAP()


// JhonnyAutoMain message handlers


VOID CALLBACK JhonnyAutoMain::runTimerProc(HWND hWnd,UINT uMsg,UINT idEvent,DWORD dwTime)
{
	JhonnyAutoMain *pMyClass= m_JhonnyAutoMainMap[idEvent];
	pMyClass->doStartTimer(idEvent , dwTime);
}

int JhonnyAutoMain::doLoad()
{
	char loadPath[MAX_PATH];

	char name_filter[] = "Load File (*.jam)|*.jam|";
	CFileDialog ins_dlg(TRUE, "jam", "*.jam", OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, name_filter, NULL);

	if(ins_dlg.DoModal() == IDOK){
		CFileFind pFind;
		BOOL bRet; 

		
		while (imageItems.GetImageCount()) imageItems.Remove(0);
		runItems.DeleteAllItems();
		runs.clear();
		listItems.DeleteAllItems();
		items.clear();

		char filePath[MAX_PATH];
		SetCurrentDirectory(rootPath);
		strcpy(filePath, rootPath);
		strcat(filePath, "\\data");
		bRet = pFind.FindFile(filePath); // 파일 존재여부 파악
		if(bRet == true)
			DeleteDir(filePath);

		strcpy(loadPath, ins_dlg.GetPathName());
		pFind.Close();

		if(uncompressFiles(loadPath) != 0)
		{
			AfxMessageBox("로드하는 동안 문제가 발생하였습니다.");
			return -1;
		}
	}
	else
		return -1;

	if(loadListItems() != 0)
	{
		AfxMessageBox("리스트아이템을 로드하는 동안 문제가 발생하였습니다.");
		return -1;
	}
	if(loadRunItems() != 0)
	{
		AfxMessageBox("실행아이템을 로드하는 동안 문제가 발생하였습니다.");
		return -1;
	}
	SetWindowText(ins_dlg.GetFileName());
	return 0;
}

int JhonnyAutoMain::loadListItems()
{
	std::vector<char*> listItemFiles;
	char itemPath[MAX_PATH];
	strcpy(itemPath, rootPath);
	strcat(itemPath, "\\");
	strcat(itemPath, listItemDirName);
	if ( getFiles(itemPath, "\\*.txt", &listItemFiles) == 0)
	{
		for(int i=0; i<listItemFiles.size(); i++)
		{
			OutputDebugString(listItemFiles.at(i));
			OutputDebugString("\n");
			//int fileExist = _access(listItemFiles.at(i), 0);

			std::ifstream input(listItemFiles.at(i), std::ios::in); 
			if (!input)
			{ 
				std::cerr<<"Error reading to ..."<<std::endl; 
				input.close();
				return -1;
			} 
			
			char id[MAX_PATH];
			char name[MAX_PATH];
			char bitmapPath[MAX_PATH];

			int j=0;
			for(  std::string line; getline( input, line ); j++)
			{
				switch (j)
				{
				case 0:
					strcpy(id, line.c_str());
					break;
				case 1:
					strcpy(name, line.c_str());
					break;
				default:
					break;
				}
			}
			sprintf(bitmapPath, "%s\\%s.bmp", itemPath, id);
			items.push_back(new JhonnyItem(id, bitmapPath));

			/*
			while(input>>id>>name>>note)
			{
				
				//JhonnyItem* item = new JhonnyItem(id, name, note, bitmapPath);
				
			}
			*/
			input.close();
		}
	}
	else 
		return -1;

	for(int i=0; i<items.size(); i++)
	{
		insertJhonnyItem(&listItems , items.at(i), i);
	}

	for(int i=0; i<listItemFiles.size(); i++)
		delete[] listItemFiles.at(i);
	return 0;
}

bool compareObj( JhonnyRunItem first, JhonnyRunItem second )
{
 // vector 내에서 앞에 놓이는 녀석은 뒤에 놓이는 녀석보다 다음과 같은 조건을 만족한다
 return first.getSortNum() < second.getSortNum();
 
 // 만약 vector의 index 0부터 끝까지 getInt()의 값에 따라 오름차순이 되게 하려면
 // return first.getInt() < second.getInt(); 가 되야 할 것이다.
}


int JhonnyAutoMain::loadRunItems()
{
	std::vector<char*> runItemFiles;
	char itemPath[MAX_PATH];
	strcpy(itemPath, rootPath);
	strcat(itemPath, "\\");
	strcat(itemPath, runItemDirName);
	if ( getFiles(itemPath, "\\*.txt", &runItemFiles) == 0)
	{
		for(int i=0; i<runItemFiles.size(); i++)
		{
			OutputDebugString(runItemFiles.at(i));
			OutputDebugString("\n");
			//int fileExist = _access(listItemFiles.at(i), 0);

			std::ifstream input(runItemFiles.at(i), std::ios::in); 
			if (!input)
			{ 
				std::cerr<<"Error reading to ..."<<std::endl; 
				input.close();
				return -1;
			} 
			

			char runID[MAX_PATH];
			char runName[MAX_PATH];
			//char priority[MAX_PATH];
			//char repeatTime[MAX_PATH];
			int priority;
			float repeatTime;
			int virtualKeyCode;
			int modifiers;
			int moveX;
			int moveY;
			int leftClick;
			int rightClick;
			int sortNum;
			char itemID[MAX_PATH];
			char ifItemID[MAX_PATH];
			std::vector<JhonnyItem*> _ifItems;
			JhonnyItem* item = NULL;

			int j=0;
			for(  std::string line; getline( input, line ); j++)
			{
				switch (j)
				{
				case 0:
					strcpy(runID, line.c_str());
					break;
				case 1:
					strcpy(runName, line.c_str());
					break;
				case 3:
					priority = atoi(line.c_str());
					break;
				case 4:
					repeatTime = atof(line.c_str());
					break;
				case 5:
					virtualKeyCode = atoi(line.c_str());
					break;
				case 6:
					modifiers = atoi(line.c_str());
					break;
				case 7:
					moveX = atoi(line.c_str());
					break;
				case 8:
					moveY = atoi(line.c_str());
					break;
				case 9:
					leftClick = atoi(line.c_str());
					break;
				case 10:
					rightClick = atoi(line.c_str());
					break;
				case 11:
					sortNum = atoi(line.c_str());
					break;
				case 12:
					strcpy(itemID, line.c_str());
					for(int j=0; j<items.size(); j++)
					{
						if(strcmp(items.at(j)->getID(), itemID) == 0)
						{
							
							item = items.at(j);
						}
					}
					if( item == NULL)
						return -1;
						
					break;
				default:
					break;
				}
				if ( j >= 13)
				{
					strcpy(ifItemID, line.c_str());
					for(int k=0; k<items.size(); k++)
					{
						if(strcmp(items.at(k)->getID(), ifItemID) == 0)
						{
							_ifItems.push_back(items.at(k));
						}

					}
				}
				
				

				
				

				//JhonnyRunItem item(runID, runName, runNote, priority, repeatTime, item, NULL);
				
			}
			//runs.push_back(JhonnyRunItem(runID, runName, priority, repeatTime, virtualKeyCode, modifiers, moveX, moveY, leftClick, rightClick, sortNum, item, _ifItems));
			input.close();
		}
	}
	else 
		return -1;

	std::sort( runs.begin(), runs.end(), compareObj);

	for(int i=0; i<runs.size(); i++)
	{
		int index = -1;
		for(int j=0; j<items.size(); j++)
		{
			if(strcmp(items.at(j)->getID(), runs.at(i).item->getID()) == 0)
			{
				index = j;
			}

		}
		if(index == -1)
			return -1;
		insertJhonnyRunItem(&listItems, &runItems , &runs.at(i), index);
	}

	for(int i=0; i<runItemFiles.size(); i++)
		delete[] runItemFiles.at(i);
	return 0;
}
	

int JhonnyAutoMain::uncompressFiles(char* loadPath)
{
	HZIP hz = OpenZip(loadPath, 0);  
	ZIPENTRY ze; GetZipItem(hz,-1,&ze);
	int numitems=ze.index;
	for (int i=0; i<numitems; i++)
	{ 
		GetZipItem(hz,i,&ze);               
		char dest[MAX_PATH];
		sprintf(dest, "%s\\%s", rootPath,ze.name);     
		UnzipItem(hz,i,dest);       
	}   
	CloseZip(hz);
	return 0;
}

int JhonnyAutoMain::deleteTempData()
{
	// list items
	char filePath[MAX_PATH];
	char runFilePath[MAX_PATH];
	char listFilePath[MAX_PATH];
	char imageDest[MAX_PATH];
	char infoDest[MAX_PATH];
	CFileFind pFind;
	BOOL bRet; 

	GetCurrentDirectory(MAX_PATH, filePath);
	strcpy(runFilePath, filePath);
	strcpy(listFilePath, filePath);
	strcat(listFilePath, "\\");
	strcat(listFilePath, listItemDirName);

	

	
	std::vector<char*> fileList;
	getFiles(listFilePath, "\\*.*", &fileList);
	for(int i=0; i<fileList.size(); i++)
	{
		int j;
		for(j=0; j<items.size(); j++)
		{
			strcpy(imageDest, listFilePath);
			strcat(imageDest, "\\");
			strcat(imageDest, items.at(j)->getID());
			strcat(imageDest, ".bmp");

			strcpy(infoDest, listFilePath);
			strcat(infoDest, "\\");
			strcat(infoDest, items.at(j)->getID());
			strcat(infoDest, ".txt");

			if(strcmp(imageDest, fileList.at(i)) == 0 ||  strcmp(infoDest, fileList.at(i)) == 0 )
				break;
			
		}
		if(j==items.size())
		{
			bRet = pFind.FindFile(fileList.at(i)); // 파일 존재여부 파악
			if(bRet == true)
				DeleteFile(fileList.at(i)); // 파일삭제 
			else
			{
				pFind.Close();
				return -1;
			}
		}
		
	}


	strcat(runFilePath, "\\");
	strcat(runFilePath, runItemDirName);
	getFiles(runFilePath, "\\*.*", &fileList);
	for(int i=0; i<fileList.size(); i++)
	{
		int j;
		for(j=0; j<runs.size(); j++)
		{
			strcpy(infoDest, runFilePath);
			strcat(infoDest, "\\");
			strcat(infoDest, runs.at(j).getID());
			strcat(infoDest, ".txt");

			if(strcmp(infoDest, fileList.at(i)) == 0 )
				break;
			
		}

		if(j==runs.size())
		{
			bRet = pFind.FindFile(fileList.at(i)); // 파일 존재여부 파악
			if(bRet == true)
				DeleteFile(fileList.at(i)); // 파일삭제 
			else
			{
				pFind.Close();
				return -1;
			}
		}
		
	}
	pFind.Close();
	return 0;
}

int JhonnyAutoMain::doSave()
{
	char savePath[MAX_PATH];
	char name_filter[] = "Save File (*.jam)|*.jam|";
		CFileDialog ins_dlg(FALSE, "jam", "*.jam", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT |
													OFN_NOCHANGEDIR, name_filter, NULL);

	if(listItems.GetItemCount() > 0)
	{
		// TODO: Add your command handler code here
		

		if(ins_dlg.DoModal() == IDOK){
			SetCurrentDirectory(rootPath);
			strcpy(savePath, ins_dlg.GetPathName());
		}
		else
			return -1;
	}
	else
	{
		AfxMessageBox("저장 할 아이템이 없습니다.");
		return -1;
	}
	
	if(deleteTempData() != 0)
	{
		AfxMessageBox("임시 데이터를 삭제하는 동안 문제가 발생하였습니다.");
		return -1;
	}

	if(saveRunItems() != 0)
	{
		AfxMessageBox("실행아이템을 저장하는 동안 문제가 발생하였습니다.");
		return -1;
	}
	if(saveListItems() != 0)
	{
		AfxMessageBox("리스트아이템을 저장하는 동안 문제가 발생하였습니다.");
		return -1;
	}
	
	if(compressFiles(savePath) != 0)
	{
		AfxMessageBox("저장하는 동안 문제가 발생하였습니다.");
		return -1;
	}
	SetWindowText(ins_dlg.GetFileName());
	return 0;
}


int JhonnyAutoMain::saveListItems()
{
	CFileFind pFind;
	BOOL bRet; 
	char filePath[MAX_PATH];
	char infoDest[MAX_PATH];
	char imageDest[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, filePath);
	strcat(filePath, "\\");
	strcat(filePath, listItemDirName);

	for(int i=0; i<items.size(); i++)
	{
		strcpy(imageDest, filePath);
		strcat(imageDest, "\\");
		strcat(imageDest, items.at(i)->getID());
		strcat(imageDest, ".bmp");
		bRet = pFind.FindFile(imageDest); // 파일 존재여부 파악
		if(bRet != true)
		{
			pFind.Close();
			return -1;
		}

		strcpy(infoDest, filePath);
		strcat(infoDest, "\\");
		strcat(infoDest, items.at(i)->getID());
		strcat(infoDest, ".txt");
		bRet = pFind.FindFile(infoDest); // 파일 존재여부 파악
		if(bRet != true)
		{
			pFind.Close();
			return -1;
		}

	}
	pFind.Close();
	return 0;
}

int JhonnyAutoMain::saveRunItems()
{
	CFileFind pFind;
	BOOL bRet; 
	char filePath[MAX_PATH];
	char infoDest[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, filePath);
	strcat(filePath, "\\");
	strcat(filePath, runItemDirName);

	for(int i=0; i<runs.size(); i++)
	{
		strcpy(infoDest, filePath);
		strcat(infoDest, "\\");
		strcat(infoDest, runs.at(i).getID());
		strcat(infoDest, ".txt");


		
	
		bRet = pFind.FindFile(infoDest); // 파일 존재여부 파악
		if(bRet == true)
			DeleteFile(infoDest); // 파일삭제 
		
		//MoveFile(imageSrc,  imageDest);
		//strcpy(item.bitmapPath, imageDest);

		/*
		bRet = pFind.FindFile(infoDest); // 파일 존재여부 파악
		if(bRet == true)
			DeleteFile(infoDest); // 파일삭제 
		strcpy(item.bitmapPath, infoDest);
			*/

	
		int fileExist = _access(infoDest, 0);
		std::ofstream output(infoDest,  std::ios::out | std::ios::trunc );  
		if (!output)
		{ 
			std::cerr<<"Error writing to ..."<<std::endl; 
			pFind.Close();
			output.close();
			return -1;
		} 

		output << runs.at(i).getID() << std::endl;
		output << runs.at(i).getName() << std::endl;
		output << runs.at(i).getPriority() << std::endl;
		output << runs.at(i).getRepeatTime() << std::endl;
		output << runs.at(i).getVirtualKeyCode() << std::endl;
		output << runs.at(i).getModifiers() << std::endl;
		output << runs.at(i).getMoveX() << std::endl;
		output << runs.at(i).getMoveY() << std::endl;
		output << runs.at(i).getLeftClick() << std::endl;
		output << runs.at(i).getRightClick() << std::endl;
		output << i << std::endl;
		output << runs.at(i).item->getID() << std::endl;
		for(int j=0; j<runs.at(i).ifItems.size(); j++)
			output << runs.at(i).ifItems.at(j)->getID() << std::endl;


		output.close();
		pFind.Close();
	}
	return 0;
}

int JhonnyAutoMain::compressFiles(char* savePath)
{
	char filePath[MAX_PATH];
	char dstZipPath[MAX_PATH];
	char  szDirPath[MAX_PATH] = "";
	char  listItemDirSearch[MAX_PATH] = "";
	char  runItemDirSearch[MAX_PATH] = "";
	HANDLE hSrch;
	WIN32_FIND_DATA stWinFindData;
	BOOL   bResult = TRUE;

	HZIP hz;
	GetCurrentDirectory(MAX_PATH, filePath);
	strcpy(dstZipPath, filePath);
	strcat(dstZipPath, "\\data");
	
	hz = CreateZip ( savePath, 0 );
	ZipAddFolder(hz, listItemDirName);
	ZipAddFolder(hz, runItemDirName);
	
	
	
	
	strcpy(listItemDirSearch, listItemDirName);
	strcat(listItemDirSearch, "\\*.*");
	hSrch = FindFirstFile ( listItemDirSearch, &stWinFindData );
	if ( hSrch == INVALID_HANDLE_VALUE )
		return FALSE;
	while ( bResult )
	{
		if ( !(stWinFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
		{
			OutputDebugString(stWinFindData.cFileName);
			OutputDebugString("\n");
			sprintf ( szDirPath, "%s\\%s", listItemDirName, stWinFindData.cFileName );
			ZipAdd ( hz, szDirPath, szDirPath );
		}
		bResult = FindNextFile ( hSrch, &stWinFindData );
	}
	FindClose(hSrch);

	SetCurrentDirectory(rootPath);
	bResult = TRUE;
	strcpy(runItemDirSearch, runItemDirName);
	strcat(runItemDirSearch, "\\*.*");
	hSrch = FindFirstFile ( runItemDirSearch, &stWinFindData );
	if ( hSrch == INVALID_HANDLE_VALUE )
		return FALSE;
	while ( bResult )
	{
		if ( !(stWinFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
		{
			OutputDebugString(stWinFindData.cFileName);
			OutputDebugString("\n");
			sprintf ( szDirPath, "%s\\%s", runItemDirName, stWinFindData.cFileName );
			ZipAdd ( hz, szDirPath, szDirPath );
		}
		bResult = FindNextFile ( hSrch, &stWinFindData );
	}
	FindClose(hSrch);
	CloseZip ( hz );
	SetCurrentDirectory(rootPath);
	return 0;	
}

int getFiles(char* path, char* extensionType, std::vector<char*>* fileList)
{
	char  filePathSearch[MAX_PATH];
	HANDLE hSrch;
	WIN32_FIND_DATA stWinFindData;
	BOOL   bResult = TRUE;
	
	strcpy(filePathSearch, path);
	strcat(filePathSearch, extensionType);
	hSrch = FindFirstFile ( filePathSearch, &stWinFindData );
	if ( hSrch == INVALID_HANDLE_VALUE )
		return -1;
	fileList->clear();


	while ( bResult )
	{
		if ( !(stWinFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
		{
			char* inputPath = new char[MAX_PATH];
			sprintf ( inputPath, "%s\\%s", path, stWinFindData.cFileName );
			fileList->push_back(inputPath);
		}
		bResult = FindNextFile ( hSrch, &stWinFindData );
	}
	FindClose(hSrch);
	return 0;
}


int JhonnyAutoMain::doStartTimer(UINT idEvent, DWORD dwTime)
{
	

	int index = -1;
	for(int i=0; i<runs.size(); i++)
	{
		if(atoi(runs.at(i).getID())== idEvent)
			index = i;
	}
	
	
	if(index == -1)
		return -1;

	int i=0;
	int max = runsPool.size();
	if(max == 0)
	{
		runsPool.push_back(&runs.at(index));
	}
	else
	{
		for(i=0; i<max; i++)
		{
			if(atoi(runsPool.at(i)->getID()) == idEvent)
				break;
		}
		if(i == max)
			runsPool.push_back(&runs.at(index));
	}

	
	CString strTmp;
	strTmp.Format("call : %s, array size : %d\n", runs.at(index).getName(), runsPool.size());
	OutputDebugString(strTmp);
	return 0;
}



void JhonnyAutoMain::OnBnClickedStart()
{
	// TODO: Add your control notification handler code here
	UpdateData();

	

	CString strLine;
	if(btnAdd.IsWindowEnabled())
	{
		if(runItems.GetItemCount() <= 0)
		{
			AfxMessageBox("실행 아이템이 없습니다.\n위에 모든 아이템에서 아래로 끌어다 놓아주세요.");
			return ;
		}

		POSITION pos = runItems.GetFirstSelectedItemPosition();
		while( pos ) {
			int index = runItems.GetNextSelectedItem( pos );
			runItems.SetItemState(index, 0, LVIS_SELECTED);
		}

		pos = listItems.GetFirstSelectedItemPosition();
		while( pos ) {
			int index = listItems.GetNextSelectedItem( pos );
			listItems.SetItemState(index, 0, LVIS_SELECTED);
		}

		listItems.EnableWindow(false);
		runItems.EnableWindow(false);
		btnAdd.EnableWindow(false);
		GetDlgItem(IDC_RADIO_RUN1)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_RUN2)->EnableWindow(FALSE);
		btnStart.SetWindowTextA("중지");
		CString strTmp;
		strTmp.Format("size : %d %d\n", items.size(), runs.size());
		OutputDebugString(strTmp);
		//core.setRunMain(&runs);
		//core.start(&core, 3000);
		runsPool.clear();
		switch (radioTimeRun)
		{
		case 0:
			for(int i=0; i<runs.size(); i++)
			{
				UINT_PTR id = :: SetTimer(hWnd, atoi(runs.at(i).getID()), runs.at(i).getRepeatTime() * 1000,   JhonnyAutoMain::runTimerProc);
				m_JhonnyAutoMainMap[id]= this; 
				strTmp.Format("start : %d\n", atoi(runs.at(i).getID()));
				OutputDebugString(strTmp);
			}
			break;
		case 1:
			for(int i=0; i<runs.size(); i++)
			{
				runsPool.push_back(&runs.at(i));
			}
			break;
		default:
			break;
		}
		strLine.Format(_T("\r\n프로그램이 시작되었습니다."));
		int nLength = editLogCtrl.GetWindowTextLength();
		editLogCtrl.SetSel(nLength, nLength);
		editLogCtrl.ReplaceSel(strLine);
		RegisterHotKey(hWnd, HOTKEY_STOP, MOD_CONTROL | MOD_ALT | MOD_SHIFT , VK_F4);
	

	}
	else
	{
		//core.end();
		Sleep(500);

		

		for(int i=0; i<runs.size(); i++)
		{
			bool result = ::KillTimer(hWnd, atoi(runs.at(i).getID()));
			CString strTmp;
			strTmp.Format("KillTimer : %s %d\n", runs.at(i).getName(), result);
			OutputDebugString(strTmp);
		}

		
		POSITION pos = runItems.GetFirstSelectedItemPosition();
		while( pos ) {
			int index = runItems.GetNextSelectedItem( pos );
			runItems.SetItemState(index, 0, LVIS_SELECTED);
		}

		pos = listItems.GetFirstSelectedItemPosition();
		while( pos ) {
			int index = listItems.GetNextSelectedItem( pos );
			listItems.SetItemState(index, 0, LVIS_SELECTED);
		}


		listItems.EnableWindow(true);
		runItems.EnableWindow(true);
		btnAdd.EnableWindow(true);
		GetDlgItem(IDC_RADIO_RUN1)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_RUN2)->EnableWindow(TRUE);


		btnStart.SetWindowTextA("시작");
		strLine.Format(_T("\r\n프로그램이 중지되었습니다."));
		int nLength = editLogCtrl.GetWindowTextLength();
		editLogCtrl.SetSel(nLength, nLength);
		editLogCtrl.ReplaceSel(strLine);
		UnregisterHotKey(hWnd, HOTKEY_STOP);
	}
	
	
	
}

BOOL DeleteDir(CString dir)
{
    if(dir == _T(""))
    {
        return FALSE;
    }
 
    BOOL bRval = FALSE;
    int nRval = 0;
    CString szNextDirPath = _T("");
    CString szRoot = _T("");
    CFileFind find;
 
    // Directory가 존재 하는지 확인 검사
    bRval = find.FindFile(dir);
 
    if(bRval == FALSE)
    {
        return bRval;
    }
 
    while(bRval)
    {
        bRval = find.FindNextFile();
 
        // . or .. 인 경우 무시한다.
        if(find.IsDots() == TRUE)
        {
            continue;
        }
 
        // Directory 일 경우
        if(find.IsDirectory())
        {
            szNextDirPath.Format(_T("%s\\*.*"), find.GetFilePath());
 
            // Recursion function 호출
            DeleteDir(szNextDirPath);
        }
 
        // file일 경우
        else
        {
            //파일 삭제
            ::DeleteFile(find.GetFilePath());
        }
    }
 
    szRoot = find.GetRoot();
    find.Close();
 
    Sleep(1);
    bRval = RemoveDirectory(szRoot);
 
    return bRval;
}


BOOL JhonnyAutoMain::OnInitDialog()
{
	CDialog::OnInitDialog();
	  
	srand((unsigned int)time(NULL));

	pImageItemsDrag = NULL;
	nIndexLeftSel = -1;

	strcpy(listItemDirName,"data\\listitem");
	strcpy(runItemDirName,"data\\runitem");
	
	char filePath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, rootPath);
	
	

	

	CFileFind pFind;
	BOOL bRet; 

	GetCurrentDirectory(MAX_PATH, filePath);
	strcat(filePath, "\\data");
	bRet = pFind.FindFile(filePath); // 파일 존재여부 파악
	if(bRet == true)
		DeleteDir(filePath);
	CreateDirectory(filePath, NULL);


	GetCurrentDirectory(MAX_PATH, filePath);
	strcat(filePath, "\\");
	strcat(filePath, listItemDirName);
	/*
	bRet = pFind.FindFile(filePath); // 파일 존재여부 파악
	if(bRet == true)
		DeleteDir(filePath);
		*/
	CreateDirectory(filePath, NULL);

	GetCurrentDirectory(MAX_PATH, filePath);
	strcat(filePath, "\\");
	strcat(filePath, runItemDirName);
	/*
	bRet = pFind.FindFile(filePath); // 파일 존재여부 파악
	if(bRet == true)
		DeleteDir(filePath);
		*/
	CreateDirectory(filePath, NULL);

	// TODO:  Add extra initialization here
	/*
	LVITEM listObj;
	::ZeroMemory(&listObj, sizeof(listObj));
	listObj.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
	listObj.pszText = "test";
	listObj.iItem = 5;
	listObj.iImage = 1;
	listObj.state = LVIS_SELECTED | LVIS_FOCUSED;

	listItems.InsertItem(&listObj);
	*/
	//items.resize(100);
	//	runs.resize(100);

	imageItems.Create(ICON_SIZE, ICON_SIZE, ILC_COLOR32 | ILC_MASK, 0, 0);
	
	listItems.InsertColumn(0, "아이템 이름", LVCFMT_LEFT, 100);
	listItems.InsertColumn(1, "아이템 설명", LVCFMT_LEFT, 200);
	listItems.SetImageList(&imageItems, LVSIL_NORMAL);
	listItems.SetImageList(&imageItems, LVSIL_SMALL);

	runItems.InsertColumn(0, "아이템 이름", LVCFMT_LEFT, 100);
	runItems.InsertColumn(1, "아이템 설명", LVCFMT_LEFT, 200);
	runItems.SetImageList(&imageItems, LVSIL_NORMAL);
	runItems.SetImageList(&imageItems, LVSIL_SMALL);



	

	DWORD dwExStyle = listItems.GetExtendedStyle();
	listItems.SetExtendedStyle ( dwExStyle | LVS_EX_BORDERSELECT);
	dwExStyle = runItems.GetExtendedStyle();
	runItems.SetExtendedStyle ( dwExStyle | LVS_EX_BORDERSELECT);
	runItems.ModifyStyle(0, LVS_SHOWSELALWAYS); 

	sliderThreshold.SetRange(1,100);
	sliderThreshold.SetPos(90);
	sliderThreshold.SetTic(2);

	hWnd = GetSafeHwnd();
	//core = JhonnyAutoCore(&hWnd, &runsPool);
	
	char thres[50];
	int thresInt = core.threshold * 100.f;
	sprintf(thres, "%d", thresInt);
	staticThreshold.SetWindowTextA(thres);

	pFind.Close();
	SetCurrentDirectory(rootPath);

	
	


	//listItems.SetItemText(0, 0, "name");
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}



inline void writeLogForVisualStudio(const char * pszFormat, ...)
{
	const int MAX_LEN = 200;
    char szBuf[MAX_LEN];
	va_list ap;
	va_start(ap, pszFormat);
	vsnprintf_s(szBuf, MAX_LEN, MAX_LEN, pszFormat, ap);
	va_end(ap);
	WCHAR wszBuf[MAX_LEN] = {0};
	MultiByteToWideChar(CP_UTF8, 0, szBuf, -1, wszBuf, sizeof(wszBuf));
	OutputDebugStringW(wszBuf);
	OutputDebugStringA("\n");
}

int JhonnyAutoMain::insertJhonnyItem(CListCtrl* list ,JhonnyItem* item, int index)
{
	char imageSize[50];
	sprintf(imageSize, "%d x %d", item->getIplImage()->width, item->getIplImage()->height);
			
	
			
	LVITEM listObj;
	::ZeroMemory(&listObj, sizeof(listObj));
	listObj.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
	listObj.iItem = index;
	listObj.iImage = index;
	listObj.state = LVIS_SELECTED | LVIS_FOCUSED;
			
			
	HBITMAP hBitSelect = NULL;
	hBitSelect = (HBITMAP)::LoadImage(NULL, item->getBitmapPath(), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR | LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
	if( hBitSelect == NULL )
		return -1;
			
	BITMAP bm = {0};
	GetObject(hBitSelect, sizeof(BITMAP), &bm);

	int cx = ICON_SIZE;
	int cy = ICON_SIZE;


	CPaintDC dc(this);
	//HDC dc;
	HDC memdc = CreateCompatibleDC(dc);
	HDC memdc2 = CreateCompatibleDC(dc);
	HBITMAP hbm = CreateCompatibleBitmap(dc, cx, cy);
	HGDIOBJ hOld = SelectObject(memdc, hbm);
	SelectObject(memdc2, hBitSelect);
	StretchBlt(memdc, 0, 0, cx, cy, memdc2, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

	SelectObject(memdc, hOld);
	DeleteDC(dc);
	DeleteDC(memdc);
	DeleteDC(memdc2);


	CBitmap lamp_image ;
	lamp_image.Attach(hbm);


	int result = imageItems.Add(&lamp_image, RGB(255, 255, 255));
	if(result == -1)
		return -1;
			
	list->InsertItem(&listObj);

	//list->SetItemText(index, 1, imageSize);
	//list->SetItemText(index, 1, item->getNote());

	CString strTmp;
	OutputDebugString(strTmp);

	return 0;
}

int JhonnyAutoMain::insertJhonnyRunItem(CListCtrl* parent, CListCtrl*  list ,JhonnyRunItem* item, int index)
{
	LVITEM lvItem;
	TCHAR szBuffer[256];
	::ZeroMemory(&lvItem, sizeof(lvItem));
	::ZeroMemory(szBuffer, sizeof(szBuffer));
	lvItem.mask = LVIF_TEXT | LVIF_IMAGE;
	lvItem.iItem = index;
	lvItem.pszText = szBuffer;
	lvItem.cchTextMax = 256;
	parent->GetItem(&lvItem);
	list->InsertItem(runItems.GetItemCount(), item->getName(), lvItem.iImage);

	return 0;
}

void JhonnyAutoMain::OnBnClickedAdd()
{
	// TODO: Add your control notification handler code here
	POSITION pos = runItems.GetFirstSelectedItemPosition();
	while( pos ) {
		int index = runItems.GetNextSelectedItem( pos );
		runItems.SetItemState(index, 0, LVIS_SELECTED);
	}

	pos = listItems.GetFirstSelectedItemPosition();
	while( pos ) {
		int index = listItems.GetNextSelectedItem( pos );
		listItems.SetItemState(index, 0, LVIS_SELECTED);
	}

	ShowWindow(SW_HIDE);
	JhonnyRegister ciDlg;
	INT_PTR response = ciDlg.DoModal();
	if(response == IDOK)
	{
		if(items.size() > 0)
		{
			insertJhonnyItem(&listItems, items.at(items.size()-1), listItems.GetItemCount());
		}
		

	}
	else if(response == IDCANCEL)
	{
	}

	ShowWindow(SW_SHOW);
}
	


void JhonnyAutoMain::OnClose()
{
	// TODO: Add your message handler code here and/or call default.

	for(int i=0; i<items.size(); i++)
	{
		JhonnyItem* item = items.at(i);
		//if(item->hBitmap != NULL) ::DeleteObject(item->hBitmap);
		IplImage* image = item->getIplImage();
		if(image != NULL) cvReleaseImage(&image);
	}


	EndDialog(-1);
	CDialog::OnClose();
}

void JhonnyAutoMain::OnLvnBegindragListRunItems(NMHDR *pNMHDR, LRESULT *pResult)
{
	isStartItemsList = false;
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	runItems.SetItemState(pNMLV->iItem, 0, LVIS_SELECTED);
	
	CPoint ptDrag, ptAction;
	nIndexLeftSel = pNMLV->iItem;
	pImageItemsDrag = runItems.CreateDragImage(pNMLV->iItem, &ptDrag);
	pImageItemsDrag->SetBkColor(RGB(0,0,0));
	ptAction = pNMLV->ptAction;

	SetCapture();
	pImageItemsDrag->BeginDrag(0, ptAction - ptDrag);
	runItems.ClientToScreen(&ptAction);
	pImageItemsDrag->DragEnter(NULL, ptAction);


	*pResult = 0;
}

void JhonnyAutoMain::OnLvnBegindragListItems(NMHDR *pNMHDR, LRESULT *pResult)
{
	isStartItemsList = true;
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	listItems.SetItemState(pNMLV->iItem, 0, LVIS_SELECTED);
	
	CPoint ptDrag, ptAction;
	nIndexLeftSel = pNMLV->iItem;
	pImageItemsDrag = listItems.CreateDragImage(pNMLV->iItem, &ptDrag);
	pImageItemsDrag->SetBkColor(RGB(0,0,0));
	ptAction = pNMLV->ptAction;

	SetCapture();
	pImageItemsDrag->BeginDrag(0, ptAction - ptDrag);
	listItems.ClientToScreen(&ptAction);
	pImageItemsDrag->DragEnter(NULL, ptAction);

	*pResult = 0;
}


void JhonnyAutoMain::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	if(pImageItemsDrag != NULL)
	{
		ClientToScreen(&point);
		
		CWnd* pWnd = CWnd::WindowFromPoint(point);
		if(pWnd != NULL)
		{
			if(pWnd != NULL)
			{
				if(this == pWnd || IsChild(pWnd))
				{
					pImageItemsDrag->DragEnter(NULL, point);
					pImageItemsDrag->DragMove(point);
				}
				else
					pImageItemsDrag->DragLeave(NULL);
			}
		}
	}

	CDialog::OnMouseMove(nFlags, point);
}


void JhonnyAutoMain::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CString strTmp = _T("");
	
	if(pImageItemsDrag != NULL)
	{
		ClientToScreen(&point);
		pImageItemsDrag->DragLeave(NULL);
		pImageItemsDrag->EndDrag();
		ReleaseCapture();

		CWnd* pWnd = CWnd::WindowFromPoint(point);
		if(pWnd == &runItems && nIndexLeftSel >= 0 )
		{

			runItems.ScreenToClient(&point);
			int nIndex = runItems.HitTest(point);
			if(isStartItemsList == true)		// 리스트아이템 -> 런아이템
			{
				if(nIndex == -1)
				{
					// 아래쪽에 아이템 추가
					LVITEM lvItem;
					TCHAR szBuffer[256];
					::ZeroMemory(&lvItem, sizeof(lvItem));
					::ZeroMemory(szBuffer, sizeof(szBuffer));

					lvItem.mask = LVIF_TEXT | LVIF_IMAGE;
					lvItem.iItem = nIndexLeftSel;
					lvItem.pszText = szBuffer;
					lvItem.cchTextMax = 256;
					listItems.GetItem(&lvItem);
			
			
					runItems.InsertItem(runItems.GetItemCount(), lvItem.pszText, lvItem.iImage);
					runs.push_back(&(*items.at(nIndexLeftSel)));
				}
				else       
				{
					runs.at(nIndex).ifItems.push_back(&(*items.at(nIndexLeftSel)));
					AfxMessageBox("조건 아이템으로 추가 되었습니다");
				}
				
			}
			else          // 런아이템 -> 런아이템
			{
				if(nIndex != -1)
				{
					SwapItems(&runItems, nIndexLeftSel, nIndex);
					std::iter_swap(runs.begin()+nIndexLeftSel, runs.begin() + nIndex);
				}
			}
			strTmp.Format("drop : %d\n", nIndex);
			OutputDebugStringA(strTmp);
		}

		delete pImageItemsDrag;
		pImageItemsDrag = NULL;
		nIndexLeftSel = -1;
	}

	CDialog::OnLButtonUp(nFlags, point);
}


void JhonnyAutoMain::OnNMReleasedcaptureSliderThreshold(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;

	int nPos = sliderThreshold.GetPos();
	core.setThreshold((float)nPos / 100.0f);
	char thres[50];
	sprintf(thres, "%d", nPos);
	staticThreshold.SetWindowTextA(thres);
	//stringThreshold.SetString
}


//void JhonnyAutoMain::OnHdnItemdblclickListRunItems(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
//	// TODO: Add your control notification handler code here
//	*pResult = 0;
//
//	AfxMessageBox("더불클릭!");
//}


//void JhonnyAutoMain::OnNMDblclkListItems(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
//	// TODO: Add your control notification handler code here
//
//	if(pNMItemActivate->iItem != -1)
//	{
//		AfxMessageBox("zz");
//	}
//
//	*pResult = 0;
//
//}


void JhonnyAutoMain::OnNMDblclkListRunItems(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	int index = pNMItemActivate->iItem;
	if(index != -1)
	{

		JhonnyRegisterRun ciDlg;
		ciDlg.setRunItem(&runs.at(index));
		//ciDlg.setListItems(&items);
		//ciDlg.setIndex(index);
		CString strTmp;
		strTmp.Format("index : %d\n", index);
		OutputDebugString(strTmp);
		OutputDebugString(runItems.GetItemText(index,0));
				

		INT_PTR response = ciDlg.DoModal();
		if(response == IDOK)
			runItems.SetItemText(index, 0, runs.at(index).getName());

	
	}


	*pResult = 0;
}


BOOL JhonnyAutoMain::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	// 여기의 if 문은 다이얼로그에서 'Enter' 키와 'ESC' 키를 누를때 
	// 창이 닫히는 것을 방지하기 위한 구문입니다.
	if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
	{
		return TRUE;
	}

	// 사용자가 원하는 키 입력 구문
	if(pMsg->wParam == VK_DELETE)
	{
		CString strTmp;
		POSITION listItemPos = listItems.GetFirstSelectedItemPosition();
		int listItemIndex = listItems.GetNextSelectedItem(listItemPos);
		POSITION runItemPos = runItems.GetFirstSelectedItemPosition();
		int runItemIndex = runItems.GetNextSelectedItem(runItemPos);

		if(listItemIndex != -1 || runItemIndex != -1)
		{
			if (listItemIndex != -1)
			{
				if(AfxMessageBox("리스트 아이템을 삭제하면 연결된 실행 아이템이 삭제됩니다.\n그래도 삭제하시겠습니까?", MB_OKCANCEL|MB_ICONASTERISK) == IDOK)
				{
					// list item과 연결된 run item들 모두 삭제
					char* eraseItemID = items.at(listItemIndex)->getID();

					
					
					LVITEM lvItem;
					for(int i=0; i<runItems.GetItemCount(); i++)
					{
						::ZeroMemory(&lvItem, sizeof(lvItem));
						lvItem.iItem = i;
						lvItem.mask = LVIF_IMAGE;
						runItems.GetItem(&lvItem);
						if(lvItem.iImage >=  listItemIndex)
							lvItem.iImage = lvItem.iImage-1;
						runItems.SetItem(&lvItem);
					}


					for(int i=0; i<runs.size(); i++)
					{
						if(strcmp(runs.at(i).item->getID(), eraseItemID) == 0)
						{
							runItems.DeleteItem(i);
							runs.erase(runs.begin() + i);
							i--;
							continue;
						}
						else
						{
							for(int j=0; j<runs.at(i).ifItems.size(); j++)
							{
								if(strcmp(runs.at(i).ifItems.at(j)->getID(), eraseItemID) == 0)
								{
									runs.at(i).ifItems.erase(runs.at(i).ifItems.begin() + j);
									j--;
									continue;
								}
							}
						}
					}

				

					
					listItems.DeleteItem(listItemIndex);
					items.erase(items.begin()+listItemIndex);
					imageItems.Remove(listItemIndex);
					LVITEM lvitem;
					for(int i=0; i<listItems.GetItemCount(); i++)
					{
						lvitem.iItem = i;
						lvitem.mask = LVIF_IMAGE;
						listItems.GetItem(&lvitem);
						lvitem.iImage = i;
						listItems.SetItem(&lvitem);
					}
					


					strTmp.Format("%d delete.(%d,%d,%d)\n", listItemIndex, listItems.GetItemCount(), runs.size(), imageItems.GetImageCount());
					OutputDebugString(strTmp);
					strTmp.Format("%d delete.(%d,%d)\n", runItemIndex, runItems.GetItemCount(), runs.size());
					OutputDebugString(strTmp);
				}
			
			}
			else
			{
				if(AfxMessageBox("실행 아이템을 삭제하시겠습니까?", MB_OKCANCEL|MB_ICONASTERISK) == IDOK)
				{
					if (runItemIndex != -1)
					{
						
						runItems.DeleteItem(runItemIndex);
						runs.erase(runs.begin()+runItemIndex);
						strTmp.Format("%d delete.(%d,%d)\n", runItemIndex, runItems.GetItemCount(), runs.size());
						OutputDebugString(strTmp);
					}
				}
			}
		}


		
	
		

		


	

		

		return TRUE;
	}


	return CDialog::PreTranslateMessage(pMsg);
}


void JhonnyAutoMain::OnMenuExit()
{
	// TODO: Add your command handler code here
	OnOK();
}


void JhonnyAutoMain::OnMenuSave()
{
	doSave();
}


void JhonnyAutoMain::OnMenuLoad()
{
	// TODO: Add your command handler code here
	if(listItems.GetItemCount() > 0)
	{
		int ans = AfxMessageBox("현재 아이템을 저장하시겠습니까?", MB_YESNOCANCEL);
		switch (ans)
		{
		case IDYES:
			doSave();
			doLoad();
			break;
		case IDNO:
			doLoad();
			break;
		case IDCANCEL:
			break;
		default:
			break;
		}

	}
	else
	{
		doLoad();
	}

}


void JhonnyAutoMain::OnNMClickListItems(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	POSITION pos = runItems.GetFirstSelectedItemPosition();
	while( pos ) {
		int index = runItems.GetNextSelectedItem( pos );
		runItems.SetItemState(index, 0, LVIS_SELECTED);
	}

	*pResult = 0;
}


void JhonnyAutoMain::OnNMClickListRunItems(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	POSITION pos = listItems.GetFirstSelectedItemPosition();
	while( pos ) {
		int index = listItems.GetNextSelectedItem( pos );
		listItems.SetItemState(index, 0, LVIS_SELECTED);
	}


	*pResult = 0;
}


void JhonnyAutoMain::OnMenuInfo()
{
	// TODO: Add your command handler code here
	JhonnyAbout dlgAbout;
	dlgAbout.DoModal();
	
}


void JhonnyAutoMain::OnMenuHelp()
{
	// TODO: Add your command handler code here
	const char* helpUrl = "http://runafterthedream.blogspot.kr/2014/09/blog-post.html";
	::ShellExecute(NULL, "open", helpUrl, NULL, NULL, SW_SHOW);
	//runItems.change
}


void JhonnyAutoMain::OnMenuNew()
{
	// TODO: Add your command handler code here
	listItems.DeleteAllItems();
	items.clear();
	runItems.DeleteAllItems();
	//runItems.
	
	//std::iter_swap(runs.at(0), runs.at(1));
	while (imageItems.GetImageCount()) imageItems.Remove(0);
}


void SwapItems(CListCtrl* list, int idxFrom, int idxTo)
{
	LVITEM temp;
	LVITEM lvItem;
	TCHAR szBufferTemp[256];
	TCHAR szBuffer[256];

	::ZeroMemory(&temp, sizeof(temp));
	::ZeroMemory(szBufferTemp, sizeof(szBufferTemp));
	temp.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
	temp.iItem = idxTo;
	temp.pszText = szBufferTemp;
	temp.cchTextMax = 256;
	list->GetItem(&temp);
	temp.iItem = idxFrom;
	
	
	::ZeroMemory(&lvItem, sizeof(lvItem));
	::ZeroMemory(szBuffer, sizeof(szBuffer));
	lvItem.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
	lvItem.iItem = idxFrom;
	lvItem.pszText = szBuffer;
	lvItem.cchTextMax = 256;
	list->GetItem(&lvItem);
	lvItem.iItem = idxTo;

	list->SetItem(&lvItem);
	list->SetItem(&temp);
	
	
	
}




void JhonnyAutoMain::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
	 if(nHotKeyId == HOTKEY_STOP)
	 {
		 if(core.isPlaying==true)
		 {
			//core.end();
			Sleep(500);

		
		
			for(int i=0; i<runs.size(); i++)
			{
				bool result = ::KillTimer(hWnd, atoi(runs.at(i).getID()));
				CString strTmp;
				strTmp.Format("KillTimer : %s %d\n", runs.at(i).getName(), result);
				OutputDebugString(strTmp);
			}

			listItems.EnableWindow(true);
			runItems.EnableWindow(true);
			btnAdd.EnableWindow(true);
			GetDlgItem(IDC_RADIO_RUN1)->EnableWindow(TRUE);
			GetDlgItem(IDC_RADIO_RUN2)->EnableWindow(TRUE);
			POSITION pos = runItems.GetFirstSelectedItemPosition();
			while( pos ) {
				int index = runItems.GetNextSelectedItem( pos );
				runItems.SetItemState(index, 0, LVIS_SELECTED);
			}

			pos = listItems.GetFirstSelectedItemPosition();
			while( pos ) {
				int index = listItems.GetNextSelectedItem( pos );
				listItems.SetItemState(index, 0, LVIS_SELECTED);
			}

			CString strLine;
			btnStart.SetWindowTextA("시작");
			strLine.Format(_T("\r\n프로그램이 중지되었습니다."));
			int nLength = editLogCtrl.GetWindowTextLength();
			editLogCtrl.SetSel(nLength, nLength);
			editLogCtrl.ReplaceSel(strLine);
			UnregisterHotKey(hWnd, HOTKEY_STOP);
		 }
		
	 }


	CDialog::OnHotKey(nHotKeyId, nKey1, nKey2);
}
