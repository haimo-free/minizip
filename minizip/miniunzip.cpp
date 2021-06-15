#include "StdAfx.h"
#include "miniunzip.h"
#include "miniutil.h"

int miniunzip::uncompress( const char* strzip_file, const char* strdir, const char* strpsw )
{
    if(!strzip_file || !strdir)
    {
        return ZIP_PARAM_ERROR;
    }

    char szzip_file[MAX_PATH] = {0};
    strcpy_s(szzip_file, MAX_PATH, strzip_file);

    // open zip file for uncompress
    unzFile unzfile = NULL;
	unzfile = unzOpen64(szzip_file);
    if(!unzfile)
    {
        strcat_s(szzip_file, MAX_PATH, ".zip");
        unzfile = unzOpen64(szzip_file);
    }
    if(!unzfile)
    {
        return Z_ERRNO;
    }

    int result = Z_ERRNO;

    // create unzip path and uncompress
    if(mini_util::makesure_dir_exist(strdir)/* && (_chdir(strdir) == 0)*/)
    {
        result = internal_uncompress(unzfile, strdir, strpsw);
    }

    int result_close = unzClose(unzfile);
    unzfile = NULL;

    result = (result == ZIP_OK ? result_close : result); 
    return result;
}

int miniunzip::uncompressw( const wchar* strzip_file, const wchar* strdir, const wchar* strpsw )
{
    std::string strzip_file_a;
    std::string strdir_a;
    std::string strpsw_a;

    int result = ZIP_ERRNO;
    bool result_convert = false;

    do 
    {
        if(!mini_util::convert_unicode_to_ansi(strzip_file, strzip_file_a))
        {
            break;
        }

        if(!mini_util::convert_unicode_to_ansi(strdir, strdir_a))
        {
            break;
        }

        if(!mini_util::convert_unicode_to_ansi(strpsw, strpsw_a))
        {
            break;
        }

        result_convert = true;

    } while (false);

    if(result_convert)
    {
        result = uncompress(strzip_file ? strzip_file_a.c_str() : NULL, strdir ? strdir_a.c_str() : NULL, strpsw ? strpsw_a.c_str() : NULL);
    }

    return result;
}

int miniunzip::internal_uncompress( unzFile pUnzFile, const char* strdir, const char* strpsw )
{
    if(!pUnzFile)
    {
        return ZIP_PARAM_ERROR;
    }

    int result = ZIP_OK;

    unz_global_info64 unzinfo;
    result = unzGetGlobalInfo64(pUnzFile, &unzinfo);
    if(result != ZIP_OK)
    {
        return result;
    }

    for(int i = 0; i < unzinfo.number_entry; i++)
    {
        result = internal_uncompress_current_dir_file(pUnzFile, strdir, strpsw);
        if(result != ZIP_OK)
        {
            break;
        }

        if((i + 1) < unzinfo.number_entry)
        {
            result = unzGoToNextFile(pUnzFile);
            if(result != ZIP_OK)
            {
                break;
            }
        }
    }

    return result;
}

