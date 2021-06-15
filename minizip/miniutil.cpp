#include "StdAfx.h"
#include "miniutil.h"

bool mini_util::check_file_exist( const char* strfile )
{
    if(!strfile)
        return false;

    bool bExist = false;

    FILE* file = NULL;
    int result = fopen_s(&file, strfile, "rb");
    if(result == 0 && file)
    {
        fclose(file);
        file = NULL;
        
        bExist = true;
    }

    return bExist;
}

bool mini_util::makesure_dir_exist( const char* strdir )
{
    if(!strdir)
        return false;

    int result = m_mkdir(strdir);

    return (result == 0 || errno == EEXIST) ? true : false;
}

bool mini_util::m_mkdir(const char * FilePath)
{
	int bRet = FALSE;
	if (NULL == FilePath)
	{
		return bRet;
	}
	char szFilePath[MAX_PATH] = {0};
	strcpy_s(szFilePath, MAX_PATH, FilePath);

	char * pTmp = szFilePath;
	while (pTmp != '\0')
	{
		std::string strFilePath;
		char *pDivision = strchr(pTmp, '\\');
		if (!pDivision || pDivision <= pTmp)
		{
			break;
		}

		strFilePath = szFilePath;
		strFilePath[pDivision-szFilePath] = '\0';
		if (!PathFileExistsA(strFilePath.c_str()))
		{
			bRet = _mkdir(strFilePath.c_str());
		}

		pTmp = pDivision + 1;
	}
	if (!PathFileExistsA(FilePath))
	{
		bRet = _mkdir(FilePath);
	}

	return bRet;
}

bool mini_util::is_large_file( const char* strfile )
{
    if(!strfile)
        return false;

    bool result = false;

    FILE* file = NULL;
    int result_fopen = fopen_s(&file, strfile, "rb");
    if(result_fopen == 0 && file)
    {
        fseeko64(file, 0, SEEK_END);
        ZPOS64_T nLen = ftello64(file);

        if(nLen >= 0xffffffff)
        {
            result = true;
        }

        fclose(file);
        file = NULL;
    }

    return result;
}

bool mini_util::get_dir_file_time( const char* strpath, bool dir, unsigned long* pdostime )
{
    if(!strpath || !pdostime)
        return false;

    char* search = (char*)strpath;
    if(dir)
    {
        search = new char[MAX_PATH];
        if(!search)
        {
            return false;
        }

        strcpy_s(search, MAX_PATH, strpath);
        strcat_s(search, MAX_PATH, "\\*.*");
    }

    bool result = false;

    WIN32_FIND_DATAA finddata;    
    HANDLE hFind = FindFirstFileA(search, &finddata);
    if(hFind != INVALID_HANDLE_VALUE)
    {
        FILETIME ftLocal;
        FileTimeToLocalFileTime(&(finddata.ftLastWriteTime), &ftLocal);

        if(pdostime)
        {
            FileTimeToDosDateTime(&ftLocal, ((LPWORD)pdostime) + 1, ((LPWORD)pdostime) + 0);
        }

        FindClose(hFind);
        hFind = NULL;

        result = true;
    }

    if(dir)
    {
        delete[] search;
        search = NULL;
    }

    return result;
}

