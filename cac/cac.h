/////////////////////////////////////////////
//
//
//   callAny client interface definitions
//   (C) 2000 by dumb, Innocence Inc.
//   
//   2000-04-22	dumb -- created
//   2000-04-25	dumb -- added ntlm auth + added PKT_PRIVACY 
//                      (what to do in France?:)
//   2000-06-11	dumb -- fixed caExec ret. type to int :7;
//                      fixed caExec bug with buffer type CA_FLAG_IN :(
//   2000-07-09	dumb -- improved connection behavior, new arg in caConnect
//                      
//
//

typedef HANDLE han;
typedef unsigned ui;
typedef DWORD dw;

#ifdef UNICODE
#define	caConnect caConnectW
#define	caExec caExecW
#else
#define	caConnect caConnectA
#define	caExec caExecA
#endif

#ifndef __callAny_INTERFACE_DEFINED__
#define CA_FLAG_IN  1
#define CA_FLAG_OUT 2
#define CA_FLAG_PTR 4
struct  par
    {
    unsigned int flags;
    unsigned int size;
    unsigned int buf;
    /* [length_is][size_is][unique] */ unsigned char __RPC_FAR *data;
    };
struct  exec
    {
    unsigned long retCode;
    unsigned long lastError;
    };
#endif

#ifdef __cplusplus
extern "C"
{
#endif

dw WINAPI caClose(han connection);
/*
 returns: rpc status.
*/
han WINAPI caConnectW(WCHAR *host,WCHAR *port,WCHAR *protocol,WCHAR provider[MAX_PATH]);
han WINAPI caConnectA(const char *host,const char *port,const char *protocol,char provider[MAX_PATH]);
/*
 returns:
  !=0 -- connection handle; must be closed with caClose after all
  ==0 -- use GetLastError to obtain status of failure
*/

int WINAPI caExecW(han connection,const WCHAR *dllPath,const char *fn,int nPar,par *p,void *retcode=0);
int WINAPI caExecA(han connection,const char *dllPath,const char *fn,int nPar,par *p,void *retcode=0);
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
#ifdef __cplusplus
}
#endif

