// rcopy.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../cac/cac.h"


int __cdecl main(int c,char* v[])
{
 char prov[MAX_PATH];
 if(c<4)
 {
  printf("%s server source destination\n",v[0]);
  return 0;
 }
 han srv=caConnect(v[1],"\\\\pipe\\cc$","ncacn_np",prov);
 if(srv)
 {
  printf("using %s\n",prov);
  dw rc,acc=GENERIC_READ|GENERIC_WRITE;
  dw share=FILE_SHARE_READ|FILE_SHARE_WRITE;
  dw cd=CREATE_ALWAYS;
  par p1[]={
   {CA_FLAG_IN|CA_FLAG_PTR,lstrlen(v[3])+1,0,(BYTE*)v[3]},
   {CA_FLAG_IN,sizeof(acc),0,(BYTE*)&acc},
   {CA_FLAG_IN,sizeof(share),0,(BYTE*)&share},
   {CA_FLAG_PTR,0,0,0},
   {CA_FLAG_IN,sizeof(cd),0,(BYTE*)&cd},
   {CA_FLAG_PTR,0,0,0},
   {CA_FLAG_PTR,0,0,0}};

  han src=CreateFile(v[2],GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,
   0,OPEN_EXISTING,0,0);
  if(src!=INVALID_HANDLE_VALUE)
  {
   dw len=GetFileSize(src,0);
   char *buf=new char[len];
   ReadFile(src,buf,len,&len,0);
   CloseHandle(src);

   if(caExec(srv,"kernel32.dll","CreateFileA",7,p1,&rc)>0 && rc!=~0)
   {
    dw wr,r2;
    par p2[]={
     {CA_FLAG_IN,sizeof(rc),0,(BYTE*)&rc},
     {CA_FLAG_IN|CA_FLAG_PTR,len,0,(BYTE*)buf},
     {CA_FLAG_IN,sizeof(len),0,(BYTE*)&len},
     {CA_FLAG_OUT|CA_FLAG_PTR,sizeof(wr),0,(BYTE*)&wr},
     {CA_FLAG_PTR,0,0,0}},
    p3[]={{CA_FLAG_IN,sizeof(rc),0,(BYTE*)&rc}};

    if(caExec(srv,"kernel32.dll","WriteFile",5,p2,&r2)<=0 || !r2)
     printf("rWriteFile: %d\n",GetLastError());
    caExec(srv,"kernel32.dll","CloseHandle",1,p3,&r2);
   }
   else
    printf("rCreateFile: %d\n",GetLastError());
  }
  else
   printf("lCreateFile: %d\n",GetLastError());
  caClose(srv);
 }
 else
  printf("server @%s is not responding\n",v[1]);
 return 0;
}
