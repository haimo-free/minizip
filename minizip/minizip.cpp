#include "StdAfx.h"
#include "minizip.h"
#include "miniutil.h"

int minizip::compress_file( const char* strzipfile, const char* strfile, const char* strpsw, int level, int flag )
{
    if(!strzipfile || !strfile)
    {
        return ZIP_PARAM_ERROR;
    }

    char szZipFile[MAX_PATH] = {0};
    strcpy_s(szZipFile, MAX_PATH, strzipfile);

    // if zip file has no extension, then add .zip
    if(!strstr(szZipFile, "."))
    {
        strcat_s(szZipFile, MAX_PATH, ".zip");
    }

    int result = ZIP_OK;

    // check param
    result = check_param(szZipFile, level, flag);
    if(result != ZIP_OK)
    {
        return result;
    }

    // open zip file for compress
    zipFile zipfile = zipOpen64(szZipFile, flag_to_append(flag));
    if(!zipfile)
    {
        return Z_ERRNO;
    }

    // compress file to zip
    result = internal_compress_file(zipfile, strfile, false, NULL, strpsw, level);

    int result_close = zipClose(zipfile, NULL);
    zipfile = NULL;

    result = (result == ZIP_OK ? result_close : result);
    return result;
}

int minizip::compress_filew( const wchar* strzip_file, const wchar* strfile, const wchar* strpsw, int level, int flag )
{
    std::string strzip_file_a;
    std::string strfile_a;
    std::string strpsw_a;

    int result = ZIP_ERRNO;
    int result_convert = false;

    do 
    {
        if(!mini_util::convert_unicode_to_ansi(strzip_file, strzip_file_a))
        {
            break;
        }

        if(!mini_util::convert_unicode_to_ansi(strfile, strfile_a))
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
        result = compress_file(strzip_file ? strzip_file_a.c_str() : NULL, strfile ? strfile_a.c_str() : NULL, strpsw ? strpsw_a.c_str() : NULL, level, flag);
    }

    return result;
}

int minizip::compress_files( const char* strzip_file, const std::vector<std::string>& vecfiles, const char* strbase_file, const char* strpsw, int level, int flag )
{
    if(!strzip_file || vecfiles.size() <= 0)
    {
        return ZIP_PARAM_ERROR;
    }

    char szzip_file[MAX_PATH] = {0};
    strcpy_s(szzip_file, MAX_PATH, strzip_file);

    // if zip file has no extension, then add .zip
    if(!strstr(szzip_file, "."))
    {
        strcat_s(szzip_file, MAX_PATH, ".zip");
    }

    int result = ZIP_ERRNO;

    // check param
    result = check_param(szzip_file, level, flag);
    if(result != ZIP_OK)
    {
        return result;
    }

    // open zip file for compress
    zipFile zipfile = zipOpen64(szzip_file, flag_to_append(flag));
    if(!zipfile)
    {
        return Z_ERRNO;
    }

    // compress file to zip one by one
    for(std::vector<std::string>::const_iterator itor = vecfiles.begin(); itor != vecfiles.end(); ++itor)
    {
        if(!mini_util::check_file_exist(itor->c_str()))
        {
            continue;
        }

        result = internal_compress_file(zipfile, itor->c_str(), false, strbase_file, strpsw, level);
        if(result != ZIP_OK)
        {
            break;
        }
    }

    int result_close = zipClose(zipfile, NULL);
    zipfile = NULL;

    result = (result == ZIP_OK ? result_close : result);
    return result;
}

int minizip::compress_filesw( const wchar* strzip_file, const std::vector<std::wstring>& vecfiles, const wchar* strbase_dir, const wchar* strpsw, int level, int flag )
{
    if(!strzip_file)
        return ZIP_PARAM_ERROR;

    std::string strzip_file_a;
    std::vector<std::string> vecfiles_a;
    std::string strbase_dir_a;
    std::string strpsw_a;

    int result = ZIP_ERRNO;
    int result_convert = false;

    do 
    {
        if(!mini_util::convert_unicode_to_ansi(strzip_file, strzip_file_a))
        {
            break;
        }

        std::string strfile;
        for(std::vector<std::wstring>::const_iterator itor = vecfiles.begin(); itor != vecfiles.end(); ++itor)
        {
            if(!mini_util::convert_unicode_to_ansi(itor->c_str(), strfile))
            {
                break;
            }

            vecfiles_a.push_back(strfile);
        }
        if(vecfiles.size() != vecfiles_a.size())
        {
            break;
        }

        if(!mini_util::convert_unicode_to_ansi(strbase_dir, strbase_dir_a))
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
        result = compress_files(strzip_file_a.c_str(), vecfiles_a, strbase_dir ? strbase_dir_a.c_str() : NULL, strpsw ? strpsw_a.c_str() : NULL, level, flag);
    }

    return result;
}

