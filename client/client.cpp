#include "stdafx.h"
#include <stdio.h>
#include "../cac/cac.h"

void __cdecl main(int i,char **v)
{
 BYTE buf[10000];
 dw len=10000;
 par p[4]={{5,0,0,0},{5,13,0,(BYTE*)"Hello, world"},{5,0,0,0},{5,0,0,0}},
     p1[]={{6,len,0,buf},{7,4,0,(BYTE*)&len}};

 if(i<2)
  puts("client host [endpoint [protocol]]");
 else
 {
  han sess=caConnect(v[1],v[2],v[3]);
  //caExec(sess,"user32.dll","MessageBoxA",4,p);  
  caExec(sess,"advapi32.dll","GetUserNameA",2,p1);
  printf("GetUserName returned %s\n",buf);
  caClose(sess);
 }
}
