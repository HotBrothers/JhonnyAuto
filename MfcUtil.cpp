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