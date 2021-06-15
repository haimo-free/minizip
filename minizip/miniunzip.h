/*******************************************************************************
    @file:      miniunzip.h
    @author:    Haimo Lei
    @date:      2013/07/20
    @version:   1.0.0.1001
    
    @brief:     uncompress file
    
    Copyright(C) 2013, Haimo Lei. All Rights Reserved.
*******************************************************************************/

#ifndef _MINIUNZIP_H_20130720_
#define _MINIUNZIP_H_20130720_

#include "minidef.h"

#ifndef UNICODE
#define mini_uncompress_file      miniunzip::uncompress
#else
#define mini_uncompress_file      miniunzip::uncompressw
#endif

class miniunzip
{
public:
    //******************************************************************************
    // Method:    uncompress
    // FullName:  miniunzip::uncompress
    // Access:    public 
    // Returns:   int
    // Brief:     uncompress file
    // Qualifier:
    // Parameter: const char * strzip_file
    //              the original zip file
    // Parameter: const char * strdir
    //              the directory will file saved
    // Parameter: const char * strpsw
    //              if the zip is crypt, you should enter the correct password
    //******************************************************************************
    static int uncompress(const char* strzip_file, const char* strdir, const char* strpsw);
    static int uncompressw(const wchar* strzip_file, const wchar* strdir, const wchar* strpsw);

protected:
    static int internal_uncompress(unzFile pUnzFile, const char* strdir, const char* strpsw);
    static int internal_uncompress_current_dir_file(unzFile pUnzFile, const char* strdir, const char* strpsw);

    static int internal_uncompress_current_dir(unzFile pUnzFile, const char* strdir_inzip, unz_file_info64* pfile_info);
    static int internal_uncompress_current_file(unzFile pUnzFile, const char* strfile_inzip, unz_file_info64* pfile_info, const char* strpsw);

    static int write_zip_to_file(unzFile pUnzFile, const char* strfile_inzip, unz_file_info64* pfile_info);
};

#endif