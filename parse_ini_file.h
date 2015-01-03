//
//  parse_ini_file.h
//  paser_ini_file
//
//  Created by 易国磐 on 15-1-3.
//  Copyright (c) 2015年 易国磐. All rights reserved.
//

#ifndef __paser_ini_file__parse_ini_file__
#define __paser_ini_file__parse_ini_file__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

typedef struct _ini {
    char key[50];
    char value[255];
    struct _ini *next;
} ini;

char **str_split(char *src, size_t strlen, const char *delm, size_t delmlen);
ini *make_ini(const char *key, const char *value);
void ini_set(ini **init, ini *i);
ini *parse_ini_file(const char *filename);
char *ini_get(ini *init, char *key);

#endif /* defined(__paser_ini_file__parse_ini_file__) */
