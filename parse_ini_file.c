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
#include "parse_ini_file.h"

#define MALLOC_INI_PARAMS   (ini_params *)malloc(sizeof(ini_params))
#define MALLOC_INI          (ini *)malloc(sizeof(ini))

#define Z_ERROR              -1
#define Z_SUCCESS             0
#define NOT_FOUND            -1
#define RETURN_NULL          return NULL

/* found substring in string position LIKE PHP */
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

    return NOT_FOUND;
}

/* create ini params */
ini_params *create_ini_params(const char *key, const char *value)
{
    if (!key || !value) {
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

/* ini params print */
void visit(ini_params *ini_params_t)
{
    printf("%s = %s\n", ini_params_t->key, ini_params_t->value);
}

/* ini params print */
void print_ini_params_list(ini_params *ini_params_t, void (*visit)(ini_params*))
{
    for (; ini_params_t; ini_params_t = ini_params_t->next) {
        visit(ini_params_t);
    }
}

/* ini params get */
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

/* destroy ini params */
void destroy_ini_params(ini_params *ini_params_t)
{
    ini_params *q,*p = ini_params_t;
    while (p) {
        q = p;
        p = q->next;
        free(q);
    }
}

/* make ini */
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
    ini_t->ini_params_t = ini_params_t;
    return ini_t;
}

/* ini list ptr */
void ini_set(ini **ini_ptr, ini *ini_t)
{
    ini_t->next = *ini_ptr;
    *ini_ptr = ini_t;
}

/* found ini handle from section */
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

/* get section key */
char *get_section_key(char *key)
{
    size_t keylen = strlen(key);
    if (key[0] == '[' && key[keylen - 1] == ']') {
        return key;
    }

    char str[50] = {0};
    memset(str, 0, strlen(str));
    int i = 1;
    *str = '[';
    while (*key) {
        if (*key != '[' && *key != ']') {
            *(str + i) = *key;
            i++;
        }
        key++;
    }
    strcat(str, "]\0");
    memset(key, 0, strlen(key));
    memcpy(key, str, sizeof(str));
    return key;
}

/* ini get */
char *ini_get(ini *init_t, const char *section_key, const char *key)
{
    char seckey[50] = {0};
    memcpy(seckey, section_key, strlen(section_key));
    char *section = get_section_key(seckey);
    size_t sectionlen = strlen(section);
    for (; init_t; init_t = init_t->next) {
        if (!memcmp(init_t->key, section, sectionlen)) {
            return ini_params_get(init_t->ini_params_t, key);
        }
    }

    RETURN_NULL;
}

/* free ini handle */
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

/* need free */
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

/* clear comment */
int clear_comment(char *line)
{
    int pos = strpos(line, "#");
    if (NOT_FOUND == pos) {
        return Z_ERROR;     
    } 
    
    size_t linelen = strlen(line);
    for (; (line[pos] != '\n') && pos < linelen; pos++) {
        *(line + pos) = '\0'; 
    }

    return Z_SUCCESS;
}

ini *parse_ini_file(const char *filename)
{
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        fprintf(stderr, "read ini file error!");
        RETURN_NULL;
    }

    ini *ini_t      = NULL;
    ini *ini_t_ptr  = NULL;

    char line[1024];    // read one line from file
    char section[32];   // ini section name
    char *pk = NULL;    // ini params key
    char *pv = NULL;    // ini params value
    int  pos = NOT_FOUND; // found position

    memset(line, 0, sizeof(line));
    memset(section, 0, sizeof(section));

    while (fgets(line, 1024, fp)) {
        clear_comment(line);
        if (!line) continue;

        pos = strpos(line, "=");
        if (pos == NOT_FOUND && NOT_FOUND != strpos(line, "[") && NOT_FOUND != strpos(line, "]")) {
            memset(section, 0, sizeof(section));
            memcpy(section, line, strlen(line));
        } else if (pos != NOT_FOUND) {
            pk = substr(line, 0, pos);
            pv = substr(line, pos + 1, NOT_FOUND);
            ini_t_ptr = ini_search(ini_t, section);
            if (ini_t_ptr && ini_t_ptr->ini_params_t) {
                add_ini_params_node(&ini_t_ptr->ini_params_t, create_ini_params(pk, pv));
            } else {
                ini_set(&ini_t, create_ini(section, create_ini_params(pk, pv)));
            }
            free(pk);
            free(pv);
        }
    }
    fclose(fp);
    return ini_t;
}

void visit_section(ini *ini_t)
{
    printf("%s\n", ini_t->key);
}

void print_ini_list(ini *ini_t, void (*visit_section)(ini*))
{
    for (; ini_t; ini_t = ini_t->next) {
        visit_section(ini_t);
        print_ini_params_list(ini_t->ini_params_t, visit);
    }
}
#define DEBUG 1
#if DEBUG
int main(int argc, const char * argv[]) {
    ini *ini_t = parse_ini_file("/data1/code/c/parser_ini_file/a.ini");
    printf("value = %s\n", ini_get(ini_t, "[redis]", "host"));
    printf("value = %s\n", ini_get(ini_t, "db", "port"));
    printf("value = %s\n", ini_get(ini_t, "[mc]", "timeout"));

    print_ini_list(ini_t, visit_section);
    destroy_ini(ini_t);
    return 0;
}
#endif