int miniunzip::internal_uncompress_current_dir_file( unzFile pUnzFile, const char* strdir, const char* strpsw )
{
    if(!pUnzFile)
    {
        return ZIP_PARAM_ERROR;
    }

    int result = ZIP_OK;

	BOOL bDirOrFile = FALSE;
    char szpath_inzip[MAX_PATH] = {0};
	char szpath_outzipTemp[MAX_PATH] = {0};
    unz_file_info64 file_info;

    // get current file info
    result = unzGetCurrentFileInfo64(pUnzFile, &file_info, szpath_inzip, MAX_PATH, NULL, 0, NULL, 0);
    if(result != ZIP_OK)
    {
        return result;
    }
	char* szpath_inzipTmp = szpath_inzip;
	char* pDivision = strrchr(szpath_inzipTmp, '/');
	if (pDivision && pDivision > szpath_inzipTmp)
	{
		bDirOrFile = TRUE;
		strcpy_s(szpath_outzipTemp, MAX_PATH, szpath_inzipTmp);
		szpath_outzipTemp[pDivision-szpath_inzipTmp] = '\0';
	}

    // get output file name
	char szdir[MAX_PATH] = {0};
	strcpy_s(szdir, MAX_PATH, strdir);
	strcat_s(szdir, MAX_PATH, "\\");
	strcat_s(szdir, MAX_PATH, szpath_outzipTemp);
	strcpy_s(szpath_outzipTemp, MAX_PATH, szdir);

	char * pszTmp = szpath_inzip;
	while((*pszTmp) != '\0')
	{
		if (((*pszTmp) == '/'))
		{
			((*pszTmp) = '\\');
		}

		pszTmp++;
	}

	strcpy_s(szdir, MAX_PATH, strdir);
	strcat_s(szdir, MAX_PATH, "\\");
	strcat_s(szdir, MAX_PATH, szpath_inzipTmp);
	strcpy_s(szpath_inzipTmp, MAX_PATH, szdir);
	::PathRemoveBackslashA(szpath_inzipTmp);

	if (bDirOrFile && !::PathFileExistsA(szpath_outzipTemp))
    {
        result = internal_uncompress_current_dir(pUnzFile, szpath_outzipTemp, &file_info);
    }

	if (0 == result && !mini_util::is_directory(szpath_inzipTmp))
    {
        result = internal_uncompress_current_file(pUnzFile, szpath_inzipTmp, &file_info, strpsw);
    }

    return result;
}

int miniunzip::internal_uncompress_current_dir( unzFile pUnzFile, const char* strdir_inzip, unz_file_info64* pfile_info )
{
    if(!pUnzFile || !strdir_inzip || !pfile_info)
    {
        return ZIP_PARAM_ERROR;
    }

    int result = mini_util::makesure_dir_exist(strdir_inzip) ? ZIP_OK : ZIP_FILE_ERROR;
    if(result == ZIP_OK)
    {
        result = mini_util::set_dir_file_time(strdir_inzip, pfile_info->dosDate) ? ZIP_OK : ZIP_FILE_ERROR;
    }

    return result;
}

int miniunzip::internal_uncompress_current_file( unzFile pUnzFile, const char* strfile_inzip, unz_file_info64* pfile_info, const char* strpsw )
{
    if(!pUnzFile || !strfile_inzip || !pfile_info)
    {
        return ZIP_PARAM_ERROR;
    }

    int result = unzOpenCurrentFilePassword(pUnzFile, strpsw);
    if(result != ZIP_OK)
    {
        return result;
    }

    result = write_zip_to_file(pUnzFile, strfile_inzip, pfile_info);

    int result_close = unzCloseCurrentFile(pUnzFile);
    result = (result == ZIP_OK ? result_close : result); 

    return result;
}

int miniunzip::write_zip_to_file( unzFile pUnzFile, const char* strfile_inzip, unz_file_info64* pfile_info )
{
    assert(pUnzFile && strfile_inzip && pfile_info);

    int result = ZIP_FILE_ERROR;

    FILE* fout = NULL;
    void* szbuffer = NULL;

    do 
    {
        result = fopen_s(&fout, strfile_inzip, "wb");
        if(result != 0 || !fout)
        {
            break;
        }

        int buffer_len = MINIZIP_MAX_BUFFER_LEN;
        szbuffer = (void*)malloc(buffer_len);
        if(!szbuffer)
        {
            result = ZIP_NO_MEMORY;
            break;
        }

        int read_size = 0;
        BOOL result_read_write = true;
        do 
        {
            read_size = unzReadCurrentFile(pUnzFile, szbuffer, buffer_len);
            if(read_size < 0)
            {
                result_read_write = false;
                break;
            }

            if(read_size > 0)
            {
                if(fwrite(szbuffer, read_size, 1, fout) != 1)
                {
                    result_read_write = false;
                    break;
                }
            }
        } while (read_size > 0);

        if(fout)
        {
            fclose(fout);
            fout = NULL;
        }

        if(result_read_write)
        {
            result = mini_util::set_dir_file_time(strfile_inzip, pfile_info->dosDate) ? ZIP_OK : ZIP_FILE_ERROR;
        }

    } while (false);

    if(fout)
    {
        fclose(fout);
        fout = NULL;
    }

    if(szbuffer)
    {
        free(szbuffer);
        szbuffer = NULL;
    }

    return result;
}
