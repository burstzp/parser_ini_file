parser_ini_file
===============

linux c

用C解析ini文件

支持"#"注释, 不支持value里面有出现多行

使用例子：

// 解析INI 文件<br>
ini *ini_t = parse_ini_file("/Users/ekikokuiwa/data/code/c/ini/ini/a.ini");<br>
// 获取ini值，redis为section值<br>
printf("value = %s\n", ini_get(ini_t, "redis", "host"));<br>
printf("value = %s\n", ini_get(ini_t, "db", "port"));<br>
printf("value = %s\n", ini_get(ini_t, "mc", "timeout"));<br><br>
// 显示ini内容<br>
print_ini_list(ini_t, visit_section);<br>
// 销毁ini对象<br>
destroy_ini(ini_t);
 
