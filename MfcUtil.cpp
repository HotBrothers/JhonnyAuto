#include "stdafx.h"
#include "MfcUtil.h"
#include <Shobjidl.h>

void MfcUtil::HideTaskbarIcon(HWND WindowHandle)
{
	ITaskbarList* TaskListPtr;
    CoInitialize(nullptr);
    long Result = !CoCreateInstance(CLSID_TaskbarList, nullptr, CLSCTX_SERVER, IID_ITaskbarList, reinterpret_cast<void**>(&TaskListPtr));
    if (Result) TaskListPtr->DeleteTab(WindowHandle);
    TaskListPtr->Release();
    CoUninitialize();
}

void MfcUtil::ShowTaskbarIcon(HWND WindowHandle)
{
	ITaskbarList* TaskListPtr;
    CoInitialize(nullptr);
    long Result = !CoCreateInstance(CLSID_TaskbarList, nullptr, CLSCTX_SERVER, IID_ITaskbarList, reinterpret_cast<void**>(&TaskListPtr));
    if (Result) TaskListPtr->AddTab(WindowHandle);
    TaskListPtr->Release();
    CoUninitialize();
}


HICON MfcUtil::GetAppIcon(HWND hwnd)
{
  HICON iconHandle = (HICON)SendMessage(hwnd,WM_GETICON,ICON_SMALL2,0);
  
  if(iconHandle == NULL)
    iconHandle = (HICON)SendMessage(hwnd,WM_GETICON,ICON_SMALL,0);
  if(iconHandle == NULL)
    iconHandle = (HICON)SendMessage(hwnd,WM_GETICON,ICON_BIG,0);
  if (iconHandle == NULL)
    iconHandle = (HICON)GetClassLongPtr(hwnd, GCL_HICON);
  if (iconHandle == NULL)
    iconHandle = (HICON)GetClassLongPtr(hwnd, GCL_HICONSM);
 
  if(iconHandle == NULL)
    return NULL;

 
  return iconHandle;
}
