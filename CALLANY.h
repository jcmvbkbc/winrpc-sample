/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Mon Jul 10 00:56:30 2000
 */
/* Compiler settings for D:\TEXT\ismd\ismd2\rpc\CALLANY.IDL:
    Os (OptLev=s), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __CALLANY_h__
#define __CALLANY_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __callAny_INTERFACE_DEFINED__
#define __callAny_INTERFACE_DEFINED__

/* interface callAny */
/* [explicit_handle][version][uuid] */ 

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
long callAny( 
    /* [in] */ handle_t session,
    /* [string][in] */ wchar_t __RPC_FAR *path,
    /* [string][in] */ unsigned char __RPC_FAR *fn,
    int n,
    /* [ref][size_is][out][in] */ struct par __RPC_FAR *p,
    /* [size_is][ref][out] */ struct exec __RPC_FAR *e);

long getInfo( 
    /* [in] */ handle_t session,
    /* [size_is][string][out] */ wchar_t __RPC_FAR *provider);



extern RPC_IF_HANDLE callAny_v1_0_c_ifspec;
extern RPC_IF_HANDLE callAny_v1_0_s_ifspec;
#endif /* __callAny_INTERFACE_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
