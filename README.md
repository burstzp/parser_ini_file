parser_ini_file
===============

linux c

用C解析ini文件

使用例子：

ini *ini_t = parse_ini_file("/Users/ekikokuiwa/data/code/c/ini/ini/a.ini");
    printf("value = %s\n", ini_get(ini_t, "[redis]", "host"));
    printf("value = %s\n", ini_get(ini_t, "db", "port"));
    printf("value = %s\n", ini_get(ini_t, "[mc]", "timeout"));

    destroy_ini(ini_t);
 
