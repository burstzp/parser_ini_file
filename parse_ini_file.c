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
    ini *node = (ini *)malloc(sizeof(ini));
    strcpy(node->key, key);
    strcpy(node->value, value);
    
    node->next = NULL;
    return node;
}

void ini_set(ini **node, ini *i)
{
    i->next = *node;
    *node = i;
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
    ini *node = (ini *)malloc(sizeof(ini));
    if (!node) {
        return NULL;
    }
    
    while (fgets(line, 1024, fp)) {
        if (strlen(line)<=1) {
            continue;
        }
        
        char **rst = str_split(line, strlen(line), d, dlen);
        if (!rst) {
            continue;
        }
        
        trim(rst[0]);
        trim(rst[1]);
        if (!rst[1]) {
            free(rst);
            continue;
        }
        
        ini_set(&node, make_ini(rst[0], rst[1]));
        free(rst);
    }
    
    return node;
}

char *ini_get(ini *init, char *key)
{
    ini *node = init;
    while (node) {
        if (strcmp(node->key, key) == 0) {
            return node->value;
        }
        node = node->next;
    }
    
    return NULL;
}

void ini_destroy(ini *init)
{
    ini *node = init, *tmp = NULL;
    while (node) {
        tmp = node;
        node = node->next;
        free(tmp);
    }
}

char *last_char_is(const char *s, int c)
{
    char *sret;
    if (!s)
        return NULL;
    sret = (char *)s+strlen(s)-1;
    if (sret>=s && *sret == c)
        return sret;
    else
        return NULL;
}

void chomp(char *s)
{
    char *lc = last_char_is(s, '\n');

    if(lc)
        *lc = 0;
}

void trim(char *s)
{
    int len = strlen(s);

    /* trim trailing whitespace */
    while ( len > 0 && isspace(s[len-1]))
        s[--len]='\0';

    /* trim leading whitespace */
    memmove(s, &s[strspn(s, " \n\r\t\v")], len);
}
