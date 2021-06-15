/*******************************************************************************
    @file:      minidef.h
    @author:    Haimo Lei
    @date:      2013/07/20
    @version:   1.0.0.1001
    
    @brief:     common define
    
    Copyright(C) 2013, Haimo Lei. All Rights Reserved.
*******************************************************************************/

#ifndef _MINIDEF_H_20130720_
#define _MINIDEF_H_20130720_

#pragma once

#include <tchar.h>
#include <assert.h>
#include <direct.h>

#include <string>
#include <vector>

#ifndef ZLIB_WINAPI
#define ZLIB_WINAPI
#endif

#include "zip.h"
#include "unzip.h"

#define wchar                       WCHAR
#define MINIZIP_MAX_BUFFER_LEN      (1024 * 4)

// zip file create or open flag
enum
{
    ZIP_CREATE_ALWAYS       = 1,
    ZIP_OPEN_EXIST          = (1<<1),
};

// extend zip error define
enum
{
    ZIP_PARAM_ERROR         = -200,
    ZIP_FILE_ERROR          = -201,
    ZIP_NO_MEMORY           = -202,
};

// zip compress level
enum
{
    ZIP_LEVEL_0             = 0,
    ZIP_LEVEL_1,
    ZIP_LEVEL_2,
    ZIP_LEVEL_3,
    ZIP_LEVEL_4,
    ZIP_LEVEL_5,
    ZIP_LEVEL_6,
    ZIP_LEVEL_7,
    ZIP_LEVEL_8,
    ZIP_LEVEL_9,
};

// zip compress level
enum
{
    ZIP_LEVEL_STORE_ONLY    = ZIP_LEVEL_0,
    ZIP_LEVEL_FASTER        = ZIP_LEVEL_1,
    ZIP_LEVEL_BETTER        = ZIP_LEVEL_9,
};

typedef struct _path_file_data
{
    bool    is_dir;
    char    path_or_file[MAX_PATH];

    _path_file_data(bool bDir, const char* pszPathOrFile)
    {
        is_dir = bDir;
        strcpy_s(path_or_file, MAX_PATH, pszPathOrFile);
    }
}path_file_data;

#endif