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
 
// 无内存泄露，valgrind检测结果如下<br>
 valgrind --leak-check=full ./parse_ini_file <br>
 ==4925== <br>
==4925== HEAP SUMMARY:<br>
==4925==     in use at exit: 0 bytes in 0 blocks<br>
==4925==   total heap usage: 25 allocs, 25 frees, 1,979 bytes allocated<br>
==4925== 
==4925== All heap blocks were freed -- no leaks are possible<br>
==4925== 
==4925== For counts of detected and suppressed errors, rerun with: -v<br>
==4925== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 4 from 4)<br>
