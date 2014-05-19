/////////////////////////////////////////////
//
//
//   callAny server interface definitions
//   (C) 2000 by dumb, Innocence Inc.
//   
//   2000-04-22	dumb -- created
//   2000-04-25	dumb -- fixed ncacn_np security bug + added ntlm auth
//                      + added PKT_PRIVACY (what to do in France?:)
//
//

typedef HANDLE han;
typedef unsigned ui;
typedef DWORD dw;

#ifdef UNICODE
#define	casStart casStartW
#else
#define	casStart casStartA
#endif

#ifdef __cplusplus
extern "C"
{
#endif

dw WINAPI casStartW(WCHAR *protSeq,WCHAR *endpoint,int maxCalls,int minCalls,int fDontWait);
dw WINAPI casStartA(char *protSeq,char *endpoint,int maxCalls,int minCalls,int fDontWait);
dw WINAPI casStop(int wait=1);

#ifdef __cplusplus
}
#endif
