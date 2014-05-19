#include "stdafx.h"
#include "../callany.h"
#include "cas.h"
#include <stdio.h>

long getInfo( 
    /* [in] */ handle_t session,
    /* [size_is][string][out] */ wchar_t __RPC_FAR *provider)
{
 MEMORY_BASIC_INFORMATION mbi;
 VirtualQuery(&getInfo,&mbi,sizeof(mbi));
 return GetModuleFileNameW((han)mbi.AllocationBase,provider,MAX_PATH);
}

long callAny( 
    /* [in] */ handle_t session,
    /* [string][in] */ wchar_t __RPC_FAR *path,
    /* [string][in] */ unsigned char __RPC_FAR *fn,
    int n,
    /* [ref][size_is][out][in] */ struct par __RPC_FAR *p,
    /* [size_is][ref][out] */ struct exec __RPC_FAR *e)
{
#ifndef NDEBUG
 printf("sess %08x: %ls::%s with %d parameters\n",session,path,fn,n);
#endif
 dw rc=0;
 han lib=LoadLibraryW(path);
 void *f=GetProcAddress(lib,(char*)fn);
 if(lib && f)
 {
  __try
  {
   void *d,*stt;
   unsigned int size,res;

   __asm mov stt,esp
   for(int i=n-1;i>=0;i--)
   {
    d=p[i].data;
    size=p[i].size;
    p[i].buf=(p[i].flags & CA_FLAG_OUT)?p[i].size:0;

    if(p[i].flags & CA_FLAG_PTR)//ptr type
     __asm push d
    else
     if(d)
     {
      __asm mov ecx,[d]
      __asm push dword ptr [ecx]
     }
     else
      __asm push d;//==push 0;
   }
   __asm call f
   __asm mov res,eax
   __asm mov esp,stt
   e->retCode=res;
   rc++;
  }
  __except(EXCEPTION_EXECUTE_HANDLER)
  {
   rc--;
#ifndef NDEBUG
   puts("Exception during the call :(");
#endif
  }
 }
 e->lastError=GetLastError();
 FreeLibrary(lib);
 return rc;
}

dw WINAPI casStop(int wait)
{
 RpcMgmtStopServerListening(0);
 if(wait)
  RpcMgmtWaitServerListen();
 return 0;
}

dw WINAPI casStartW(WCHAR *protSeq,WCHAR *endpoint,int maxCalls,int minCalls,int fDontWait)
{
 dw rc;
 SECURITY_DESCRIPTOR sd;

 InitializeSecurityDescriptor(&sd,SECURITY_DESCRIPTOR_REVISION);
 SetSecurityDescriptorDacl(&sd,1,0,0);
 if((rc=RpcServerUseProtseqEpW(
                protSeq,
                maxCalls,    
                endpoint,
                &sd))!=RPC_S_OK)  
  return rc;

 if((rc=RpcServerRegisterAuthInfo(0,RPC_C_AUTHN_WINNT,0,0))!=RPC_S_OK)
  return rc;
 if((rc=RpcServerRegisterIf(
              callAny_v1_0_s_ifspec,
              NULL,   // MgrTypeUuid
              NULL)   // MgrEpv; null means use default
	      )!=RPC_S_OK)
  return rc;
 return RpcServerListen(minCalls,maxCalls,fDontWait);
}

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

dw WINAPI casStartA(char *protSeq,char *endpoint,int maxCalls,int minCalls,int fDontWait)
{
 dw rc;
 WCHAR *ps,*ep;

 ps=a2w(protSeq);
 ep=a2w(endpoint);
 rc=casStartW(ps,ep,maxCalls,minCalls,fDontWait);
 delete ps;
 delete ep;
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
