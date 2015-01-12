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

int strpos(const char *str, const char *find);
ini_params *create_ini_params(const char *key, const char *value);
void add_ini_params_node(ini_params **ini_params_t, ini_params *node);
void visit(ini_params *ini_params_t);
void print_ini_params_list(ini_params *ini_params_t, void (*visit)(ini_params*));
char *ini_params_get(ini_params *ini_params_t, const char *key);
void destroy_ini_params(ini_params *ini_params_t);
ini *create_ini(const char *key, ini_params *ini_params_t);
void ini_set(ini **ini_ptr, ini *ini_t);
ini *ini_search(ini *init_t, const char *section);
char *get_section_key(char *key);
char *ini_get(ini *init_t, const char *section_key, const char *key);
void destroy_ini(ini *ini_t);
char *substr(const char *str, size_t start, size_t len);
ini *parse_ini_file(const char *filename);
void visit_section(ini *ini_t);
void print_ini_list(ini *ini_t, void (*visit_section)(ini*));

#endif
