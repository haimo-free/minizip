/*******************************************************************************
    @file:      minizip.h
    @author:    Haimo Lei
    @date:      2013/07/20
    @version:   1.0.0.1001
    
    @brief:     compress file or directory
    
    Copyright(C) 2013, Haimo Lei. All Rights Reserved.
*******************************************************************************/

#ifndef _MINIZIP_H_20130720_
#define _MINIZIP_H_20130720_

#pragma once

#include "minidef.h"

#ifndef UNICODE
#define mini_compress_file      minizip::compress_file
#define mini_compress_files     minizip::compress_files
#define mini_compress_dir       minizip::compress_dir
#else
#define mini_compress_file      minizip::compress_filew
#define mini_compress_files     minizip::compress_filesw
#define mini_compress_dir       minizip::compress_dirw
#endif

class minizip
{
public:
    //******************************************************************************
    // Method:    compress_file
    // FullName:  minizip::compress_file
    // Access:    public static 
    // Returns:   int
    // Brief:     compress single file
    // Qualifier:
    // Parameter: const char * strzip_file
    //              the target zip file, must valid file.
    // Parameter: const char * strfile
    //              the original file, must valid file.
    // Parameter: const char * strpsw
    //              the password for target zip file, could be set null for default.
    // Parameter: int level
    //              compress level, from ZIP_LEVEL_0 to ZIP_LEVEL_9
    // Parameter: int flag
    //              zip file create or open flag, ZIP_CREATE_ALWAYS or ZIP_OPEN_EXIST
    //******************************************************************************
    static int compress_file(const char* strzip_file, const char* strfile, const char* strpsw, int level, int flag);
    static int compress_filew(const wchar* strzip_file, const wchar* strfile, const wchar* strpsw, int level, int flag);

    //******************************************************************************
    // Method:    compress_files
    // FullName:  minizip::compress_files
    // Access:    public static 
    // Returns:   int
    // Brief:     compress some files, not include folder
    // Qualifier:
    // Parameter: const char * strzip_file
    // Parameter: const std::vector<std::string> & vecfiles
    //              files for compress
    // Parameter: const char * strbase_dir
    //              this base_dir will be delete when files compressed into zip file
    // Parameter: const char * strpsw
    // Parameter: int level
    // Parameter: int flag
    //******************************************************************************
    static int compress_files(const char* strzip_file, const std::vector<std::string>& vecfiles, const char* strbase_dir, const char* strpsw, int level, int flag);
    static int compress_filesw(const wchar* strzip_file, const std::vector<std::wstring>& vecfiles, const wchar* strbase_dir, const wchar* strpsw, int level, int flag);

    //******************************************************************************
    // Method:    compress_dir
    // FullName:  minizip::compress_dir
    // Access:    public 
    // Returns:   int
    // Brief:     compress directory
    // Qualifier:
    // Parameter: const char * strzip_file
    // Parameter: const char * strdir
    // Parameter: const char * strbase_dir
    // Parameter: const char * strpsw
    // Parameter: int level
    // Parameter: int flag
    //******************************************************************************
    static int compress_dir(const char* strzip_file, const char* strdir, const char* strbase_dir, const char* strpsw, int level, int flag);
    static int compress_dirw(const wchar* strzip_file, const wchar* strdir, const wchar* strbase_dir, const wchar* strpsw, int level, int flag);

protected:
    static int internal_compress_file(zipFile pZipFile, const char* strfile, bool dir, const char* strbase_dir, const char* strpsw, int level);
    static int internal_compress_dir(zipFile pZipFile, const char* strdir, const char* strbase_dir, const char* strpsw, int level);

protected:
    static int check_param(const char* strzip_file, int& level, int& flag);
    static int flag_to_append(int flag);
    static bool parse_filename_inzip(const char* strfile, bool dir, const char* strbase_dir, char** pstrfilename_inzip);
    static int write_file_to_zip(zipFile pZipFile, const char* strfile);
};

#endif