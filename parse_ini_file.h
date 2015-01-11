//
//  ini.h
//  ini
//
//  Created by 易国磐 on 15-1-9.
//  Copyright (c) 2015年 易国磐. All rights reserved.
//

#ifndef ini_ini_h
#define ini_ini_h

typedef struct _ini
{
    char key[32];
    struct _ini *next;
    struct _ini_params *ini_params_t;
} ini;

typedef struct _ini_params
{
    char key[32];
    char value[128];
    struct _ini_params *next;
} ini_params;
#endif