bool mini_util::set_dir_file_time( const char* strpath, unsigned long dostime )
{
    if(!strpath)
        return false;

    HANDLE hFile = CreateFileA(strpath, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
    if(hFile == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    FILETIME ftLocal, ftm;
    FILETIME ftCreate, ftLastAcc, ftLastWrite;
    GetFileTime(hFile, &ftCreate, &ftLastAcc, &ftLastWrite);
    DosDateTimeToFileTime((WORD)(dostime>>16), (WORD)dostime, &ftLocal);
    LocalFileTimeToFileTime(&ftLocal, &ftm);
    SetFileTime(hFile, &ftm, &ftLastAcc, &ftm);

    CloseHandle(hFile);
    hFile = NULL;

    return true;
}

bool mini_util::get_file_crc32( const char* strfile, unsigned long* pcrc32 )
{
    if(!strfile || !pcrc32)
        return false;

    bool result = false;
    unsigned long ucrc32 = 0;

    FILE* file = NULL;
    void* szbuffer = NULL;

    do 
    {
        int result_fopen = fopen_s(&file, strfile, "rb");
        if(result_fopen != 0 || !file)
        {
            break;
        }

        int buffer_len = MINIZIP_MAX_BUFFER_LEN;
        szbuffer = (void*)malloc(buffer_len);
        if(!szbuffer)
        {
            break;
        }

        int read_len = 0;
        int total_len = 0;
        bool result_inner = true;

        do 
        {
            read_len = (int)fread(szbuffer, 1, buffer_len, file);
            if(read_len < buffer_len && !feof(file))
            {
                result_inner = false;
                break;
            }

            if(read_len > 0)
            {
                ucrc32 = crc32(ucrc32, (byte*)szbuffer, read_len);
            }

            total_len += read_len;

        } while (read_len > 0);

        result = result_inner;
    } while (false);

    if(szbuffer)
    {
        delete[] szbuffer;
        szbuffer = NULL;
    }

    if(file)
    {
        fclose(file);
        file = NULL;
    }

    if(result)
    {
        *pcrc32 = ucrc32;
    }

    return result;    
}

bool mini_util::list_files_in_dir( const char* strdir, std::vector<path_file_data>& vec_files )
{
    if(!strdir)
        return false;

    char szsearch[MAX_PATH] = {0};
    if(!cat_file_to_dir(strdir, "*.*", szsearch))
    {
        return false;
    }    

    bool result = true;
    char sztmp[MAX_PATH] = {0};

    WIN32_FIND_DATAA finddata;
    HANDLE hFind = FindFirstFileA(szsearch, &finddata);
    while(hFind)
    {
        if(strcmp(finddata.cFileName, ".") != 0 && strcmp(finddata.cFileName, "..") != 0)
        {
            if(mini_util::cat_file_to_dir(strdir, finddata.cFileName, sztmp))
            {
                path_file_data data((finddata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ? true : false, sztmp);
                vec_files.push_back(data);
            }
            else
            {
                result = false;
                break;
            }
        }

        if(!FindNextFileA(hFind, &finddata))
        {
            break;
        }
    }

    if(hFind)
    {
        FindClose(hFind);
        hFind = NULL;
    }

    return result;
}

bool mini_util::cat_file_to_dir( const char* strdir, const char* strfile, char* strdst )
{
    if(!strdst || !strdir || !strfile)
        return false;

    int dir_len = strlen(strdir);
    int file_len = strlen(strfile);
    int backslash = 0;

    if(dir_len == 0 || strcmp(&strdst[dir_len - 1], "\\") != 0)
    {
        backslash = 1;
    }
    if(dir_len + file_len + backslash >= MAX_PATH)
    {
        return false;
    }

    ZeroMemory(strdst, MAX_PATH * sizeof(char));
    strcpy_s(strdst, MAX_PATH, strdir);
    if(backslash)
    {
        strcat_s(strdst, MAX_PATH, "\\");
    }
    strcpy_s(strdst + dir_len + backslash, MAX_PATH - dir_len - backslash, strfile);
    
    return true;
}

bool mini_util::convert_unicode_to_ansi( const wchar* strsrc, std::string& strout )
{
    strout = "";

    if(!strsrc)
    {
        return true;
    }

    bool result = false;
    char* buffer = NULL;

    do 
    {
        int ansi_len = ::WideCharToMultiByte(CP_ACP, NULL, strsrc, wcslen(strsrc), NULL, NULL, NULL, NULL);

        char* buffer = new char[ansi_len + 1];
        if(!buffer)
        {
            break;
        }

        memset(buffer, 0 , ansi_len);

        if (ansi_len != ::WideCharToMultiByte(CP_ACP, NULL, strsrc, wcslen(strsrc), buffer, ansi_len, NULL, NULL))
        {
            break;
        }

        buffer[ansi_len] = '\0';
        strout = buffer;
        result = true;
    } while (false);

    if(buffer)
    {
        delete[] buffer;
        buffer = NULL;
    }

    return result;
}

bool mini_util::is_directory(const char* filename)
{
	DWORD dwAttr = ::GetFileAttributesA(filename);  

	// file not exist
	if (dwAttr == 0xFFFFFFFF)    
		return false;

	else if (dwAttr & FILE_ATTRIBUTE_DIRECTORY) 
		return true; 
	else
		return false;
}