int minizip::compress_dir( const char* strzip_file, const char* strdir, const char* strbase_dir, const char* strpsw, int level, int flag )
{
    if(!strzip_file || !strdir)
    {
        return ZIP_PARAM_ERROR;
    }

    char szzip_file[MAX_PATH] = {0};
    strcpy_s(szzip_file, MAX_PATH, strzip_file);

    // if zip file has no extension, then add .zip
    if(!strstr(szzip_file, "."))
    {
        strcat_s(szzip_file, MAX_PATH, ".zip");
    }

    int result = ZIP_OK;

    // check param
    result = check_param(szzip_file, level, flag);
    if(result != ZIP_OK)
    {
        return result;
    }

    // open zip file for compress
    zipFile zipfile = zipOpen64(szzip_file, flag_to_append(flag));
    if(!zipfile)
    {
        return Z_ERRNO;
    }

    // compress dir to zip
    result = internal_compress_dir(zipfile, strdir, strbase_dir ? strbase_dir : strdir, strpsw, level);

    int result_close = zipClose(zipfile, NULL);
    zipfile = NULL;

    result = (result == ZIP_OK ? result_close : result);
    return result;
}

int minizip::compress_dirw( const wchar* strzip_file, const wchar* strdir, const wchar* strbase_dir, const wchar* strpsw, int level, int flag )
{
    std::string strzip_file_a;
    std::string strdir_a;
    std::string strbase_dir_a;
    std::string strpsw_a;

    int result = ZIP_ERRNO;
    int result_convert = false;

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

        if(!mini_util::convert_unicode_to_ansi(strbase_dir, strbase_dir_a))
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
        result = compress_dir(strzip_file ? strzip_file_a.c_str() : NULL, strdir ? strdir_a.c_str() : NULL, strbase_dir ? strbase_dir_a.c_str() : NULL, 
            strpsw ? strpsw_a.c_str() : NULL, level, flag);
    }

    return result;
}

int minizip::internal_compress_file( zipFile pZipFile, const char* strfile, bool dir, const char* strbase_dir, const char* strpsw, int level )
{
    if(!pZipFile || !strfile)
    {
        return ZIP_PARAM_ERROR;
    }

    assert(level >= ZIP_LEVEL_0 && level <= ZIP_LEVEL_9);

    char szfile[MAX_PATH] = {0};
    strcpy_s(szfile, MAX_PATH, strfile);
    if(dir)
    {
        // if dir, should add backslash for create in zip
        strcat_s(szfile, MAX_PATH, "/");
    }

    // get file name in zip
    const char* szfile_inzip = szfile;
    if(!parse_filename_inzip(szfile, dir, strbase_dir, (char**)&szfile_inzip))
    {
        return ZIP_PARAM_ERROR;
    }

    // get file time info
    zip_fileinfo zi;
    memset(&zi, 0, sizeof(zip_fileinfo));
    if(mini_util::get_dir_file_time(strfile, dir, &zi.dosDate) == false)
    {
        return ZIP_FILE_ERROR;
    }

    // get crc32 if needed
    unsigned long ucrc32 = 0;
    if(strpsw && !dir)
    {
        if(!mini_util::get_file_crc32(strfile, &ucrc32))
        {
            return ZIP_FILE_ERROR;
        }
    }

    // get file size info
    bool large_file = dir ? false : mini_util::is_large_file(strfile);

    // open file in zip for writing
    int result = zipOpenNewFileInZip3_64(pZipFile, szfile_inzip, &zi, NULL, 0, NULL, 0, NULL, 
        (level != ZIP_LEVEL_0) ? Z_DEFLATED : 0, level, 0, 
        -MAX_WBITS, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY, 
        strpsw, ucrc32, large_file);
    if(result != ZIP_OK)
    {
        return result;
    }

    // write source file to zip
    if(!dir)
    {
        result = write_file_to_zip(pZipFile, strfile);
    }

    int result_close = zipCloseFileInZip(pZipFile);
    result = (result == ZIP_OK ? result_close : result);

    return result;
}

int minizip::internal_compress_dir( zipFile pZipFile, const char* strdir, const char* strbase_dir, const char* strpsw, int level )
{
    if(!pZipFile || !strdir)
    {
        return ZIP_PARAM_ERROR;
    }

    assert(level >= ZIP_LEVEL_0 && level <= ZIP_LEVEL_9);

    // list all dirs or files in current dir
    std::vector<path_file_data> vec_dir_file;
    if(!mini_util::list_files_in_dir(strdir, vec_dir_file))
    {
        return ZIP_FILE_ERROR;
    }

    int result = ZIP_OK;

    // add current dir to zip
    if(strcmp(strdir, strbase_dir) != 0)
    {
        result = internal_compress_file(pZipFile, strdir, true, strbase_dir, strpsw, level);
        if(result != ZIP_OK)
        {
            return result;
        }
    }

    // add all dirs or files in current dir
    for(std::vector<path_file_data>::const_iterator itor = vec_dir_file.begin(); itor != vec_dir_file.end(); ++itor)
    {
        if(itor->is_dir)
        {
            result = internal_compress_dir(pZipFile, itor->path_or_file, strbase_dir, strpsw, level);
        }
        else
        {
            result = internal_compress_file(pZipFile, itor->path_or_file, false, strbase_dir, strpsw, level);
        }

        if(result != ZIP_OK)
        {
            break;
        }
    }

    return result;
}

