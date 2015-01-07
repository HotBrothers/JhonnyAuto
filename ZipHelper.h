#pragma once

// zlib �������  
#include "zlib.h"

// ǥ�� C�������
#include <stdio.h>
#include <vector>
#include "zip.h"



class ZipHelper
{
public:
	HZIP hZip;

	int doCompress(TCHAR* gzFileName, std::vector<TCHAR*> files);
	BOOL SetCompressDirectory (HZIP* phZip, TCHAR* pszCompressPath, TCHAR* pszZipCreationPath, TCHAR* pszZipName );
	BOOL InternalCompressDirectory  (HZIP* phZip, TCHAR* pszFilePath);
};

