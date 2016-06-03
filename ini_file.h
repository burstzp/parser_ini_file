#ifndef INI_FILE_H
#define INI_FILE_H

char* load_ini_file(char* file_path,int* total_bytes);
void ini_file_buffer_free(char* buf);
int read_section_int(const char* buf, int total_bytes, char* section, char* key);
long read_section_long(const char* buf, int total_bytes, char* section, char* key);
void read_section_string(const char* buf, int total_bytes , char* section, char* key, char* result, int result_len);

#endif