int minizip::check_param( const char* strzip_file, int& level, int& flag )
{
    // check dwLeval, must be ZIP_LEVEL_0 to ZIP_LEVEL_9
    if(level < ZIP_LEVEL_0 || level > ZIP_LEVEL_9)
    {
        level = ZIP_LEVEL_FASTER;
    }

    // dwFlag can't set ZIP_OPEN_EXIST | ZIP_CREATE_ALWAYS at the same time.
    if((flag & ZIP_OPEN_EXIST) && (flag & ZIP_CREATE_ALWAYS))
    {
        return ZIP_PARAM_ERROR;
    }

    // if dwFlag set ZIP_OPEN_EXIST when the file isn't really exist, then set ZIP_CREATE_ALWAYS.
    bool bZipExist = mini_util::check_file_exist(strzip_file);
    if((flag & ZIP_OPEN_EXIST) && !bZipExist)
    {
        flag &= (~ZIP_OPEN_EXIST);
        flag |= ZIP_CREATE_ALWAYS;
    }

    // if there is no ZIP_OPEN_EXIST and no ZIP_CREATE_ALWAYS when zip file has exist, return error ZIP_HAS_EXIST.
    if(!(flag & ZIP_OPEN_EXIST) && !(flag & ZIP_CREATE_ALWAYS) && bZipExist)
    {
        return ZIP_PARAM_ERROR;
    }

    return ZIP_OK;
}

int minizip::flag_to_append( int flag )
{
    int append = APPEND_STATUS_CREATE;

    if(flag & ZIP_CREATE_ALWAYS)
    {
        append = APPEND_STATUS_CREATE;
    }
    else if(flag & ZIP_OPEN_EXIST)
    {
        append = APPEND_STATUS_ADDINZIP;
    }

    return append;
}

bool minizip::parse_filename_inzip( const char* strfile, bool dir, const char* strbase_file, char** pstrfilename_inzip )
{
    if(!strfile || !pstrfilename_inzip)
    {
        return false;
    }

    const char* pszfilename_inzip = strfile;

    // the path name saved, should not include a leading slash
    while(pszfilename_inzip && (strlen(pszfilename_inzip) > 0))
    {
        if(pszfilename_inzip[0] != '\\' && pszfilename_inzip[0] != '/')
        {
            break;
        }

        pszfilename_inzip++;
    }

    // cut off path, only store file name if strbase_file is null, otherwise, cut off the strbase_file
    if(!strbase_file)
    {
        const char* pszlastslash = NULL;

        for(const char* psztmp_ptr = pszfilename_inzip; *psztmp_ptr; psztmp_ptr++)
        {
            if(*psztmp_ptr == '\\' || *psztmp_ptr == '/')
            {
                pszlastslash = psztmp_ptr;
            }
        }

        if(pszlastslash)
        {
            pszfilename_inzip = pszlastslash + 1;
        }
    }
    else
    {
        if(strstr(pszfilename_inzip, strbase_file) != &pszfilename_inzip[0] || strlen(pszfilename_inzip) < strlen(strbase_file))
        {
            return false;
        }

        pszfilename_inzip += strlen(strbase_file);

        while(pszfilename_inzip && (strlen(pszfilename_inzip) > 0))
        {
            if(pszfilename_inzip[0] != '\\' && pszfilename_inzip[0] != '/')
            {
                break;
            }

            pszfilename_inzip++;
        }
    }

    *pstrfilename_inzip = (char*)pszfilename_inzip;

    return true;
}

int minizip::write_file_to_zip( zipFile pZipFile, const char* strfile )
{
    assert(pZipFile && strfile);

    int result = ZIP_FILE_ERROR;

    FILE* fin = NULL;
    void* szbuffer = NULL;

    do 
    {
        result = fopen_s(&fin, strfile, "rb");
        if(result != 0 || !fin)
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
        bool result_read_write = true;
        do 
        {
            read_size = (int)fread(szbuffer, 1, buffer_len, fin);
            if(read_size < buffer_len && !feof(fin))
            {
                result_read_write = false;
                break;
            }

            if(read_size > 0)
            {
                result = zipWriteInFileInZip(pZipFile, szbuffer, read_size);
                if(result != ZIP_OK)
                {
                    result_read_write = false;
                    break;
                }
            }
        } while (read_size > 0);

        result = (result_read_write ? ZIP_OK : ZIP_FILE_ERROR);

    } while (false);

    if(szbuffer)
    {
        free(szbuffer);
        szbuffer = NULL;
    }

    if(fin)
    {
        fclose(fin);
        fin = NULL;
    }

    return result;
}
