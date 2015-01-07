#pragma once

// zlib 헤더파일  
#include "zlib.h"

// 표준 C헤더파일
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

