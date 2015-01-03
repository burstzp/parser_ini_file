//
//  parse_ini_file.c
//  paser_ini_file
//
//  Created by 易国磐 on 15-1-3.
//  Copyright (c) 2015年 易国磐. All rights reserved.
//

#include "parse_ini_file.h"

/***
 * 根据分割符，切分字符串
 *
 * @param char*     str
 * @param size_t    strlen
 * @param char*     delm
 * @param size_t    delmlen
 *
 * @return char **
 */
char **str_split(char *src, size_t strlen, const char *delm, size_t delmlen)
{
    // 需要修改字符串，提前复制数据源
    char *str = (char *)malloc(sizeof(char) * strlen + 1);
    memcpy(str, src, strlen);
    int i = 0;
    int c = 0;
    for (; i <  strlen; ++i) {
        if (memcmp(str+i, delm, delmlen) == 0) {
            ++c;
        }
    }
    
    if (c == 0) {
        return NULL;
    }
    
    char **res = (char **)malloc(sizeof(char*) * c);
    res[0] = str;
    int j;
    for (j=1, i = 0; i < strlen; i++) {
        if (memcmp(str+i, delm, delmlen) == 0) {
            res[j-1][i] = '\0';
            res[j++] = str + i + delmlen;
        }
    }
    *(str + i) = '\0';
    return res;
}


ini *make_ini(const char *key, const char *value)
{
    ini *init = (ini *)malloc(sizeof(ini));
    strcpy(init->key, key);
    strcpy(init->value, value);
    
    init->next = NULL;
    return init;
}

void ini_set(ini **init, ini *i)
{
    i->next = *init;
    *init = i;
}

ini *parse_ini_file(const char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "read ini file error", errno);
        return NULL;
    }
    char line[1024];
    
    char *d = "=";
    size_t dlen = strlen(d);
    ini *init = (ini *)malloc(sizeof(ini));
    if (!init) {
        return NULL;
    }
    
    while (fgets(line, 1025, fp)) {
        if (strlen(line)<=1) {
            continue;
        }
        
        char **rst = str_split(line, strlen(line), d, dlen);
        if (!rst) {
            continue;
        }
        
        if (!rst[1]) {
            free(rst);
            continue;
        }
        
        ini_set(&init, make_ini(rst[0], rst[1]));
        free(rst);
    }
    
    return init;
}

char *ini_get(ini *init, char *key)
{
    ini *ini_item = init;
    while (ini_item) {
        if (strcmp(ini_item->key, key) == 0) {
            return ini_item->value;
        }
        ini_item = ini_item->next;
    }
    
    return NULL;
}
