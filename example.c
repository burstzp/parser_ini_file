#include <stdio.h>
#include "./include/ini_file.h"

int main() {
    int total_bytes;
    char* buf = load_ini_file("config.ini",&total_bytes);
    char result[1024] = {0};

    read_section_string(buf,total_bytes,"test","a",result);
    char host[1024] = {0};
    read_section_string(buf,total_bytes,"redis","host",host);

    printf("result=%s,%d,%s\r\n",result,read_section_long(buf,total_bytes,"test","b"),host);
    ini_file_buffer_free(buf);

    return 0;
}
