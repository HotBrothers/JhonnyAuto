
#include "stdafx.h"

#include "ZipHelper.h"

int ZipHelper::doCompress(TCHAR* gzFileName, std::vector<TCHAR*> files)
{

    gzFile  zfp;
    int     n;
    TCHAR    buff[1024];
    int     lerrno;
	FILE **fl;

 

	if(files.size() <= 0)
        return -1;


	fl = new FILE*[files.size()];


	// 압축파일을 연다.
	char fileName[MAX_PATH]="";
	int len = MAX_PATH;
	WideCharToMultiByte(CP_ACP, 0, gzFileName, len, fileName, len, NULL, NULL);

    if ((zfp = gzopen(fileName, "wb")) == NULL)
    {
        exit(0);
    }

	for(int i=0; i<files.size(); i++)
	{
		strcpy(fileName, "");
		WideCharToMultiByte(CP_ACP, 0, files.at(i), len, fileName, len, NULL, NULL);
		if ((fl[i] = fopen(fileName, "r")) < 0)
		{
			printf("file open error\n");
			return -1;
		}

		
		size_t   n_size;     // 읽거나 쓰기를 위한 데이터의 개수
		while( 0 < (n_size = fread( buff, 1, 1024, fl[i])))
		{
			 if (gzwrite(zfp, buff, n_size) < 0)
			{
				printf("%s\n",gzerror(zfp, &lerrno));
				return -1;
			}
		}    
	}

   




    gzclose(zfp);

    return 0;

}	


BOOL ZipHelper::SetCompressDirectory (HZIP* phZip, TCHAR* pszCompressPath, TCHAR* pszZipCreationPath, TCHAR* pszZipName )
{
	TCHAR szOldCurrentPath[MAX_PATH] = _T("");
	TCHAR szCurrentPath[MAX_PATH] = _T("");
	TCHAR szCompressFolderPath[MAX_PATH] = _T("");

	// You cannot create zip file into root directory such as C:\
	// because CreateZip doesn't support that path

	/*
	if ( StrCmpW ( pszZipCreationPath+1, ":") != 0)
	{
		OutputDebugString( pszZipCreationPath+1);
		return FALSE;
	}
	*/

	// Parsing Directory Name

	StrCpyW ( szCurrentPath, pszCompressPath );

	//strchr
	TCHAR* ucData 	 = StrChrW ( szCurrentPath, _T('\\') );
	int	   nPosition = ucData - szCurrentPath;

	//StrCpyW ( szCompressFolderPath, szCurrentPath + nPosition + 1 );
	StrCpyW ( szCompressFolderPath, szCurrentPath );

	if ( StrCmpW ( szCompressFolderPath, _T("")) == 0 )
		return FALSE;

	szCurrentPath[nPosition] = '\0';

	if ( StrCmpW ( szCurrentPath+1, _T(":")) != 0 )
	{
		StrCatW ( szCurrentPath, _T("\\") );
	}

	// Backup Current Directory
	GetCurrentDirectory ( _countof(szOldCurrentPath), szOldCurrentPath );
	
	// Set Current Directory For Create ZIP
	if ( !SetCurrentDirectory ( pszZipCreationPath ) )
		return FALSE;

	// Create Zip File
	*phZip = CreateZip ( pszZipName, 0 );

	if ( phZip == INVALID_HANDLE_VALUE )
	{
		return FALSE;
		SetCurrentDirectory ( szOldCurrentPath );
	}

	// Set Current Directory
	SetCurrentDirectory ( szCurrentPath );

	//ZipAddFolder(*phZip, szDirPath);
	//ZipAdd ( *phZip, szDirPath, szDirPath );
	
	if(InternalCompressDirectory ( phZip, szCompressFolderPath ) == FALSE)
		return FALSE;
		

	CloseZip ( *phZip );

	// Recover Current Directory 
	SetCurrentDirectory ( szOldCurrentPath );

	return TRUE;
}

BOOL ZipHelper::InternalCompressDirectory(HZIP* phZip, TCHAR* pszFilePath)
{
	BOOL   bResult = TRUE;
	TCHAR  szDrive[_MAX_DRIVE] = _T("");
	TCHAR  szDir[MAX_PATH] = _T("");
	TCHAR  szNewPath[MAX_PATH] = _T("");
	TCHAR  szDirPath[MAX_PATH] = _T("");
	HANDLE hSrch;

	WIN32_FIND_DATA stWinFindData;

	hSrch = FindFirstFile ( pszFilePath, &stWinFindData );

	if ( hSrch == INVALID_HANDLE_VALUE )
	{
		return FALSE;
	}

	
	//_tsplitpath_s ( pszFilePath, szDrive, _countof(szDrive), szDir, _countof(szDir), NULL, 0, NULL, 0 );
	_wsplitpath(pszFilePath, szDrive, szDir, NULL, NULL);

	while ( bResult )
	{
		if ( stWinFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
		{
			if ( StrCmpW ( stWinFindData.cFileName, _T(".") )  && StrCmpW ( stWinFindData.cFileName, _T("..") ) )
			{
				
				wsprintf ( szNewPath, _T("%s%s%s\\*.*"), szDrive, szDir, stWinFindData.cFileName );		
				wsprintf ( szDirPath, _T("%s%s%s"), szDrive, szDir, stWinFindData.cFileName );
				ZipAddFolder(*phZip, szDirPath);
				InternalCompressDirectory ( phZip, szNewPath );
			}
		} 

		else
		{
			wsprintf ( szDirPath, _T("%s%s%s"), szDrive, szDir, stWinFindData.cFileName );
			ZipAdd ( *phZip, szDirPath, szDirPath );
		}
		bResult = FindNextFile ( hSrch, &stWinFindData );
	}

	FindClose(hSrch);
	return TRUE;
}