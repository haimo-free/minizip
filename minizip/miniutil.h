/*******************************************************************************
    @file:      miniutil.h
    @author:    Haimo Lei
    @date:      2013/07/20
    @version:   1.0.0.1001
    
    @brief:     minidump util method
    
    Copyright(C) 2013, Haimo Lei. All Rights Reserved.
*******************************************************************************/

#ifndef _MINIUTIL_H_20130720_
#define _MINIUTIL_H_20130720_

#include "minidef.h"

namespace mini_util
{
    bool check_file_exist(const char* strfile);
    bool makesure_dir_exist(const char* strdir);
	bool m_mkdir(const char * FilePath);
	bool is_directory(const char* filename);

    bool is_large_file(const char* strfile);

    bool get_dir_file_time(const char* strpath, bool dir, unsigned long* pdostime);
    bool set_dir_file_time(const char* strpath, unsigned long dostime);

    bool get_file_crc32(const char* strfile, unsigned long* pcrc32);

    bool list_files_in_dir(const char* strdir, std::vector<path_file_data>& vec_files);

    bool cat_file_to_dir(const char* strdir, const char* strfile, char* strdst);

    bool convert_unicode_to_ansi(const wchar* strsrc, std::string& strout);
};

#endif