// minizip_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <shlwapi.h>

#include "MiniZip.h"
#include "MiniUnzip.h"

BOOL DelDir(LPCTSTR pszDir);
void test_zip();
void test_unzip();
void test_unzip_w();

void test_zip()
{
    TCHAR szFile[MAX_PATH] = _T("E:\\Test\\hah.txt");
    TCHAR szPsw[MAX_PATH] = _T("abc");

    BOOL bRet = FALSE;
    INT nRet = ZIP_OK;

    nRet = mini_compress_file(_T("E:\\Test\\1.zip"), _T("E:\\Test\\a.dmp"), NULL, ZIP_LEVEL_5, ZIP_OPEN_EXIST);
    assert(nRet == ZIP_OK);

#ifndef UNICODE
    std::vector<std::string> vecFiles;
#else
    std::vector<std::wstring> vecFiles;
#endif
    vecFiles.push_back(_T("E:\\Test\\a.dmp"));
    vecFiles.push_back(_T("E:\\Test\\b.dmp"));
    vecFiles.push_back(_T("E:\\Test\\c\\c.dmp"));
    nRet = mini_compress_files(_T("E:\\Test\\multi-files-vec.zip"), vecFiles, _T("E:\\Test"), NULL, ZIP_LEVEL_5, ZIP_CREATE_ALWAYS);
    assert(nRet == ZIP_OK);

    nRet = mini_compress_dir(_T("E:\\Test\\dir.zip"), _T("E:\\Test\\dir_"), _T("E:\\Test"), szPsw, ZIP_LEVEL_5, ZIP_CREATE_ALWAYS);
    assert(nRet == ZIP_OK);
}

void test_unzip()
{
    TCHAR szZipFile[MAX_PATH] = _T("E:\\Test\\dir.zip");
    TCHAR szOutDir[MAX_PATH] = _T("D:\\Test\\temp");
    TCHAR szPsw[MAX_PATH] = _T("abc");
    TCHAR szPswWrong[MAX_PATH] = _T("adc");

    INT nRet = mini_uncompress_file(szZipFile, szOutDir, NULL);
    DelDir(szOutDir);
    assert(nRet == ZIP_OK);
}

void test_unzip_w()
{
    WCHAR szZipFile[MAX_PATH] = L"E:\\test\\dir.zip";
    WCHAR szOutDir[MAX_PATH] = L"E:\\test";
    WCHAR szPsw[MAX_PATH] = L"abc";
    WCHAR szPswWrong[MAX_PATH] = L"adc";

    INT nRet = miniunzip::uncompressw(szZipFile, szOutDir, szPsw);
    assert(nRet == ZIP_OK);
}

BOOL DelDir(LPCTSTR pszDir)
{
    if(!pszDir)
    {
        return FALSE;
    }

    if(!PathFileExists(pszDir))
    {
        return TRUE;
    }

    BOOL bRet = FALSE;

    TCHAR szSearch[MAX_PATH] = {0};
    _tcscpy_s(szSearch, MAX_PATH, pszDir);
    PathAddBackslash(szSearch);
    PathAppend(szSearch, _T("*.*"));

    TCHAR szTmp[MAX_PATH] = {0};

    WIN32_FIND_DATA data;
    HANDLE hFind = FindFirstFile(szSearch, &data);
    while(hFind != INVALID_HANDLE_VALUE)
    {
        do 
        {
            if(_tcscmp(data.cFileName, _T(".")) == 0 || _tcscmp(data.cFileName, _T("..")) == 0)
            {
                break;
            }

            _tcscpy_s(szTmp, MAX_PATH, pszDir);
            PathAddBackslash(szTmp);
            PathAppend(szTmp, data.cFileName);

            if(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                bRet = DelDir(szTmp);
            }
            else
            {
                bRet = DeleteFile(szTmp);
            }
        } while (FALSE);

        if(!FindNextFile(hFind, &data))
        {
            break;
        }
    }
    FindClose(hFind);

    bRet = RemoveDirectory(pszDir);
    if(!bRet)
    {
        DWORD dwErrCode = GetLastError();
        return bRet;
    }
    return bRet;
}

int _tmain(int argc, _TCHAR* argv[])
{
    //test_zip();

    test_unzip();
    //test_unzip_w();
    
	return 0;
}

