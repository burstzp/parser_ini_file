#include "../include/ini_file.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>

#include <unistd.h>

const int MAX_INI_FILE_BUF_SIZE = 10240;

int is_pair_value(const char* sp, const char *ep, const char **pos) {
   while (sp<ep) {
        if (*sp=='=') {
            *pos = sp;
            return 1;
        }
        sp++;
   }

   return 0;
}

char* load_ini_file(char* file_path, int* total_bytes) {
    assert(file_path != NULL);
    int fd = open(file_path, O_RDONLY);
    if (fd < 0) {
        close(fd);
        printf("file open error '%s'", strerror(errno));
        return NULL;
    }

    char *buf = (char*)malloc(MAX_INI_FILE_BUF_SIZE);
    memset(buf,0,MAX_INI_FILE_BUF_SIZE);
    assert(buf != NULL);
    int n;
    *total_bytes = 0;
    while ((n = read(fd, buf, MAX_INI_FILE_BUF_SIZE)) > 0) {
        *total_bytes = *total_bytes + n;
    }

    close(fd);
    if (total_bytes<=0) {
        ini_file_buffer_free(buf);
        return NULL;
    }
    buf[*total_bytes-1]='\0';
    return buf;
}

void ini_file_buffer_free(char* buf) {
    if (buf!=NULL) {
        free(buf);
        buf=NULL;
    }
}

int read_section_int(const char* buf,int total_bytes , char* section, char* key) {
    char result[4]={0};
    read_section_string(buf, total_bytes, section,key, result);
    return atoi(result);
}

long read_section_long(const char* buf,int total_bytes , char* section, char* key) {
    char result[8]={0};
    read_section_string(buf, total_bytes, section,key, result);
    return atol(result);
}

void read_section_string(const char* buf, int total_bytes , char* section, char* key, char* result) {
    assert(total_bytes>1);
    assert(buf!=NULL);
    assert(section!=NULL);
    assert(key!=NULL);
    assert(result!=NULL);

    int i = 0;
    const char *sp = buf;
    const char *ep = buf;

    int found = 0;
    int section_len = strlen(section);
    while (i < total_bytes) {
        if (buf[i] == '\r') {
            ep = buf + i;
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

        const char *pos=NULL;
        if (found && is_pair_value(sp, ep, &pos)) {
            if (memcmp(key,sp,pos-sp) == 0) {
                memcpy(result,pos+1,ep-pos);
                result[ep-pos+1]='\0';
                break;
            }
        } else {
            sp++;
            ep--;
            if (sp > ep) {
                continue;
            }

            found = 0;
            int l = ep-sp+1;
            if (l<section_len) l = section_len;
            if (memcmp(section,sp,l) == 0) {
                found = 1;
            }

            ep++;
        }

        sp = ep;
        sp++;
    }
}





