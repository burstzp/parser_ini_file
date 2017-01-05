//
//  ini_file_parser.c
//  gateway
//
//  Created by 易国磐 on 17-1-5.
//  Copyright (c) 2017年 易国磐. All rights reserved.
//

#include "ini_file_parser.h"

#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int ini_file_to_buffer(char* file_path, char* buffer, size_t buffer_capacity) {
    assert(file_path!=NULL);
    assert(buffer!=NULL);
    
    int fd = open(file_path, O_RDONLY);
    if (fd < 0) {
        close(fd);
        fprintf(stderr, "file:%s, open fail. error '%s'", file_path, strerror(errno));
        return 0;
    }
    
    ssize_t total_bytes = 0;
    total_bytes = read(fd,buffer,buffer_capacity);
    close(fd);
    
    if (total_bytes <= 0) {
        fprintf(stderr, "file:%s, can't read. error '%s'", file_path, strerror(errno));
        return 0;
    }
    
    buffer[total_bytes-1]=0;
    return (int)total_bytes-1;
}

char *skip_wss(const char *string) {
    unsigned int c;
    
    while((c = *string) == ' ' || c == '\t' || c == '\r' || c == '\n') {
        string++;
    }
    
    return (char *)string;
}

int read_section_int(const char* buffer,
                     int buffer_capacity,
                     char* section,
                     char* key) {
    char result[4]={0};
    read_section_string(buffer, buffer_capacity, section,key, result);
    return atoi(result);
}

long read_section_long(const char* buffer,
                       int buffer_capacity,
                       char* section,
                       char* key) {
    char result[8]={0};
    read_section_string(buffer, buffer_capacity, section,key, result);
    return atol(result);
}

size_t read_section_string(const char* buffer,
                          int buffer_capacity,
                          char* section,
                          char* key,
                          char* to) {
    
    assert(buffer!=NULL&&section!=NULL&&key!=NULL&&to!=NULL);
    
    const char* sp = buffer;
    const char* ep = buffer;
    
    int i = 0;
    int found = 0;
    size_t k = 0;
    while (i < buffer_capacity) {
        if (buffer[i]=='#') {
        
        }
        
        if (buffer[i] == '\n') {
            ep = buffer + i;
        } else {
            i++;
            continue;
        }
        
        while(*sp != '\0' && (*sp==' ' || *sp=='\r' || *sp=='\n' || *sp=='\t')) sp++;
        while(*ep != '\0' && (*ep==' ' || *ep=='\r' || *ep=='\n' || *ep=='\t')) ep--;
        
        i++;
        if (sp > ep) {
            continue;
        }
        
        char *pos=NULL;
        if (found) {
            pos = getpos(sp, ep);
            if (memcmp(key,sp,pos-sp) == 0) {
                memcpy(to,pos+1,ep-pos);
                k = ep - pos;
                to[k]='\0';
                break;
            }
        } else {
            sp++;
            ep--;
            if (sp > ep) {
                continue;
            }
            
            found = 0;
            size_t l = ep - sp + 1;
            if (l < strlen(section)) l = strlen(section);
            
            if (memcmp(section,sp,l) == 0) {
                found = 1;
            }
            
            ep++;
        }
        
        sp = ep;
        sp++;
    }
    
    return k;
}

char* getpos(char* sp, char *ep) {
    while (sp<ep) {
        if (*sp=='=') {
          //  *pos = sp;
            return sp;
        }
        sp++;
    }
    
    return NULL;
}
