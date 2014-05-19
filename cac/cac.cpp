#include "stdafx.h"
#include "../callany.h"
#include "cac.h"

WCHAR *a2w(const char *in)
{
 dw cc;
 if(in)
 {
  WCHAR *out;
  out=new WCHAR[cc=lstrlen(in)+1];
  MultiByteToWideChar(CP_ACP,0,in,-1,out,cc);
  return out;
 }
 return 0;
}

dw WINAPI caClose(han connection)
{
 return RpcBindingFree(&connection);
}

han WINAPI caConnectW(WCHAR *host,WCHAR *port,WCHAR *protocol,WCHAR provider[MAX_PATH])
/*
 returns:
  !=0 -- connection handle; must be closed with caClose after all
  ==0 -- use GetLastError to obtain status of failure
*/
{
 WCHAR *pszStringBinding=0;
 han ch=0;
 RPC_STATUS status;
 __try
 {
  __try
  {
   if((status=RpcStringBindingComposeW(
		 0/*pszUuid*/,
		 protocol,
		 host,
		 port,
		 0,
		 &pszStringBinding))!=RPC_S_OK)
    __leave;
   if((status=RpcBindingFromStringBindingW(
		pszStringBinding,
		&ch))!=RPC_S_OK)
    __leave;
   if((status=RpcBindingSetAuthInfo(ch,0,RPC_C_AUTHN_LEVEL_PKT_PRIVACY,
    RPC_C_AUTHN_WINNT,0,0))!=RPC_S_OK)
    __leave;
   {
    /*
    dw sz=MAX_PATH,rc;
   
    par p[3]={
     {CA_FLAG_PTR,0,0,0},
     {CA_FLAG_OUT|CA_FLAG_PTR,MAX_PATH,0,(BYTE*)provider},
     {CA_FLAG_IN,sizeof(dw),0,(BYTE*)&sz}};
    status=caExecW(ch,L"kernel32.dll","GetModuleFileNameW",3,p,&rc)>0?RPC_S_OK:RPC_S_INVALID_BINDING;*/
    status=RPC_S_INVALID_BINDING;
    status=getInfo(ch,provider)?RPC_S_OK:RPC_S_INVALID_BINDING;
   }
  }
  __finally
  {
   RpcStringFreeW(&pszStringBinding);
   if(status!=RPC_S_OK && ch)
    RpcBindingFree(&ch);
   SetLastError(status);
  }
 }
 __except(EXCEPTION_EXECUTE_HANDLER)
 {
 }
 return ch;
}

han WINAPI caConnectA(const char *host,const char *port,const char *protocol,char provider[MAX_PATH])
{
 han ch;
 WCHAR *h,*p,*r,pr[MAX_PATH];
 h=a2w(host);
 p=a2w(port);
 r=a2w(protocol);
 ch=caConnectW(h,p,r,pr);
 delete h;
 delete p;
 delete r;
 WideCharToMultiByte(CP_ACP,0,pr,-1,provider,MAX_PATH,0,0);
 return ch;
}

int WINAPI caExecW(han connection,const WCHAR *dllPath,const char *fn,int nPar,par *p,void *retcode)
/*
 returns:
   > 0 -- call executed remotely; 
          *retcode contains fn's retcode, GetLastError set up properly
  == 0 -- call failed remotely -- library or entrypoint not found;
          GetLastError set up properly
  ==-1 -- remote call exception; GetLastError contains exception code
  ==-2 -- local exception; GetLastError contains exception code
	    possibly directional flags set up incorrectly,
	    causing writing into const etc...

 on enter:
  connection, dllPath, fn, nPar && p MUST be valid (othw. -- -2);
  each p's flags, size & data MUST be valid
*/
{
 dw rc;
 __try
 {
  exec e;

  for(int i=0;i<nPar;i++)
   p[i].buf=(p[i].flags & CA_FLAG_IN)?p[i].size:0;
  rc=callAny(connection,(WCHAR*)dllPath,(BYTE*)fn,nPar,p,&e);
  if(retcode)
   *((dw*)retcode)=e.retCode;
  SetLastError(e.lastError);
 }
 __except(EXCEPTION_EXECUTE_HANDLER)
 {
  rc=-2;
 }
 return rc;
}

int WINAPI caExecA(han connection,const char *dllPath,const char *fn,int nPar,par *p,void *retcode)
{
 dw rc;
 WCHAR *path;
 path=a2w(dllPath);
 rc=caExecW(connection,path,fn,nPar,p,retcode);
 delete path;
 return rc;
}

void __RPC_FAR * __RPC_USER midl_user_allocate(size_t len)
{
 return(new char[len]);
}

void __RPC_USER midl_user_free(void __RPC_FAR * ptr)
{
 delete ptr;
}
