#pragma once
#include <WinDef.h>
#include <Windows.h>

class MfcUtil
{
public:
	static void ShowTaskbarIcon(HWND WindowHandle);
	static void HideTaskbarIcon(HWND WindowHandle);
};