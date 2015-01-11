//
//  main.c
//  ini
//
//  Created by 易国磐 on 15-1-9.
//  Copyright (c) 2015年 易国磐. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ini.h"

#define MALLOC_INI_PARAMS   (ini_params *)malloc(sizeof(ini_params))
#define MALLOC_INI          (ini *)malloc(sizeof(ini))

#define Z_ERROR              -1
#define Z_SUCCESS             0
#define RETURN_NULL          return NULL

int strpos(const char *str, const char *find)
{
    size_t findlen = strlen(find);
    size_t str_len = strlen(str);
 
    int i = 0;
    do {
        if (memcmp(str + i, find, findlen) == 0) {
            return i;
        }
    } while (i++ < str_len);
    
    return Z_ERROR;
}

ini_params *create_ini_params(const char *key, const char *value)
{
    if (is_empty(key) || is_empty(value)) {
        RETURN_NULL;
    }
    
    ini_params *init_param_t = MALLOC_INI_PARAMS;
    if (!init_param_t) {
        RETURN_NULL;
    }
    
    memset(init_param_t,        0, sizeof(ini_params));
    memset(init_param_t->key,   0, sizeof(init_param_t->key));
    memset(init_param_t->value, 0, sizeof(init_param_t->value));
    
    memcpy(init_param_t->key,   key, strlen(key));
    memcpy(init_param_t->value, value, strlen(value));
    init_param_t->next = NULL;
    return init_param_t;
}

void add_ini_params_node(ini_params **ini_params_t, ini_params *node)
{
    node->next = *ini_params_t;
    *ini_params_t = node;
}

void visit(ini_params *ini_params_t)
{
    printf("ini_param key = %s, value = %s\n", ini_params_t->key, ini_params_t->value);
}

void print_ini_params_list(ini_params *ini_params_t, void (*visit)(ini_params*))
{
    for (; ini_params_t; ini_params_t = ini_params_t->next) {
        visit(ini_params_t);
    }
}

char *ini_params_get(ini_params *ini_params_t, const char *key)
{
    size_t keylen = strlen(key);
    for (; ini_params_t; ini_params_t = ini_params_t->next) {
        if (!memcmp(ini_params_t->key, key, keylen)) {
            return ini_params_t->value;
        }
    }
    RETURN_NULL;
}

void destroy_ini_params(ini_params *ini_params_t)
{
    ini_params *q,*p = ini_params_t;
    while (p) {
        q = p;
        p = q->next;
        free(q);
    }
}

ini *create_ini(const char *key, ini_params *ini_params_t)
{
    if (!key || !ini_params_t) {
        RETURN_NULL;
    }
    
    ini *ini_t = MALLOC_INI;
    if (!ini_t) {
        RETURN_NULL;
    }
    memset(ini_t, 0, sizeof(ini));
    memset(ini_t->key, 0, sizeof(ini_t->key));
    
    memcpy(ini_t->key, key, strlen(key));
    
    ini_t->ini_params_t = NULL;
    add_ini_params_node(&ini_t->ini_params_t, ini_params_t);
    return ini_t;
}

void ini_set(ini **ini_ptr, ini *ini_t)
{
    ini_t->next = *ini_ptr;
    *ini_ptr = ini_t;
}

ini *ini_search(ini *init_t, const char *section)
{
    size_t sectionlen = strlen(section);
    for (; init_t; init_t = init_t->next) {
        if (!memcmp(init_t->key, section, sectionlen)) {
            return init_t;
        }
    }
    
    RETURN_NULL;
}

const char *get_section_key(const char *key)
{
    size_t keylen = strlen(key);
    if (key[0] == '[' && key[keylen - 1] == ']') {
        return key;
    }
    
    char str[50];// = (char *)malloc(sizeof(char) * keylen);
    int i = 1;
    *str = '[';
    while (*key) {
        if (*key != '[' && *key != ']') {
            *(str + i) = *key;
            i++;
        }
        key++;
    }
    
    *(str + i++) = ']';
    *(str + i)   = '\0';
    
    return str;
}

char *ini_get(ini *init_t, const char *section_key, const char *key)
{
    const char *section = get_section_key(section_key);
    size_t sectionlen = strlen(section);
    for (; init_t; init_t = init_t->next) {
        if (!memcmp(init_t->key, section, sectionlen)) {
            return ini_params_get(init_t->ini_params_t, key);
        }
    }
    
    RETURN_NULL;
}

void destroy_ini(ini *ini_t)
{
    ini *q,*p = ini_t;
    while (p) {
        q = p;
        p = q->next;
        destroy_ini_params(q->ini_params_t);
        free(q);
    }
}

char *substr(const char *str, size_t start, size_t len)
{
    size_t size;
    if (len == -1) {
        size = strlen(str) - start;
    } else {
        size = len;
    }
    char *ret = (char *)malloc(sizeof(char) * size + 1);
    memcpy(ret, str + start, size);
    *(ret + size) = '\0';
    return ret;
}

ini *parse_ini_file(const char *filename)
{
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        fprintf(stderr, "read ini file error!");
        RETURN_NULL;
    }
    
    ini *ini_t      = NULL;// = MALLOC_INI;
    ini *ini_t_ptr  = NULL;
    
    char line[1024];
    char section[32];
    
    memset(line, 0, sizeof(line));
    memset(section, 0, sizeof(section));
    
    int     pos = -1;
    char    *pk = NULL;
    char    *pv = NULL;
    
    while (fgets(line, 1024, fp)) {
        pos = strpos(line, "=");
        if (-1 != pos && section) {
            pk = substr(line, 0, pos);
            pv = substr(line, pos + 1, -1);
            ini_t_ptr = ini_search(ini_t, section);
            if (ini_t_ptr && ini_t_ptr->ini_params_t) {
                add_ini_params_node(&ini_t_ptr->ini_params_t, create_ini_params(pk, pv));
            } else {
                ini_set(&ini_t, create_ini(section, create_ini_params(pk, pv)));
            }
            free(pk);
            free(pv);
        } else {
            memset(section, 0, sizeof(section));
            memcpy(section, line, strlen(line));
        }
    }
    fclose(fp);
    return ini_t;
}

int main(int argc, const char * argv[]) {
    ini *ini_t = parse_ini_file("/Users/ekikokuiwa/data/code/c/ini/ini/a.ini");
    printf("value = %s\n", ini_get(ini_t, "[redis]", "host"));
    printf("value = %s\n", ini_get(ini_t, "db", "port"));
    printf("value = %s\n", ini_get(ini_t, "[mc]", "timeout"));

    destroy_ini(ini_t);
    return 0;
}
