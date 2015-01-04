//
//  main.c
//  paser_ini_file
//
//  Created by 易国磐 on 14-12-28.
//  Copyright (c) 2014年 易国磐. All rights reserved.
//

#include "parse_ini_file.h"
//TCBDB *db;
int main(int argc, const char * argv[])
{
    ini *init = parse_ini_file("./a.ini");
    ini_set(&init, make_ini("need", "1"));
    char *value = ini_get(init, "host");
    printf("\nresult\nhost = %s\n", value);
    printf("port = %s\n", ini_get(init, "port"));
    printf("key = %s\n", ini_get(init, "key"));
    printf("key = %s\n", ini_get(init, "need1"));
    ini_destroy(init); 
    return 0;
}
