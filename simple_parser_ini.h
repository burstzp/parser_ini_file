//
//  ini_file_parser.h
//  gateway
//
//  Created by 易国磐 on 17-1-5.
//  Copyright (c) 2017年 易国磐. All rights reserved.
//

#ifndef __gateway__ini_file_parser__
#define __gateway__ini_file_parser__

#include <stdio.h>

#define INI_FILE_PARSER(x,o) ini_file_to_buffer((x),o,sizeof(o))
#define READ_SECION_STRING(x,s,k,o) read_section_string((x),sizeof(x),s,k,o)
#define READ_SECION_INT(x,s,k) read_section_int((x),sizeof(x),s,k)
#define READ_SECION_LONG(x,s,k) read_section_long((x),sizeof(x),s,k)

char* getpos(char* sp, char *ep);
int ini_file_to_buffer(char* file_path, char* buffer, size_t buffer_capacity);
size_t read_section_string(const char* buffer,
                        int buffer_capacity,
                        char* section,
                        char* key,
                        char* to);
#endif /* defined(__gateway__ini_file_parser__) */
